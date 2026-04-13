/*
 * vega_random.cpp
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

#include "root_generic/vega_random.h"

VegaRandom::VegaRandom() {
    std::random_device rd;
    gen.seed(rd());
}

VegaRandom::VegaRandom(const uint32_t seed) {
    gen.seed(seed);
}

VegaRandom::VegaRandom(uint32_t init_key[], const size_t key_length) {
    std::seed_seq seq(init_key, init_key + key_length);
    gen.seed(seq);
}

void VegaRandom::init_gen_rand(const uint32_t seed) {
    gen.seed(seed);
}
