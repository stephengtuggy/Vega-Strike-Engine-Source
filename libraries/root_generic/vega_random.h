/*
 * vega_random.h
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2026 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
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

#ifndef VEGA_STRIKE_VEGA_RANDOM_H
#define VEGA_STRIKE_VEGA_RANDOM_H

#define VS_RAND_MAX 0x7fffffffUL
#include <random>
#include <cstdint>


class VegaRandom {
    std::mt19937 gen;

public:

    /// Initializes random number generator with system random_device, where available
    explicit VegaRandom();

    explicit VegaRandom(const uint32_t seed);

    explicit VegaRandom(uint32_t init_key[], const size_t key_length);

    void init_gen_rand(const uint32_t seed);
};

#endif //VEGA_STRIKE_VEGA_RANDOM_H
