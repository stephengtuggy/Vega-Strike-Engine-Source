///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for random generators.
 *  \file		IceRandom.cpp
 *  \author		Pierre Terdiman
 *  \date		August, 9, 2001
 *
 *  Copyright (C) 1998-2025 Pierre Terdiman, Stephen G. Tuggy, Benjamen R. Meyer
 *  Public Domain
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "collide2/Opcode.h"

using namespace Opcode;

void SRand(uint32_t seed) {
    srand(seed);
}

uint32_t Rand() {
    return rand();
}

static BasicRandom gRandomGenerator(42);

uint32_t GetRandomIndex(uint32_t max_index) {
    // We don't use rand() since it's limited to RAND_MAX
    uint32_t Index = gRandomGenerator.Randomize();
    return Index % max_index;
}


