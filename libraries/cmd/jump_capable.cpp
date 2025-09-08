/*
 * jump_capable.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
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


#include "cmd/jump_capable.h"
#include "cmd/ai/order.h"
#include "cmd/unit_find.h"
#include "src/universe.h"
#include "src/universe_util.h"
#include "gfx/warptrail.h"
#include "root_generic/vsfilesystem.h"
#include "src/vs_exit.h"
#include "src/vega_cast_utils.h"
#include "root_generic/configxml.h"

// TODO: once implementation is refactored, deal with this too
extern QVector RealPosition(const Unit *un);
extern float getAutoRSize(Unit *orig, Unit *un, bool ignore_friend = false);
extern float globQueryShell(QVector st, QVector dir, float radius);

static QVector AutoSafeEntrancePoint(const QVector start, float rsize, const Unit *goal) {
    QVector def = UniverseUtil::SafeEntrancePoint(start, rsize);
    double bdis = (def - RealPosition(goal)).MagnitudeSquared();
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; k += 2) {
                QVector delta(i, j, k);
                delta.Normalize();
                QVector tmp = RealPosition(goal) + delta * (goal->rSize() + rsize);
                tmp = UniverseUtil::SafeEntrancePoint(tmp, rsize);
                double tmag = (tmp - RealPosition(goal)).MagnitudeSquared();
                if (tmag < bdis) {
                    bdis = tmag;
                    def = tmp;
                }
            }
        }
    }
    return def;
}

signed char ComputeAutoGuarantee(Unit *un) {
    Cockpit *cp;
    size_t cpnum = 0;
    if ((cp = _Universe->isPlayerStarship(un))) {
        cpnum = cp - _Universe->AccessCockpit(0);
    } else {
        return Mission::AUTO_ON;
    }
    size_t i = 0;
    for (i = 0; i < active_missions.size(); ++i) {
        if (active_missions[i]->player_num == cpnum && active_missions[i]->player_autopilot != Mission::AUTO_NORMAL) {
            return active_missions[i]->player_autopilot;
        }
    }

    for (i = 0; i < active_missions.size(); ++i) {
        if (active_missions[i]->global_autopilot != Mission::AUTO_NORMAL) {
            return active_missions[i]->global_autopilot;
        }
    }

    return Mission::AUTO_NORMAL;
}

std::string GenerateAutoError(Unit *me, Unit *targ) {
    if (UnitUtil::isAsteroid(targ)) {
        return configuration().graphics.hud.asteroids_near_message;
    }
    if (targ->isPlanet()) {
        return configuration().graphics.hud.planet_near_message;
    }
    if (targ->getRelation(me) < 0) {
        return configuration().graphics.hud.enemy_near_message;
    }
    return configuration().graphics.hud.starship_near_message;
}

///////////////////////////////////////////////

JumpCapable::~JumpCapable() = default;

JumpCapable::JumpCapable() : activeStarSystem(nullptr) {
};

void JumpCapable::ActivateJumpDrive(int destination) {
    Unit *unit = vega_dynamic_cast_ptr<Unit>(this);
    if (((unit->docked & (unit->DOCKED | unit->DOCKED_INSIDE)) == 0) && unit->jump_drive.Installed()) {
        unit->jump_drive.SetDestination(destination);
    }
}

void JumpCapable::AddDestination(const std::string &dest) {
    Unit *unit = vega_dynamic_cast_ptr<Unit>(this);
    unit->pImage->destination.push_back(dest);
}

bool JumpCapable::AutoPilotTo(Unit *un, bool automaticenergyrealloc) {
    std::string tmp;
    return AutoPilotToErrorMessage(un, automaticenergyrealloc, tmp);
}

bool JumpCapable::AutoPilotToErrorMessage(const Unit *target,
        bool ignore_energy_requirements,
        std::string &failuremessage,
        int recursive_level) {
    Unit *unit = vega_dynamic_cast_ptr<Unit>(this);
    const Unit *const_unit = vega_dynamic_const_cast_ptr<const Unit>(this);

    const bool auto_valid = configuration().physics.in_system_jump_or_timeless_auto_pilot;
    if (!auto_valid) {
        static std::string err = "No Insystem Jump";
        failuremessage = err;
        return false;
    }
    if (target->getUnitType() == Vega_UnitType::planet) {
        const Unit *targ = *(target->viewSubUnits());
        if (targ && 0 == targ->graphicOptions.FaceCamera) {
            return AutoPilotToErrorMessage(targ, ignore_energy_requirements, failuremessage, recursive_level);
        }
    }
    if (!unit->ftl_drive.CanConsume()) {
        if (!ignore_energy_requirements) {
            return false;
        }
    }
    signed char Guaranteed = ComputeAutoGuarantee(unit);
    if (Guaranteed == Mission::AUTO_OFF) {
        return false;
    }
    const float autopilot_term_distance = configuration().physics.auto_pilot_termination_distance_flt;
    const float atd_no_enemies = configuration().physics.auto_pilot_termination_distance_no_enemies_flt;
    const float autopilot_no_enemies_multiplier = configuration().physics.auto_pilot_no_enemies_distance_multiplier_flt;
    if (unit->isSubUnit()) {
        static std::string err = "Return To Cockpit for Auto";
        failuremessage = err;
        return false;                            //we can't auto here;
    }
    StarSystem *ss = activeStarSystem;
    if (ss == nullptr) {
        ss = _Universe->activeStarSystem();
    }
    Unit *un = nullptr;
    QVector start(unit->Position());
    QVector end(RealPosition(target));
    float totallength = (start - end).Magnitude();
    bool nanspace = false;
    if (!FINITE(totallength)) {
        nanspace = true;
        start = QVector(100000000.0, 100000000.0, 10000000000000.0);
        totallength = (start - end).Magnitude();
        if (!FINITE(totallength)) {
            end = QVector(200000000.0, 100000000.0, 10000000000000.0);
            totallength = (start - end).Magnitude();
        }
    }
    QVector endne(end);

    float totpercent = 1;
    if (totallength > 1) {
        float apt =
                (target->getUnitType() == Vega_UnitType::planet) ? (autopilot_term_distance + target->rSize()
                        * UniverseUtil::getPlanetRadiusPercent()) : autopilot_term_distance;
        float aptne =
                (target->getUnitType() == Vega_UnitType::planet) ? (atd_no_enemies + target->rSize()
                        * UniverseUtil::getPlanetRadiusPercent()) : atd_no_enemies;
        float percent = (getAutoRSize(unit, unit) + unit->rSize() + target->rSize() + apt) / totallength;
        float percentne = (getAutoRSize(unit, unit) + unit->rSize() + target->rSize() + aptne) / totallength;
        if (percentne > 1) {
            endne = start;
        } else {
            endne = start * percentne + end * (1 - percentne);
        }
        if (percent > 1) {
            end = start;
            totpercent = 0;
        } else {
            totpercent *= (1 - percent);
            end = start * percent + end * (1 - percent);
        }
    }
    bool ok = true;

    const bool teleport_autopilot = configuration().physics.teleport_autopilot;
    bool unsafe = false;
    if ((!teleport_autopilot) && (!nanspace)) {
        if (Guaranteed == Mission::AUTO_NORMAL && unit->cloak.Cloaked()) {
            bool ignore_friendlies = true;
            for (un_iter i = ss->getUnitList().createIterator(); (un = *i) != nullptr; ++i) {
                const bool canflythruplanets = configuration().physics.can_auto_through_planets;
                if ((!(un->getUnitType() == Vega_UnitType::planet
                        && canflythruplanets)) && un->getUnitType() != Vega_UnitType::nebula && (!UnitUtil::isSun(un))) {
                    if (un != this && un != target) {
                        float tdis = (start - un->Position()).Magnitude() - unit->rSize() - un->rSize();
                        float nedis = (end - un->Position()).Magnitude() - unit->rSize() - un->rSize();
                        float trad =
                                getAutoRSize(unit, un, ignore_friendlies) + getAutoRSize(unit, unit, ignore_friendlies);
                        if (tdis <= trad) {
                            failuremessage = GenerateAutoError(unit, un);
                            return false;
                        }
                        if ((nedis < trad * autopilot_no_enemies_multiplier
                                || tdis <= trad * autopilot_no_enemies_multiplier) && un->getRelation(unit) < 0) {
                            unsafe = true;
                            failuremessage = GenerateAutoError(unit, un);
                        }
                        float intersection =
                                globQueryShell(start - un->Position(), end - start, getAutoRSize(unit,
                                        un,
                                        ignore_friendlies)
                                        + un->rSize());
                        if (intersection > 0) {
                            unsafe = true;
                            end = start + (end - start) * intersection;
                            totpercent *= intersection;
                            ok = false;
                            failuremessage = GenerateAutoError(unit, un);
                        }
                    }
                }
            }
        }
    } else if (!nanspace) {
        //just make sure we aren't in an asteroid field
        Unit *un;
        for (un_iter i = ss->getUnitList().createIterator(); (un = *i) != nullptr; ++i) {
            if (UnitUtil::isAsteroid(un)) {
                const float minasteroiddistance = configuration().physics.min_asteroid_distance_flt;
                if (UnitUtil::getDistance(unit, un) < minasteroiddistance) {
                    failuremessage = GenerateAutoError(unit, un);
                    return false;                     //no auto in roid field
                }
            }
        }
    }
    bool nowhere = false;
    if (this != target) {
        if ((end - start).MagnitudeSquared()
                < (static_cast<double>(unit->rSize()) * static_cast<double>(unit->rSize()))) {
            failuremessage = configuration().graphics.hud.already_near_message;
            return false;
        }

        if (unsafe == false && totpercent == 0) {
            end = endne;
        }
        QVector sep(UniverseUtil::SafeEntrancePoint(end, unit->rSize()));
        if ((sep - end).MagnitudeSquared()
                > (16.0 * static_cast<double>(unit->rSize()) * static_cast<double>(unit->rSize()))) {
            //DOn't understand why rsize is so bigsep = AutoSafeEntrancePoint (end,(RealPosition(target)-end).Magnitude()-target->rSize(),target);
            sep = AutoSafeEntrancePoint(end, unit->rSize(), target);
        }
        if ((sep - RealPosition(target)).MagnitudeSquared()
                > (RealPosition(unit) - RealPosition(target)).MagnitudeSquared()) {
            sep = RealPosition(unit);
            nowhere = true;
        }
        const bool auto_turn_towards = configuration().physics.auto_turn_towards;
        if (auto_turn_towards) {
            for (int i = 0; i < 3; ++i) {
                Vector methem(RealPosition(target).Cast() - sep.Cast());
                methem.Normalize();
                Vector p, q, r;
                unit->GetOrientation(p, q, r);
                p = methem.Cross(r);
                float theta = p.Magnitude();
                if (theta * theta > .00001) {
                    p *= (asin(theta) / theta);
                    unit->Rotate(p);
                    unit->GetOrientation(p, q, r);
                }
                if (r.Dot(methem) < 0) {
                    unit->Rotate(p * (PI / theta));
                }
                unit->Velocity = methem * unit->Velocity.Magnitude();
            }
        }
        const std::string& insys_jump_ani = configuration().graphics.in_system_jump_animation;
        if (!insys_jump_ani.empty()) {
            static bool docache = true;
            if (docache) {
                UniverseUtil::cacheAnimation(insys_jump_ani);
                docache = false;
            }
            const float insys_jump_ani_size = configuration().graphics.in_system_jump_animation_size_flt;
            const float insys_jump_ani_growth = configuration().graphics.in_system_jump_animation_growth_flt;
            UniverseUtil::playAnimationGrow(insys_jump_ani, RealPosition(unit),
                    unit->rSize() * insys_jump_ani_size, insys_jump_ani_growth);

            Vector v(unit->GetVelocity());
            v.Normalize();
            Vector p, q, r;
            unit->GetOrientation(p, q, r);
            const float sec = configuration().graphics.in_system_jump_ani_second_ahead_flt;
            UniverseUtil::playAnimationGrow(insys_jump_ani,
                    sep + unit->GetVelocity() * sec + v * unit->rSize(),
                    unit->rSize() * 8,
                    .97);
            UniverseUtil::playAnimationGrow(insys_jump_ani,
                    sep + unit->GetVelocity() * sec + 2 * v * unit->rSize()
                            + r * 4 * unit->rSize(),
                    unit->rSize() * 16,
                    .97);
        }
        const bool warptrail = configuration().graphics.warp_trail;
        if (warptrail && (!nowhere)) {
            const float warptrailtime = configuration().graphics.warp_trail_time_flt;
            AddWarp(unit, RealPosition(unit), warptrailtime);
        }
        if (!nowhere) {
            unit->SetCurPosition(sep);
        }
        Cockpit *cp;
        if ((cp = _Universe->isPlayerStarship(const_unit)) != nullptr) {
            std::string followermessage;
            if (unit->getFlightgroup() != nullptr) {
                Unit *other = nullptr;
                if (recursive_level > 0) {
                    for (un_iter ui = ss->getUnitList().createIterator(); nullptr != (other = *ui); ++ui) {
                        Flightgroup *ff = other->getFlightgroup();
                        bool leadah = (ff == unit->getFlightgroup());
                        if (ff) {
                            if (ff->leader.GetUnit() == this) {
                                leadah = true;
                            }
                        }
                        Order *otherord = other->getAIState();
                        if (otherord) {
                            if (otherord->PursueTarget(unit, leadah)) {
                                other->AutoPilotToErrorMessage(unit,
                                        ignore_energy_requirements,
                                        followermessage,
                                        recursive_level - 1);
                                if (leadah) {
                                    if (other->IsPlayerShip()) {
                                        other->SetPosition(AutoSafeEntrancePoint(unit->LocalPosition(),
                                                other->rSize() * 1.5,
                                                other));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return ok;
}

float JumpCapable::CalculateNearestWarpUnit(float minmultiplier,
        Unit **nearest_unit,
        bool count_negative_warp_units) const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);

    const float smallwarphack = configuration().physics.min_warp_effect_size_flt;
    const float bigwarphack = configuration().physics.max_warp_effect_size_flt;
    //Boundary between multiplier regions 1&2. 2 is "high" mult
    const double warpregion1 = configuration().physics.warp_region1_dbl;
    //Boundary between multiplier regions 0&1 0 is mult=1
    const double warpregion0 = configuration().physics.warp_region0_dbl;
    //Mult at 1-2 boundary
    const double warpcruisemult = configuration().physics.warp_cruise_mult_dbl;
    //degree of curve
    const double curvedegree = configuration().physics.warp_curve_degree_dbl;
    //coefficient so as to agree with above
    const double upcurvek = warpcruisemult / std::pow((warpregion1 - warpregion0), curvedegree);
    //inverse fractional effect of ship vs real big object
    const float def_inv_interdiction = 1.0 / configuration().physics.default_interdiction_flt;
    Unit *planet;
    Unit *testthis = NULL;
    {
        NearestUnitLocator locatespec;
        findObjects(_Universe->activeStarSystem()->collide_map[Unit::UNIT_ONLY],
                unit->location[Unit::UNIT_ONLY],
                &locatespec);
        testthis = locatespec.retval.unit;
    }
    for (un_fiter iter = _Universe->activeStarSystem()->gravitationalUnits().fastIterator();
            (planet = *iter) || testthis;
            ++iter) {
        if (!planet || !planet->Killed()) {
            if (planet == NULL) {
                planet = testthis;
                testthis = NULL;
            }
            if (planet == this) {
                continue;
            }
            float shiphack = 1;
            if (planet->getUnitType() != Vega_UnitType::planet) {
                shiphack = def_inv_interdiction;
                double spec_interdiction = planet->ship_functions.Value(Function::ftl_interdiction);
                if (spec_interdiction != 0 && planet->graphicOptions.specInterdictionOnline != 0
                        && (spec_interdiction > 0 || count_negative_warp_units)) {
                    shiphack = 1 / fabs(spec_interdiction);
                    if (unit->ship_functions.Value(Function::ftl_interdiction) != 0 && unit->graphicOptions.specInterdictionOnline != 0) {
                        //only counters artificial interdiction ... or maybe it cheap ones shouldn't counter expensive ones!? or
                        // expensive ones should counter planets...this is safe now, for gameplay
                        shiphack *= fabs(spec_interdiction);
                    }
                }
            }
            float multipliertemp = 1;
            float minsizeeffect = (planet->rSize() > smallwarphack) ? planet->rSize() : smallwarphack;
            float effectiverad = minsizeeffect * (1.0f + UniverseUtil::getPlanetRadiusPercent()) + unit->rSize();
            if (effectiverad > bigwarphack) {
                effectiverad = bigwarphack;
            }
            QVector dir = unit->Position() - planet->Position();
            double udist = dir.Magnitude();
            float sigdist = UnitUtil::getSignificantDistance(unit, planet);
            if (planet->isPlanet() && udist < (1 << 28)) {
                //If distance is viable as a float approximation and it's an actual celestial body
                udist = sigdist;
            }
            do {
                double dist = udist;
                if (dist < 0) {
                    dist = 0;
                }
                dist *= shiphack;
                if (dist > (effectiverad + warpregion0)) {
                    multipliertemp = std::pow((dist - effectiverad - warpregion0), curvedegree) * upcurvek;
                } else {
                    multipliertemp = 1;
                }
                if (multipliertemp < minmultiplier) {
                    minmultiplier = multipliertemp;
                    *nearest_unit = planet;
                    //eventually use new multiplier to compute
                } else {
                    break;
                }
            } while (0);
            if (!testthis) {
                break;
            } //don't want the ++
        }
    }
    return minmultiplier;
}

float JumpCapable::CourseDeviation(const Vector &OriginalCourse, const Vector &FinalCourse) const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);
    if (unit->MaxAfterburnerSpeed() > .001) {
        return (OriginalCourse - (FinalCourse)).Magnitude() / unit->MaxAfterburnerSpeed();
    } else {
        return (FinalCourse - OriginalCourse).Magnitude();
    }
}

void JumpCapable::DeactivateJumpDrive() {
    Unit *unit = static_cast<Unit *>(this);
    unit->jump_drive.UnsetDestination();
}

const std::vector<std::string> &JumpCapable::GetDestinations() const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);
    return unit->pImage->destination;
}

StarSystem *JumpCapable::getStarSystem() {
    Unit *unit = static_cast<Unit *>(this);
    if (activeStarSystem) {
        return activeStarSystem;
    } else {
        Cockpit *cp = _Universe->isPlayerStarship(unit);
        if (cp) {
            if (cp->activeStarSystem) {
                return cp->activeStarSystem;
            }
        }
    }
    return _Universe->activeStarSystem();
}

const StarSystem *JumpCapable::getStarSystem() const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);
    if (activeStarSystem) {
        return activeStarSystem;
    } else {
        Cockpit *cp = _Universe->isPlayerStarship(unit);
        if (cp) {
            if (cp->activeStarSystem) {
                return cp->activeStarSystem;
            }
        }
    }
    return _Universe->activeStarSystem();
}

Vector JumpCapable::GetWarpRefVelocity() const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);

    //Velocity
    Vector VelocityRef(0, 0, 0);
    {
        const Unit *vr = unit->VelocityReference();
        if (vr) {
            VelocityRef = vr->cumulative_velocity;
        }
    }
    Vector v = unit->Velocity - VelocityRef;
    float len = v.Magnitude();
    if (len > .01) {
        //only get velocity going in DIRECTIOn of cumulative transformation for warp calc...
        v = v * (unit->cumulative_transformation_matrix.getR().Dot(v * (1. / len)));
    }
    return v;
}

Vector JumpCapable::GetWarpVelocity() const {
    const Unit *unit = vega_dynamic_cast_ptr<const Unit>(this);

    if (unit->graphicOptions.WarpFieldStrength == 1.0) {
        // Short circuit, most ships won't be at warp, so it simplifies math a lot
        return unit->cumulative_velocity;
    } else {
        Vector VelocityRef(0, 0, 0);
        {
            const Unit *vr = unit->VelocityReference();
            if (vr) {
                VelocityRef = vr->cumulative_velocity;
            }
        }

        //return(cumulative_velocity*graphicOptions.WarpFieldStrength);
        Vector vel = unit->cumulative_velocity - VelocityRef;
        float speed = vel.Magnitude();
        //return vel*graphicOptions.WarpFieldStrength;
        if (speed > 0) {
            Vector veldir = vel * (1. / speed);
            Vector facing = unit->cumulative_transformation_matrix.getR();
            float ang = facing.Dot(veldir);
            float warpfield = unit->graphicOptions.WarpFieldStrength;
            if (ang < 0) {
                warpfield = 1. / warpfield;
            }
            return facing * (ang * speed * (warpfield - 1.)) + vel + VelocityRef;
        } else {
            return VelocityRef;
        }
    }
}

bool JumpCapable::InCorrectStarSystem(StarSystem *active) {
    return active == activeStarSystem;
}

bool JumpCapable::TransferUnitToSystem(StarSystem *Current) {
    Unit *unit = static_cast<Unit *>(this);
    if (getStarSystem()->RemoveUnit(unit)) {
        unit->RemoveFromSystem();
        unit->Target(NULL);
        Current->AddUnit(unit);

        Cockpit *an_active_cockpit = _Universe->isPlayerStarship(unit);
        if (an_active_cockpit != NULL) {
            an_active_cockpit->activeStarSystem = Current;
            an_active_cockpit->visitSystem(Current->getFileName());
        }
        activeStarSystem = Current;
        return true;
    } else {
        VS_LOG_AND_FLUSH(fatal, "Fatal Error: cannot remove starship from critical system");
        VSExit(-4);
    }
    return false;
}

bool JumpCapable::TransferUnitToSystem(unsigned int whichJumpQueue,
        class StarSystem *&previouslyActiveStarSystem,
        bool DoSightAndSound) {
    return false;
}

