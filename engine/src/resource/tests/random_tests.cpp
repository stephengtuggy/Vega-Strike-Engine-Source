/*
 * random_tests.cpp
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

#include <gtest/gtest.h>

#include "resource/random_utils.h"
#include "root_generic/vega_random.h"

constexpr int kNumRepetitions = 100;

TEST(random_utils, randomInt) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        int random_int = randomInt(10);
        EXPECT_GE(random_int, 0);
        EXPECT_LE(random_int, 10);
    }
}

TEST(random_utils, randomDouble) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        int random_double = randomDouble();
        EXPECT_GE(random_double, 0.0);
        EXPECT_LE(random_double, 1.0);
    }
}

TEST(VegaRandom, GenRandUInt32) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        uint_fast32_t random_uint32 = vega_random.GenRandUInt32();
        EXPECT_GT(random_uint32, 0);
        EXPECT_LT(random_uint32, kVegaUIntLeast32tMax);
    }
}

TEST(VegaRandom, RandomDouble) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.RandomDouble();
        EXPECT_GE(random_double, 0.0);
        EXPECT_LE(random_double, 1.0);
    }
}

TEST(VegaRandom, RandomDoubleUpTo) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.RandomDoubleUpTo(10.0);
        EXPECT_GE(random_double, 0.0);
        EXPECT_LE(random_double, 10.0);
    }
}

TEST(VegaRandom, RandomDoubleInRange) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.RandomDoubleInRange(-10.0, 10.0);
        EXPECT_GE(random_double, -10.0);
        EXPECT_LE(random_double, 10.0);
    }
}

TEST(VegaRandom, RandomRealInRange) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.RandomRealInRange(-10.0, 10.0);
        EXPECT_GE(random_double, -10.0);
        EXPECT_LE(random_double, 10.0);
    }
}

TEST(VegaRandom, RandomFloatInRange) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        float random_float = vega_random.RandomFloatInRange(-10.0F, 10.0F);
        EXPECT_GE(random_float, -10.0F);
        EXPECT_LE(random_float, 10.0F);
    }
}

TEST(VegaRandom, RandomFloatUpTo) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        float random_float = vega_random.RandomFloatUpTo(10.0F);
        EXPECT_GE(random_float, 0.0F);
        EXPECT_LE(random_float, 10.0F);
    }
}

TEST(VegaRandom, RandomFloat) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        float random_float = vega_random.RandomFloat();
        EXPECT_GE(random_float, 0.0F);
        EXPECT_LE(random_float, 1.0F);
    }
}

TEST(VegaRandom, RandomInt32InRange) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        int random_int32 = vega_random.RandomInt32InRange(-1000, 1000);
        EXPECT_GE(random_int32, -1000);
        EXPECT_LE(random_int32, 1000);
    }
}

TEST(VegaRandom, RandomInt32UpTo) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        int random_int32 = vega_random.RandomInt32UpTo(1000);
        EXPECT_GE(random_int32, 0);
        EXPECT_LE(random_int32, 1000);
    }
}

TEST(VegaRandom, RandomUInt32InRange) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        uint_fast32_t random_uint32 = vega_random.RandomUInt32InRange( 1000, 2000);
        EXPECT_GE(random_uint32, 1000);
        EXPECT_LE(random_uint32, 2000);
    }
}

TEST(VegaRandom, RandomUInt32UpTo) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        uint_fast32_t random_uint32 = vega_random.RandomUInt32UpTo(1000);
        EXPECT_LE(random_uint32, 1000);
    }
}

TEST(VegaRandom, GenRandRes53) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.GenRandRes53();
        EXPECT_GT(random_double, 0.0);
        EXPECT_LE(random_double, 1.0);
    }
}

TEST(VegaRandom, GenRandReal1) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.GenRandReal1();
        EXPECT_GT(random_double, 0.0);
        EXPECT_LT(random_double, 1.0);
    }
}

TEST(VegaRandom, GenRandReal2) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.GenRandReal2();
        EXPECT_GT(random_double, 0.0);
        EXPECT_LE(random_double, 1.0);
    }
}

TEST(VegaRandom, GenRandReal3) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.GenRandReal3();
        EXPECT_GE(random_double, 0.0);
        EXPECT_LE(random_double, 1.0);
    }
}

TEST(VegaRandom, UniformInclusive) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.UniformInclusive(-10.0, 10.0);
        EXPECT_GE(random_double, -10.0);
        EXPECT_LE(random_double, 10.0);
    }
}

TEST(VegaRandom, UniformExclusive) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        double random_double = vega_random.UniformExclusive(-10.0, 10.0);
        EXPECT_GT(random_double, -10.0);
        EXPECT_LT(random_double, 10.0);
    }
}

TEST(VegaRandom, GenRandInt31) {
    for (int i = 0; i < kNumRepetitions; ++i) {
        int_fast32_t random_int32 = vega_random.GenRandInt31();
        EXPECT_GT(random_int32, 0);
        EXPECT_LT(random_int32, kVegaIntLeast32tMax);
    }
}
