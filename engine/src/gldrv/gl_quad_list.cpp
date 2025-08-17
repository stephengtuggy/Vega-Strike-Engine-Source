/*
 * gl_quad_list.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file. Specifically: Alan Shieh
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
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


#include "gl_globals.h"

#include "src/gfxlib.h"
#include "src/vegastrike.h"
#include "root_generic/vs_globals.h"
#include <cstdio>
#include "src/vs_logging.h"

GFXQuadList::GFXQuadList(GFXBOOL color) : numVertices(0), numQuads(0) {
    data.vertices = nullptr;
    Dirty = GFXFALSE;
    isColor = color;
}

GFXQuadList::~GFXQuadList() {
    if (isColor && data.colors) {
        delete[] data.colors;
        data.colors = nullptr;
    } else if (!isColor && data.vertices) {
        delete[] data.vertices;
        data.vertices = nullptr;
    }
}

void GFXQuadList::Draw() {
    if (!numQuads) {
        return;
    }
    if (isColor) {
        glInterleavedArrays(GL_T2F_C4F_N3F_V3F, sizeof(GFXColorVertex), &data.colors[0]);
    } else {
        glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(GFXVertex), &data.vertices[0]);
    }
    glDrawArrays(GL_QUADS, 0, numQuads * 4);
    if (isColor) {
        GFXColor(1, 1, 1, 1);
    }
}

int GFXQuadList::AddQuad(const GFXVertex *vertices, const GFXColorVertex *color) {
    int cur = numQuads * 4;
    if (cur + 3 >= numVertices) {
        if (!numVertices) {
            numVertices = 16;
            if (!isColor) {
                data.vertices = new GFXVertex[numVertices];
            } else {
                data.colors = new GFXColorVertex[numVertices];
            }
            quadassignments = (int *) malloc(numVertices * sizeof(int) / 4);
            for (int i = 0; i < numVertices / 8; i++) {
                quadassignments[i] = -1;
            }
        } else {
            numVertices *= 2;
            if (!isColor) {
                delete[] data.vertices;
                data.vertices = new GFXVertex[numVertices];
            } else {
                delete[] data.colors;
                data.colors = new GFXColorVertex[numVertices];
            }
            delete[] quadassignments;
            quadassignments = new int[numVertices];
        }
        for (int i = numVertices / 8; i < numVertices / 4; i++) {
            quadassignments[i] = -1;
        }
        Dirty = numVertices / 8;
        quadassignments[numQuads] = numQuads;
        numQuads++;
        if (!isColor && vertices) {
            for (size_t i = 0; i < 4; ++i) {
                data.vertices[i] = vertices[i];
            }
        }
        if (isColor && color) {
            for (size_t i = 0; i < 4; ++i) {
                data.colors[i] = color[i];
            }
        }
        return numQuads - 1;
    }
    for (int i = 0; i < numVertices / 4; i++) {
        if (quadassignments[i] == -1) {
            quadassignments[i] = numQuads;
            const size_t k = quadassignments[i] * 4;
            if (!isColor && vertices) {
                for (size_t j = k; j < k + 4; ++j) {
                    data.vertices[j] = vertices[j];
                }
            }
            if (isColor && color) {
                for (size_t j = k; j < k + 4; ++j) {
                    data.colors[j] = color[j];
                }
            }
            numQuads++;
            Dirty--;
            return i;
        }
    }
    VS_LOG_AND_FLUSH(fatal, "Fatal Error adding quads");
    //should NOT get here!
    return -1;
}

void GFXQuadList::DelQuad(int which) {
    if (quadassignments[which] >= numQuads) {
        VS_LOG(error, "error del");
        return;
    }
    if (which < 0 || which >= numVertices / 4 || quadassignments[which] == -1) {
        return;
    }
    Dirty++;
    for (int i = 0; i < numVertices / 4; i++) {
        if (quadassignments[i] == numQuads - 1) {
            if (isColor) {
                for (int j = 0; j < 4; ++j) {
                    data.colors[quadassignments[which] + j] = data.colors[numQuads + j - 1];
                }
            } else {
                for (int j = 0; j < 4; ++j) {
                    data.vertices[quadassignments[which] + j] = data.vertices[numQuads + j - 1];
                }
            }
            quadassignments[i] = quadassignments[which];
            quadassignments[which] = -1;
            numQuads--;
            return;
        }
    }
    VS_LOG(info, " error deleting engine flame");
}

void GFXQuadList::ModQuad(int which, const GFXVertex *vertices, float alpha) {
    if (which < 0 || which >= numVertices / 4 || quadassignments[which] == -1) {
        return;
    }
    int w = quadassignments[which] * 4;
    if (isColor) {
        data.colors[w + 0].SetVtx(vertices[0]);
        data.colors[w + 1].SetVtx(vertices[1]);
        data.colors[w + 2].SetVtx(vertices[2]);
        data.colors[w + 3].SetVtx(vertices[3]);
        if (alpha != -1) {
            if (alpha == 0) {
                alpha = .01;
            }
            float alp = (data.colors[w].r > data.colors[w].b)
                    ? ((data.colors[w].r > data.colors[w].g) ? data.colors[w].r : data.colors[w].g)
                    : ((data.colors[w].b > data.colors[w].g) ? data.colors[w].b : data.colors[w].g);
            if (alp > .0001) {
                float tmp[4] =
                        {alpha * data.colors[w + 0].r / alp, alpha * data.colors[w + 0].g / alp,
                                alpha * data.colors[w + 0].b / alp, alpha};
                data.colors[w].r = data.colors[w + 0].r;
                data.colors[w].g = data.colors[w + 0].g;
                data.colors[w].b = data.colors[w + 0].b;
                data.colors[w].a = alpha;
            }
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            data.vertices[w + i] = vertices[i];
        }
    }
}

void GFXQuadList::ModQuad(int which, const GFXColorVertex *vertices) {
    if (which < 0 || which >= numVertices / 4 || quadassignments[which] == -1) {
        return;
    }
    if (isColor) {
        for (int i = 0; i < 4; ++i) {
            data.colors[quadassignments[which] * 4 + i] = vertices[i];
        }
    } else {
        data.vertices[(quadassignments[which] * 4) + 0].SetTexCoord(vertices[0].s,
                vertices[0].t).SetNormal(Vector(vertices[0].i,
                vertices[0].j,
                vertices[0].k)).
                SetVertex(
                Vector(vertices[0].x,
                        vertices
                        [
                                0
                        ].y, vertices[0].z));
        data.vertices[(quadassignments[which] * 4) + 1].SetTexCoord(vertices[1].s,
                vertices[1].t).SetNormal(Vector(vertices[1].i,
                vertices[1].j,
                vertices[1].k)).
                SetVertex(
                Vector(vertices[1].x,
                        vertices
                        [
                                1
                        ].y, vertices[1].z));
        data.vertices[(quadassignments[which] * 4) + 2].SetTexCoord(vertices[2].s,
                vertices[2].t).SetNormal(Vector(vertices[2].i,
                vertices[2].j,
                vertices[2].k)).
                SetVertex(
                Vector(vertices[2].x,
                        vertices
                        [
                                2
                        ].y, vertices[2].z));
        data.vertices[(quadassignments[which] * 4) + 3].SetTexCoord(vertices[3].s,
                vertices[3].t).SetNormal(Vector(vertices[3].i,
                vertices[3].j,
                vertices[3].k)).
                SetVertex(
                Vector(vertices[3].x,
                        vertices
                        [
                                3
                        ].y, vertices[3].z));
    }
}
