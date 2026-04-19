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

#include <random>
#include <cstdint>

constexpr int_fast32_t kVegaIntFast32tMax = std::numeric_limits<int_fast32_t>::max();
constexpr int_least32_t kVegaIntLeast32tMax = std::numeric_limits<int_least32_t>::max();
constexpr unsigned long kVegaIntLeast32tMaxAsULong = std::numeric_limits<int_least32_t>::max();
constexpr uint_fast32_t kVegaUIntFast32tMax = std::numeric_limits<uint_fast32_t>::max();
constexpr uint_least32_t kVegaUIntLeast32tMax = std::numeric_limits<uint_least32_t>::max();
constexpr double kVegaUInt32tMaxAsDouble = std::numeric_limits<uint_least32_t>::max();
constexpr double kVegaUInt32tMaxAsDoublePlus1 = std::numeric_limits<uint_least32_t>::max() + 1.0;


class VegaRandom {
    std::mt19937 gen;

public:

    /// Initializes random number generator with system random_device, where available
    explicit VegaRandom();

    /// Initializes random number generator with a specific seed
    explicit VegaRandom(uint_fast32_t seed);

    /**
     * initializes by an array with array-length
     * init_key is the array for initializing keys
     * key_length is its length
     */
    VegaRandom(uint_fast32_t init_key[], size_t key_length);

    /// Initializes random number generator with a specific seed
    void InitGenRand(uint_fast32_t seed);

    /** generates a random uint_fast32_t on [0,0xffffffff]-interval */
    uint_fast32_t GenRandUInt32();

    /** generates a random int_fast32_t on [0,0x7fffffff]-interval */
    int_fast32_t GenRandInt31();

    /// generates a random uint_fast32_t on [0,0xffffffff]-interval
    uint_fast32_t rand();

    /** generates a random double on [0,1]-real-interval */
    double GenRandReal1();

    /** generates a random double on [0,1)-real-interval */
    double GenRandReal2();

    /// generates a random double between min and max inclusive
    double UniformInclusive(double min, double max);

    /// generates a random double between min and max exclusive
    double UniformExclusive(double min, double max);

    /** generates a random double on (0,1)-real-interval */
    double GenRandReal3();

    /** generates a random double on [0,1) with 53-bit resolution */
    double GenRandRes53();

    /// generates a random int_fast32_t between min and max
    int_fast32_t RandomInt32InRange(int_fast32_t min, int_fast32_t max);

    /// generates a random int_fast32_t between 0 and max
    int_fast32_t RandomInt32UpTo(int_fast32_t max);

    /// generates a random uint_fast32_t between min and max
    uint_fast32_t RandomUInt32InRange(uint_fast32_t min, uint_fast32_t max);

    /// generates a random uint_fast32_t between 0 and max
    uint_fast32_t RandomUInt32UpTo(uint_fast32_t max);

    /// generates a random double between min and max, inclusive
    double RandomRealInRange(double min, double max);

    /// generates a random double between min and max, inclusive
    double RandomDoubleInRange(double min, double max);

    /// generates a random double between 0.0 and max, inclusive
    double RandomDoubleUpTo(double max);

    /// generates a random double between 0.0 and 1.0, at 1/10,000th increments
    double RandomDouble();

    /// generates a random float between min and max, inclusive
    float RandomFloatInRange(float min, float max);

    /// generates a random float between 0.0F and max, inclusive
    float RandomFloatUpTo(float max);

    /// generates a random float between 0.0F and 1.0F
    float RandomFloat();
};

extern VegaRandom vega_random;

#endif //VEGA_STRIKE_VEGA_RANDOM_H
