/**
 * asteroid.h
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2025 Stephen G. Tuggy, Benjamen R. Meyer, and other Vega Strike Contributors
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
#ifndef VEGA_STRIKE_ENGINE_CMD_ASTEROID_H
#define VEGA_STRIKE_ENGINE_CMD_ASTEROID_H

#include "gfx_generic/quaternion.h"
#include "gfx_generic/vec.h"
#include "gfx_generic/matrix.h"
#include "cmd/script/flightgroup.h"
#include "cmd/collection.h"
#include "cmd/unit_generic.h"

class Asteroid : public Unit {
private:
    unsigned int asteroid_physics_offset{};

public:
    Asteroid(const char *filename, int faction, Flightgroup *fg = nullptr, int fg_snumber = 0, float difficulty = .01);

    enum Vega_UnitType isUnit() const override {
        return Vega_UnitType::asteroid;
    }

    Vega_UnitType getUnitType() const override {
        return Vega_UnitType::asteroid;
    }

private:

    Asteroid(std::deque<std::shared_ptr<Mesh>> m, bool b, int i) : Unit(m, b, i) {
    }

public:
    /// default constructor forbidden
    Asteroid() = delete;

    /// copy constructor forbidden
    Asteroid(const Asteroid &) = delete;

    /// assignment operator forbidden
    Asteroid &operator=(const Asteroid &) = delete;
};

#endif //VEGA_STRIKE_ENGINE_CMD_ASTEROID_H
