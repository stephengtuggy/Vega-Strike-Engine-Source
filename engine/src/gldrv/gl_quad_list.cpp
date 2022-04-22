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

//template<class T> static void VegaReallocArray(T* &ptr, const size_t newSize, const size_t oldSize, const T defaultValue) {
//    T* tmp = new T[newSize];
//    const size_t minSize = std::min(newSize, oldSize);
//    size_t i = 0;
//    for (; i < minSize; ++i) {
//        tmp[i] = ptr[i];
//    }
//    for (; i < newSize; ++i) {
//        tmp[i] = defaultValue;
//    }
//    delete[] ptr;
//    ptr = tmp;
//}

GFXQuadList::GFXQuadList(GFXBOOL color) : numVertices(0), numQuads(0) {
//    data.vertices = nullptr;
    Dirty = GFXFALSE;
    isColor = color;
}

GFXQuadList::~GFXQuadList() {
    if (isColor) {
        data.colorVertices.clear();
    } else {
        data.vertices.clear();
    }
}

void GFXQuadList::Draw() {
    if (numQuads == 0) {
        return;
    }
    if (isColor) {
        glInterleavedArrays(GL_T2F_C4F_N3F_V3F, sizeof(GFXColorVertex), data.colorVertices.data());
    } else {
        glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(GFXVertex), data.vertices.data());
    }
    glDrawArrays(GL_QUADS, 0, numQuads * 4);
    if (isColor) {
        GFXColor(1, 1, 1, 1);
    }
}

int GFXQuadList::AddQuad(const GFXVertex *vertices, const GFXColorVertex *color) {
    size_t cur = numQuads * 4;
    if (cur + 3 >= numVertices) {
        if (numVertices == 0) {
            numVertices = INITIAL_CAPACITY;
            if (!isColor) {
                data.vertices.reserve(numVertices);
                for (size_t i = 0; i < numVertices; ++i) {
                    data.vertices.emplace_back();
                }
            } else {
                data.colorVertices.reserve(numVertices);
                for (size_t i = 0; i < numVertices; ++i) {
                    data.colorVertices.emplace_back();
                }
            }
            quadAssignments.reserve(numVertices / 4);
            for (size_t i = 0; i < numVertices / 4; ++i) {
                quadAssignments.emplace_back(-1);
            }
        } else {
            size_t oldNumVertices = numVertices;
            numVertices *= 2;
            if (!isColor) {
                data.vertices.reserve(numVertices);
                for (size_t i = oldNumVertices; i < numVertices; ++i) {
                    data.vertices.emplace_back();
                }
            } else {
                data.colorVertices.reserve(numVertices);
                for (size_t i = oldNumVertices; i < numVertices; ++i) {
                    data.colorVertices.emplace_back();
                }
            }
            quadAssignments.reserve(numVertices / 4);
            for (size_t i = oldNumVertices / 4; i < numVertices / 4; ++i) {
                quadAssignments.emplace_back(-1);
            }
        }
        Dirty = numVertices / 4 / 2;
        quadAssignments.at(numQuads) = numQuads;
        numQuads++;
        if (!isColor && vertices) {
            for (size_t n = 0; n < 4; ++n) {
                data.vertices.push_back(vertices[n]);
            }
        }
        if (isColor && color) {
            for (size_t n = 0; n < 4; ++n) {
                data.colorVertices.push_back(color[n]);
            }
        }
        return numQuads - 1;
    }
    for (size_t i = 0; i < numVertices / 4; ++i) {
        if (quadAssignments.at(i) == -1) {
            quadAssignments.at(i) = numQuads;
            if (!isColor && vertices) {
                for (size_t n = 0; n < 4; ++n) {
                    data.vertices.at(quadAssignments.at(i) * 4 + n) = vertices[n];
                }
            }
            if (isColor && color) {
                for (size_t n = 0; n < 4; ++n) {
                    data.colorVertices.at(quadAssignments.at(i) * 4 + n) = color[n];
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
    if (which < 0 || which >= numVertices / 4 || quadAssignments.at(which) == -1) {
        VS_LOG(error, "DelQuad error 1");
        return;
    }
    if (quadAssignments.at(which) >= numQuads) {
        VS_LOG(error, "error del (DelQuad error 2)");
        return;
    }
    Dirty++;
    for (size_t i = 0; i < numVertices / 4; ++i) {
        if (quadAssignments.at(i) == numQuads - 1) {
            if (isColor) {
                for (size_t j = 0; j < 4; ++j) {
                    data.colorVertices.at(quadAssignments.at(which) * 4 + j) = data.colorVertices.at((numQuads - 1) * 4 + j);
                }
            } else {
                for (size_t j = 0; j < 4; ++j) {
                    data.vertices.at(quadAssignments.at(which) * 4 + j) = data.vertices.at((numQuads - 1) * 4 + j);
                }
            }
            quadAssignments.at(i) = quadAssignments.at(which);
            quadAssignments.at(which) = -1;
            numQuads--;
            return;
        }
    }
    VS_LOG(error, " error deleting engine flame (DelQuad error 3)");
}

void GFXQuadList::ModQuad(int which, const GFXVertex *vertices, float alpha) {
    if (which < 0 || which >= numVertices / 4 || quadAssignments.at(which) == -1) {
        VS_LOG(error, "ModQuad error 1");
        return;
    }
    if (isColor) {
        int w = quadAssignments.at(which) * 4;

        for (size_t n = 0; n < 4; ++n) {
            data.colorVertices.at(w + n).SetVtx(vertices[n]);
        }
        if (alpha != -1) {
            if (alpha == 0) {
                alpha = .01;
            }
            float alp = std::max({ data.colorVertices.at(w).r, data.colorVertices.at(w).g, data.colorVertices.at(w).b });
            if (alp > .0001) {
                float tmpR = alpha * data.colorVertices.at(w).r / alp;
                float tmpG = alpha * data.colorVertices.at(w).g / alp;
                float tmpB = alpha * data.colorVertices.at(w).b / alp;
                float tmpAlpha = alpha;
                for (size_t n = 0; n < 4; ++n) {
                    data.colorVertices.at(w + n).r = tmpR;
                    data.colorVertices.at(w + n).g = tmpG;
                    data.colorVertices.at(w + n).b = tmpB;
                    data.colorVertices.at(w + n).a = tmpAlpha;
                }
            }
        }
    } else {
        for (size_t n = 0; n < 4; ++n) {
            data.vertices.at(quadAssignments.at(which) * 4 + n) = vertices[n];
        }
    }
}

void GFXQuadList::ModQuad(int which, const GFXColorVertex *vertices) {
    if (which < 0 || which >= numVertices / 4 || quadAssignments.at(which) == -1) {
        return;
    }
    if (isColor) {
        for (size_t n = 0; n < 4; ++n) {
            data.colorVertices.at((quadAssignments.at(which) * 4) + n) = vertices[n];
        }
    } else {
        for (size_t n = 0; n < 4; ++n) {
            data.vertices.at((quadAssignments.at(which) * 4) + n)
                    .SetTexCoord(vertices[n].s, vertices[n].t)
                    .SetNormal(Vector(vertices[n].i, vertices[n].j, vertices[n].k))
                    .SetVertex(Vector(vertices[n].x, vertices[n].y, vertices[n].z));
        }
    }
}
