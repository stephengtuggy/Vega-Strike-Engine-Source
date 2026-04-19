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

VegaRandom::VegaRandom(const uint_fast32_t seed) {
    gen.seed(seed);
}

VegaRandom::VegaRandom(uint_fast32_t init_key[], const size_t key_length) {
    std::seed_seq seq(init_key, init_key + key_length);
    gen.seed(seq);
}

void VegaRandom::InitGenRand(const uint_fast32_t seed) {
    gen.seed(seed);
}

uint_fast32_t VegaRandom::GenRandUInt32() {
    return gen();
}

int_fast32_t VegaRandom::GenRandInt31() {
    return static_cast<int_fast32_t>(GenRandUInt32() >> 1);
}

uint_fast32_t VegaRandom::rand() {
    return GenRandUInt32();
}

double VegaRandom::GenRandReal1() {
    return GenRandUInt32() * (1.0 / kVegaUInt32tMaxAsDouble);
    /* divided by 2^32-1 */
}

double VegaRandom::GenRandReal2() {
    return GenRandUInt32() * (1.0 / kVegaUInt32tMaxAsDoublePlus1);
    /* divided by 2^32 */
}

double VegaRandom::UniformInclusive(const double min, const double max) {
    std::uniform_real_distribution<double> real_dist(min, max);
    return real_dist(gen);
}

double VegaRandom::UniformExclusive(const double min, const double max) {
    std::uniform_real_distribution<double> real_dist(min, max - 1.0 / kVegaUInt32tMaxAsDoublePlus1);
    return real_dist(gen);
}

double VegaRandom::GenRandReal3() {
    return (static_cast<double>(GenRandUInt32()) + 0.5) * (1.0 / kVegaUInt32tMaxAsDoublePlus1);
    /* divided by 2^32 */
}

double VegaRandom::GenRandRes53() {
    const uint_fast32_t a = GenRandUInt32() >> 5;
    const uint_fast32_t b = GenRandUInt32() >> 6;
    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

int_fast32_t VegaRandom::RandomInt32InRange(const int_fast32_t min, const int_fast32_t max) {
    std::uniform_int_distribution<int_fast32_t> int_dist(min, max);
    return int_dist(gen);
}

int_fast32_t VegaRandom::RandomInt32UpTo(const int_fast32_t max) {
    return RandomInt32InRange(0, max);
}

uint_fast32_t VegaRandom::RandomUInt32InRange(const uint_fast32_t min, const uint_fast32_t max) {
    std::uniform_int_distribution<uint_fast32_t> uint_dist(min, max);
    return uint_dist(gen);
}

uint_fast32_t VegaRandom::RandomUInt32UpTo(const uint_fast32_t max) {
    return RandomUInt32InRange(0, max);
}

double VegaRandom::RandomRealInRange(const double min, const double max) {
    return UniformInclusive(min, max);
}

double VegaRandom::RandomDoubleInRange(const double min, const double max) {
    return UniformInclusive(min, max);
}

double VegaRandom::RandomDoubleUpTo(const double max) {
    return RandomDoubleInRange(0.0, max);
}

double VegaRandom::RandomDouble() {
    constexpr double kPrecision = 10000.0;
    std::uniform_int_distribution<uint_fast32_t> int_dist(0,kPrecision);
    const uint_fast32_t random_int = int_dist(gen);
    return static_cast<double>(random_int) / kPrecision;
}

float VegaRandom::RandomFloatInRange(const float min, const float max) {
    return UniformInclusive(min, max);
}

float VegaRandom::RandomFloatUpTo(const float max) {
    return RandomFloatInRange(0.0F, max);
}

float VegaRandom::RandomFloat() {
    std::uniform_real_distribution<float> real_dist(0.0F, 1.0F);
    return real_dist(gen);
}
