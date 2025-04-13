/**
 * mesh_poly.cpp
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2025 Stephen G. Tuggy, and other Vega Strike Contributors
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


#include "gfx_generic/mesh.h"
#include "gfx/aux_texture.h"
#if !defined (_WIN32) && !defined (__CYGWIN__) && !(defined (__APPLE__) || defined (MACOSX )) && !defined (BSD) && !defined(__HAIKU__)
#include <values.h>
#endif
#include <float.h>
#include <assert.h>
#define PBEHIND (-1)
#define PFRONT (1)
#define PUNK (0)

#define WHICHSID(v) ( ( (v.x*a+v.y*b+v.z*c+d) > 0 )*2-1 )

static int whichside(GFXVertex *t, int numvertex, float a, float b, float c, float d) {
    int count = PUNK;
    for (int i = 0; i < numvertex; i++) {
        count += WHICHSID(t[i]);
    }
    if (count == PUNK) {
        count = (rand() % 2) * 2 - 1;
    }
    return ((count > 0) * 2) - 1;
}

void updateMax(Vector &mn, Vector &mx, const GFXVertex &ver);

void Mesh::Fork(std::shared_ptr<Mesh> &x, std::shared_ptr<Mesh> &y, float a, float b, float c, float d) {
    if (!originals.empty() && originals.front().get() != this) {
        originals.front()->Fork(x, y, a, b, c, d);
        return;
    }
    int numtris, numquads;
    GFXVertex *Orig;
    vlist->GetPolys(&Orig, &numquads, &numtris);
    numquads -= numtris;
    int numtqx[2] = {0, 0};
    int numtqy[2] = {0, 0};

    GFXVertex *X = new GFXVertex[numquads * 4 + numtris * 3];
    GFXVertex *xnow = X;
    GFXVertex *Y = new GFXVertex[numquads * 4 + numtris * 3];
    GFXVertex *ynow = Y;
    Vector xmax, xmin, ymax, ymin;
    xmax = ymax = Vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    xmin = ymin = Vector(FLT_MAX, FLT_MAX, FLT_MAX);

    int i, j;
    int inc = 3;
    int offset = 0;
    int last = numtris;
    mesh_polygon tmppolygon;
    for (int l = 0; l < 2; l++) {
        for (i = 0; i < last; i++) {
            if ((!(numtqy[l] < last / 3
                    && numtqx[l] > 2 * last / 3))
                    && ((numtqx[l] < last / 3
                            && numtqy[l] > 2 * last / 3)
                            || whichside(&Orig[offset + i * inc], inc, a, b, c, d) == PFRONT)) {
                numtqx[l]++;
                for (j = 0; j < inc; j++) {
                    memcpy(xnow, &Orig[offset + i * inc + j], sizeof(GFXVertex));
                    updateMax(xmin, xmax, *xnow);
                    xnow++;
                }
            } else {
                numtqy[l]++;
                for (j = 0; j < inc; j++) {
                    memcpy(ynow, &Orig[offset + i * inc + j], sizeof(GFXVertex));
                    updateMax(ymin, ymax, *ynow);
                    ynow++;
                }
            }
        }
        numtqx[l] *= inc;
        numtqy[l] *= inc;
        inc = 4;
        offset = numtris * 3;
        last = numquads;
    }
    free(Orig);
    enum POLYTYPE polytypes[2] = {GFXTRI, GFXQUAD};
    if ((!(numtqx[0] || numtqx[1])) || (!(numtqy[0] || numtqy[1]))) {
        x = y = nullptr;
        delete[] X;
        delete[] Y;
        return;
    }
    x = Mesh::Create();
    x->setLighting(getLighting());
    x->setEnvMap(getEnvMap());
    x->forceCullFace(GFXFALSE);

    y = Mesh::Create();
    y->setLighting(getLighting());
    y->setEnvMap(getEnvMap());

    y->forceCullFace(GFXFALSE);
    x->forcelogos.clear();
    x->squadlogos.clear();
    x->setLighting(getLighting());
    x->setEnvMap(getEnvMap());
    x->blendSrc = y->blendSrc = blendSrc;
    x->blendDst = y->blendDst = blendDst;
    {
        for (auto & i2 : Decal) {
            if (i2) {
                x->Decal.push_back(i2->Clone());
            }
        }
    }

    y->squadlogos.clear();
    y->forcelogos.clear();
    y->setLighting(getLighting());
    y->setEnvMap(getEnvMap());
    {
        for (auto & i2 : Decal) {
            if (i2) {
                y->Decal.push_back(i2->Clone());
            }
        }
    }
    if (numtqx[0] && numtqx[1]) {
        x->vlist = new GFXVertexList(polytypes, numtqx[0] + numtqx[1], X, 2, numtqx, true);
    } else {
        int exist = 0;
        if (numtqx[1]) {
            exist = 1;
        }
        assert(numtqx[0] || numtqx[1]);
        x->vlist = new GFXVertexList(&polytypes[exist], numtqx[exist], X, 1, &numtqx[exist], true, nullptr);
    }
    if (numtqy[0] || numtqy[1]) {
        y->vlist = new GFXVertexList(polytypes, numtqy[0] + numtqy[1], Y, 2, numtqy, true);
    } else {
        int exis = 0;
        if (numtqy[1]) {
            exis = 1;
        }
        assert(numtqx[0] || numtqx[1]);
        y->vlist = new GFXVertexList(&polytypes[exis], numtqy[exis], Y, 1, &numtqy[exis], true, nullptr);
    }
    x->local_pos = Vector(.5 * (xmin + xmax));
    y->local_pos = Vector(.5 * (ymin + ymax));
    x->radialSize = .5 * (xmax - xmin).Magnitude();
    y->radialSize = .5 * (ymax - ymin).Magnitude();
    x->mn = xmin;
    x->mx = xmax;
    y->mn = ymin;
    y->mx = ymax;
    x->originals.push_back(x);
    x->forceCullFace(GFXFALSE);

    y->originals.push_back(y);
    y->forceCullFace(GFXFALSE);
    x->draw_queue = new vector<MeshDrawContext>[NUM_ZBUF_SEQ + 1];
    y->draw_queue = new vector<MeshDrawContext>[NUM_ZBUF_SEQ + 1];
    x->forcelogos.clear();
    x->squadlogos.clear();

    delete[] X;
    delete[] Y;
}

void Mesh::GetPolys(vector<mesh_polygon> &polys) {
    int numtris;
    int numquads;
    if (!originals.empty() && originals.front().get() != this) {
        originals.front()->GetPolys(polys);
        return;
    }
    GFXVertex *tmpres;
    Vector vv;
    vlist->GetPolys(&tmpres, &numquads, &numtris);
    numquads -= numtris;
    int i;
    int inc = 3;
    int offset = 0;
    int last = numtris;
    mesh_polygon tmppolygon;
    // Unroll this loop a bit to remove conditional
    for (i = 0; i < last; i++) {
        polys.push_back(tmppolygon);
        for (int j = 0; j < 3; j++, polys.back().v.push_back(vv)) {
            vv.i = tmpres[offset + i * inc + j].x;                 //+local_pos.i;
            vv.j = tmpres[offset + i * inc + j].y;                 //+local_pos.j;
            vv.k = tmpres[offset + i * inc + j].z;                 //+local_pos.k;
        }
    }
    inc = 4;
    offset = numtris * 3;
    last = numquads;
    for (i = 0; i < last; i++) {
        polys.push_back(tmppolygon);
        for (int j = 1; j < 4; j++, polys.back().v.push_back(vv)) {
            vv.i = tmpres[offset + i * inc + j].x;                     //+local_pos.i;
            vv.j = tmpres[offset + i * inc + j].y;                     //+local_pos.j;
            vv.k = tmpres[offset + i * inc + j].z;                     //+local_pos.k;
        }
    }
    free(tmpres);
}
