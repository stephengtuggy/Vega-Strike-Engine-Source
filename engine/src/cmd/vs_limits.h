/**
 * vs_limits.h
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors
 * Copyright (C) 2022 Stephen G. Tuggy
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIMITS_H
#define LIMITS_H

#include "gfx/vec.h"

class Limits {
public:
    // Init overrides default values of 0 with the following values
//max ypr--both pos/neg are symmetrical
    float yaw = 2.55;
    float pitch = 2.55;
    float roll = 2.55;
//side-side engine thrust max
    float lateral = 2;
//vertical engine thrust max
    float vertical = 8;
//forward engine thrust max
    float forward = 2;
//reverse engine thrust max
    float retro = 2;
//after burner acceleration max
    float afterburn = 5;
//the vector denoting the "front" of the turret cone!
    // Again, an inconsistency between constructor and Init(). Chose Init
    // value as it comes later
    Vector structurelimits = Vector(0, 0, 1);
//the minimum dot that the current heading can have with the structurelimit
    float limitmin = -1;
};

#endif // LIMITS_H
