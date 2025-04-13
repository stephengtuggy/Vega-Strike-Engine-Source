/**
 * ring.h
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors
 * Copyright (C) 2022-2023 Stephen G. Tuggy, Benjamen R. Meyer
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
#ifndef VEGA_STRIKE_ENGINE_GFX_RING_H
#define VEGA_STRIKE_ENGINE_GFX_RING_H

#include "gfx_generic/mesh.h"
#include "src/vegastrike.h"

class RingMesh : public Mesh {
    virtual std::shared_ptr<Mesh> AllocNewMeshesEachInSizeofMeshSpace(int num) {
        return new RingMesh[num];
    }

    virtual void InitRing(float iradius,
            float oradius,
            int slices,
            const char *texture,
            const QVector &r,
            const QVector &s,
            int texwrapx = 1,
            int texwrapy = 1,
            const BLENDFUNC a = ONE,
            const BLENDFUNC b = ZERO,
            bool envMap = false,
            float theta_min = 0.0,
            float theta_max = 2 * M_PI,
            FILTER mipmap = MIPMAP);

public:
    RingMesh() : Mesh() {
    }

    RingMesh(float iradius,
            float oradius,
            int slices,
            const char *texture,
            const QVector &r,
            const QVector &s,
            int texwrapx = 1,
            int texwrapy = 1,
            const BLENDFUNC a = SRCALPHA,
            const BLENDFUNC b = INVSRCALPHA,
            bool envMap = false,
            float theta_min = 0.0,
            float theta_max = 2 * M_PI,
            FILTER mipmap = TRILINEAR) {
        InitRing(iradius,
                oradius,
                slices,
                texture,
                r,
                s,
                texwrapx,
                texwrapy,
                a,
                b,
                envMap,
                theta_min,
                theta_max,
                mipmap);
    }

    virtual float clipRadialSize() const;
};

#endif //VEGA_STRIKE_ENGINE_GFX_RING_H
