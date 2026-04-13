/*
 *  A C-program for MT19937, with initialization improved 2002/1/26.
 *  Coded by Takuji Nishimura and Makoto Matsumoto.
 *  Before using, initialize the state by using init_gen_rand(seed)
 *  or init_by_array(init_key, key_length).
 *  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 *  Copyright (C) 2023 Benjamen R. Meyer
 *  Copyright (C) 2026 Stephen G. Tuggy -- Overhauled to use c++11 random number generation
 *  All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. The names of its contributors may not be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Any feedback is very welcome.
 *  http://www.math.keio.ac.jp/matumoto/emt.html
 *  email: matumoto@math.keio.ac.jp
 */
#ifndef VS_RANDOM_H
#define VS_RANDOM_H

#define VS_RAND_MAX 0x7fffffffUL
#include <random>
#include <cstdint>

class VSRandom {
    std::mt19937 gen;

public:

    explicit VSRandom() {
        std::random_device rd;
        gen.seed(rd());
    }

    /* initializes random number generator with a specific seed */
    explicit VSRandom(const uint_fast32_t s) {
        init_gen_rand(s);
    }

    void init_gen_rand(const uint_fast32_t s) {
        gen.seed(s);
    }

    /*
     * initialize by an array with array-length
     * init_key is the array for initializing keys
     * key_length is its length
     */
    VSRandom(uint_fast32_t init_key[], const size_t key_length) {
        std::seed_seq seq(init_key, init_key + key_length);
        gen.seed(seq);
    }

    /* generates a random number on [0,0xffffffff]-interval */
    uint_fast32_t gen_rand_uint32() {
        return gen();
    }

    /* generates a random number on [0,0x7fffffff]-interval */
    int_fast32_t gen_rand_int31() {
        return static_cast<int_fast32_t>(gen_rand_uint32() >> 1);
    }

    uint_fast32_t rand() {
        return gen_rand_uint32();
    }

    /* generates a random number on [0,1]-real-interval */
    double gen_rand_real1() {
        std::uniform_real_distribution<double> real_dist(0.0, 4294967295.0);
        return real_dist(gen);
        // return gen_rand_uint32() * (1.0 / 4294967295.0);
        /* divided by 2^32-1 */
    }

    /* generates a random number on [0,1)-real-interval */
    double gen_rand_real2() {
        std::uniform_real_distribution<double> real_dist(0.0, 4294967296.0);
        return real_dist(gen);
        // return gen_rand_uint32() * (1.0 / 4294967296.0);
        /* divided by 2^32 */
    }

    double uniformInc(const double min, const double max) {
        std::uniform_real_distribution<double> real_dist(min, max);
        return real_dist(gen);
    }

    double uniformExc(const double min, const double max) {
        std::uniform_real_distribution<double> real_dist(min, max - 1.0 / 4294967296.0);
        return real_dist(gen);
    }

    /* generates a random number on (0,1)-real-interval */
    double gen_rand_real3() {
        return (static_cast<double>(gen_rand_uint32()) + 0.5) * (1.0 / 4294967296.0);
        /* divided by 2^32 */
    }

    /* generates a random number on [0,1) with 53-bit resolution*/
    double gen_rand_res53() {
        const uint_fast32_t a = gen_rand_uint32() >> 5;
        const uint_fast32_t b = gen_rand_uint32() >> 6;
        return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
    }
/* The above real-number versions are due to Isaku Wada, 2002/01/09 added */

    /* The following methods were added by Stephen G. Tuggy 2026-04-11 */

    int_fast32_t random_int32_in_range(const int_fast32_t min, const int_fast32_t max) {
        std::uniform_int_distribution<int_fast32_t> int_dist(min, max);
        return int_dist(gen);
    }

    int_fast32_t random_int32_up_to(const int_fast32_t max) {
        return random_int32_in_range(0, max);
    }

    uint_fast32_t random_uint32_in_range(const uint_fast32_t min, const uint_fast32_t max) {
        std::uniform_int_distribution<uint_fast32_t> uint_dist(min, max);
        return uint_dist(gen);
    }

    uint_fast32_t random_uint32_up_to(const uint_fast32_t max) {
        return random_uint32_in_range(0, max);
    }

    double randomRealInRange(const double min, const double max) {
        return uniformInc(min, max);
    }

    double randomDoubleInRange(const double min, const double max) {
        return uniformInc(min, max);
    }

    double randomDoubleUpTo(const double max) {
        return randomDoubleInRange(0, max);
    }

    double randomDouble() {
        // constexpr int kPrecision = 10000;
        // std::uniform_int_distribution<std::mt19937::result_type> int_dist(0,kPrecision);
        // const unsigned int random_int = int_dist(gen);
        // return static_cast<double>(random_int) / kPrecision;
        std::uniform_real_distribution<double> real_dist(0.0, 1.0);
        return real_dist(gen);
    }

    float randomFloatInRange(const float min, const float max) {
        return uniformInc(min, max);
    }

    float randomFloatUpTo(const float max) {
        return randomFloatInRange(0, max);
    }

    float randomFloat() {
        std::uniform_real_distribution<float> real_dist(0.0F, 1.0F);
        return real_dist(gen);
    }
};

extern VSRandom vs_random;

#endif // VS_RANDOM_H
