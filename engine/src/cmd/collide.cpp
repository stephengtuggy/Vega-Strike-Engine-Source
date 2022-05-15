/*
 * collide.cpp
 *
 * Copyright (C) 2001-2022 Daniel Horn, safemode, surfdargent, griwodz,
 * ace123, pheonixstorm, dan_w, pyramid3d, Roy Falk, Stephen G. Tuggy
 * and other Vega Strike contributors
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "cmd/collide.h"
#include "vegastrike.h"
#include "unit_generic.h"
#include "beam.h"
#include "bolt.h"
//#include "gfx/mesh.h"
#include "unit_collide.h"
#include "physics.h"
#include "universe.h"

#include "collide2/CSopcodecollider.h"
//#include "collide2/csgeom2/optransfrm.h"
//#include "collide2/basecollider.h"

#include "hashtable.h"
#include <string>
#include "vs_globals.h"
//#include "configxml.h"
#include "vs_logging.h"

float loge2 = log(2.0f);

csOPCODECollider *collideTrees::colTree(Unit *un, const Vector &othervelocity) {
    const float const_factor = 1;
    float magsqr = un->GetVelocity().MagnitudeSquared();
    float newmagsqr = (un->GetVelocity() - othervelocity).MagnitudeSquared();
    float speedsquared = const_factor * const_factor * (magsqr > newmagsqr ? newmagsqr : magsqr);
    static unsigned int max_collide_trees = static_cast<unsigned int>(XMLSupport::parse_int(vs_config->getVariable(
            "physics",
            "max_collide_trees",
            "16384")));
    if (un->rSize() * un->rSize() > simulation_atom_var * simulation_atom_var * speedsquared
            || max_collide_trees == 1) {
        return rapidColliders[0];
    }
    if (rapidColliders[0] == NULL) {
        return NULL;
    }
    if (un->rSize() <= 0.) {      //Shouldn't happen bug I've seen this for asteroid fields...
        return NULL;
    }
    //Force pow to 0 in order to avoid nan problems...
    unsigned int pow = 0;
    if (pow >= collideTreesMaxTrees || pow >= max_collide_trees) {
        pow = collideTreesMaxTrees - 1;
    }
    int val = 1 << pow;
    if (rapidColliders[pow] == NULL) {
        rapidColliders[pow] = un->getCollideTree(Vector(1, 1, val));
    }
    return rapidColliders[pow];
}

std::shared_ptr<collideTrees> collideTrees::Get(const std::string &hash_key) {
    const auto& existing = UnitColliders().find(hash_key);
    if (existing == UnitColliders().end()) {
        return nullptr;
    }
    std::weak_ptr<collideTrees> my_weak_ptr = existing->second;
    std::shared_ptr<collideTrees> my_shared_ptr = my_weak_ptr.lock();
    if (!my_shared_ptr) {
        return nullptr;
    }
    return my_shared_ptr->shared_from_this();
}

std::shared_ptr<collideTrees> collideTrees::Create(const string &hk, csOPCODECollider *cT, csOPCODECollider *cS) {
    std::shared_ptr<collideTrees> ret_val = std::shared_ptr<collideTrees>(new collideTrees());
    for (auto & rapidCollider : ret_val->rapidColliders) {
        rapidCollider = nullptr;
    }
    ret_val->rapidColliders[0] = cT;
    ret_val->colShield = cS;
    std::weak_ptr<collideTrees> weak_ptr_to_ret_val(ret_val);
    UnitColliders()[hk] = weak_ptr_to_ret_val;
    return ret_val;
}

collideTrees::~collideTrees() {
    UnitColliders().erase(hash_key);
    for (auto & rapidCollider : rapidColliders) {
        if (rapidCollider) {
            delete rapidCollider;
            rapidCollider = nullptr;
        }
    }
    if (colShield) {
        delete colShield;
        colShield = nullptr;
    }
}

bool TableLocationChanged(const QVector &Mini, const QVector &minz) {
    return _Universe->activeStarSystem()->collide_table->c.hash_int(Mini.i)
            != _Universe->activeStarSystem()->collide_table->c.hash_int(minz.i)
            || _Universe->activeStarSystem()->collide_table->c.hash_int(Mini.j)
                    != _Universe->activeStarSystem()->collide_table->c.hash_int(minz.j)
            || _Universe->activeStarSystem()->collide_table->c.hash_int(Mini.k)
                    != _Universe->activeStarSystem()->collide_table->c.hash_int(minz.k);
}

bool TableLocationChanged(const LineCollide &lc, const QVector &minx, const QVector &maxx) {
    return TableLocationChanged(lc.Mini, minx) || TableLocationChanged(lc.Maxi, maxx);
}

void KillCollideTable(LineCollide *lc, StarSystem *ss) {
    if (lc->type == LineCollide::UNIT) {
        ss->collide_table->c.Remove(lc, lc->object.u);
    } else {
        VS_LOG(warning, (boost::format("such collide types as %1$d not allowed") % lc->type));
    }
}

bool EradicateCollideTable(LineCollide *lc, StarSystem *ss) {
    if (lc->type == LineCollide::UNIT) {
        return ss->collide_table->c.Eradicate(lc->object.u);
    } else {
        VS_LOG(warning, (boost::format("such collide types as %1$d not allowed") % lc->type));
        return false;
    }
}

void AddCollideQueue(LineCollide &tmp, StarSystem *ss) {
    if (tmp.type == LineCollide::UNIT) {
        ss->collide_table->c.Put(&tmp, tmp.object.u);
    } else {
        VS_LOG(warning, (boost::format("such collide types as %1$d not allowed") % tmp.type));
    }
}

bool lcwithin(const LineCollide &lc, const LineCollide &tmp) {
    return lc.Mini.i < tmp.Maxi.i
            && lc.Mini.j < tmp.Maxi.j
            && lc.Mini.k < tmp.Maxi.k
            && lc.Maxi.i > tmp.Mini.i
            && lc.Maxi.j > tmp.Mini.j
            && lc.Maxi.k > tmp.Mini.k;
}

std::unordered_map<std::string, std::weak_ptr<collideTrees>> & UnitColliders() {
    static std::unordered_map<std::string, std::weak_ptr<collideTrees>> unit_colliders;
    return unit_colliders;
}

bool usehuge_table() {
    const unsigned int A = 9301;
    const unsigned int C = 49297;
    const unsigned int M = 233280;
    static unsigned int seed = 3259235;
    seed = (seed * A + C) % M;
    return seed < (M / 100);
}

bool Bolt::Collide(Collidable::CollideRef index) {
    return _Universe->activeStarSystem()->collide_map[Unit::UNIT_BOLT]->CheckCollisions(this, **location);
}
