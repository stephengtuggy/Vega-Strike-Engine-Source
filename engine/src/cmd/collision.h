/**
 * collision.h
 *
 * Copyright (C) 2020 Roy Falk, Stephen G. Tuggy and other Vega Strike
 * contributors
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


#ifndef COLLISION_H
#define COLLISION_H

#include "gfx/vec.h"
#include "unit_generic.h"

class Unit;
class Cockpit;

/* Rather than implement collision as part of unit as Collidable,
 * I chose to implement a class for the collision itself.
 * The reactToCollision method creates two collision object instances,
 * one for each unit.
 * It is used to reduce duplication of code, as much of the code is simple duplication.
 * Because some variables rely on previous calculations using both units,
 * we do initialization in stages.
*/

class Collision
{
    // Stage 1
    Cockpit* cockpit;
    Unit* unit;
    clsptr unit_type;
    bool is_player_ship;
    const QVector& location;
    const Vector& normal;
    float mass;
    Vector position;
    Vector velocity;
    bool apply_force = false;
    bool deal_damage = false;

    Collision(Unit* unit, const QVector &location, const Vector& normal);
    void shouldApplyForceAndDealDamage(Unit* other_unit);
    void applyForce(double elasticity, float& m2, Vector& v2);
    void dealDamage(Collision other_collision, float factor=0.01);
    bool crashLand(Unit *base);
public:
    static void collide( Unit* unit1,
                           const QVector &location1,
                           const Vector &normal1,
                           Unit* unit2,
                           const QVector &location2,
                           const Vector &normal2,
                           float distance );
};

#endif // COLLISION_H
