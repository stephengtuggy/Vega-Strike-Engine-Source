/*
 * Copyright (C) 2001-2022 Daniel Horn, Alan Shieh, pyramid3d,
 * Stephen G. Tuggy, and other Vega Strike contributors.
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
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */


#include "gl_globals.h"

#include "gfxlib.h"
#include "vegastrike.h"
#include "vs_logging.h"

const size_t INITIAL_CAPACITY = 16;

template<class T> static void VegaReallocArray(T* &ptr, const size_t newSize, const size_t oldSize, const T defaultValue) {
    T* tmp = new T[newSize];
    const size_t minSize = std::min(newSize, oldSize);
    size_t i = 0;
    for (; i < minSize; ++i) {
        tmp[i] = ptr[i];
    }
    for (; i < newSize; ++i) {
        tmp[i] = defaultValue;
    }
    delete[] ptr;
    ptr = tmp;
}

GFXQuadList::GFXQuadList(GFXBOOL color) : numVertices(0), numQuads(0) {
    data.vertices = nullptr;
    Dirty = GFXFALSE;
    isColor = color;
}

GFXQuadList::~GFXQuadList() {
    if (isColor && data.colors) {
        delete[] data.colors;
    } else if (!isColor && data.vertices) {
        delete[] data.vertices;
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
    size_t cur = numQuads * 4;
    if (cur + 3 >= numVertices) {
        if (!numVertices) {
            numVertices = INITIAL_CAPACITY;
            if (!isColor) {
                data.vertices = new GFXVertex[numVertices];
            } else {
                data.colors = new GFXColorVertex[numVertices];
            }
            quadassignments = new int[numVertices / 4];
            for (size_t i = 0; i < numVertices / 4; ++i) {
                quadassignments[i] = -1;
            }
        } else {
            size_t oldNumVertices = numVertices;
            numVertices *= 2;
            if (!isColor) {
                VegaReallocArray(data.vertices, numVertices, oldNumVertices, GFXVertex());
            } else {
                VegaReallocArray(data.colors, numVertices, oldNumVertices, GFXColorVertex());
            }
            VegaReallocArray(quadassignments, numVertices / 4, oldNumVertices / 4, -1);
        }
        Dirty = numVertices / 4 / 2;
        quadassignments[numQuads] = numQuads;
        numQuads++;
        if (!isColor && vertices) {
            for (size_t n = 0; n < 4; ++n) {
                data.vertices[cur + n] = vertices[n];
            }
        }
        if (isColor && color) {
            for (size_t n = 0; n < 4; ++n) {
                data.colors[cur + n] = color[n];
            }
        }
        return numQuads - 1;
    }
    for (size_t i = 0; i < numVertices / 4; i++) {
        if (quadassignments[i] == -1) {
            quadassignments[i] = numQuads;
            if (!isColor && vertices) {
                for (size_t n = 0; n < 4; ++n) {
                    data.vertices[quadassignments[i] * 4 + n] = vertices[n];
                }
            }
            if (isColor && color) {
                for (size_t n = 0; n < 4; ++n) {
                    data.colors[quadassignments[i] * 4 + n] = color[n];
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
    for (size_t i = 0; i < numVertices / 4; ++i) {
        if (quadassignments[i] == numQuads - 1) {
            if (isColor) {
                for (size_t j = 0; j < 4; ++j) {
                    data.colors[quadassignments[which] * 4 + j] = data.colors[(numQuads - 1) * 4 + j];
                }
            } else {
                for (size_t j = 0; j < 4; ++j) {
                    data.vertices[quadassignments[which] * 4 + j] = data.vertices[(numQuads - 1) * 4 + j];
                }
            }
            quadassignments[i] = quadassignments[which];
            quadassignments[which] = -1;
            numQuads--;
            return;
        }
    }
    VS_LOG(error, " error deleting engine flame");
}

void GFXQuadList::ModQuad(int which, const GFXVertex *vertices, float alpha) {
    if (which < 0 || which >= numVertices / 4 || quadassignments[which] == -1) {
        return;
    }
    if (isColor) {
        int w = quadassignments[which] * 4;

        for (size_t n = 0; n < 4; ++n) {
            data.colors[w + n].SetVtx(vertices[n]);
        }
        if (alpha != -1) {
            if (alpha == 0) {
                alpha = .01;
            }
            float alp = std::max({ data.colors[w].r, data.colors[w].g, data.colors[w].b });
            if (alp > .0001) {
                float tmpR = alpha * data.colors[w + 0].r / alp;
                float tmpG = alpha * data.colors[w + 0].g / alp;
                float tmpB = alpha * data.colors[w + 0].b / alp;
                float tmpAlpha = alpha;
                for (size_t n = 0; n < 4; ++n) {
                    data.colors[w + n].r = tmpR;
                    data.colors[w + n].g = tmpG;
                    data.colors[w + n].b = tmpB;
                    data.colors[w + n].a = tmpAlpha;
                }
            }
        }
    } else {
        for (size_t n = 0; n < 4; ++n) {
            data.vertices[quadassignments[which] * 4 + n] = vertices[n];
        }
    }
}

void GFXQuadList::ModQuad(int which, const GFXColorVertex *vertices) {
    if (which < 0 || which >= numVertices / 4 || quadassignments[which] == -1) {
        return;
    }
    if (isColor) {
        for (size_t n = 0; n < 4; ++n) {
            data.vertices[(quadassignments[which] * 4) + n] = GFXVertex(vertices[n].x, vertices[n].y, vertices[n].z, vertices[n].i, vertices[n].j, vertices[n].k, vertices[n].s, vertices[n].t);
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
