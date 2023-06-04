/*
 * gl_vertex_list.cpp
 *
 * Copyright (C) 2001-2023 Daniel Horn, Alan Shieh, pyramid3d,
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


#include "gfxlib_struct.h"
#include "vegastrike.h"
#include "vs_globals.h"
#include "vs_logging.h"
#include <assert.h>
#ifndef NO_GFX //Server cannot depend on GL, but still needs a mesh library.
#include "gl_globals.h"
#else
#define GFX_BUFFER_MAP_UNMAP false
#endif
//Untransformed and transformed data

#ifndef GFX_SCALE
#define GFX_SCALE 1./1024.
#endif

#include "gnuhash.h"

GFXVertexList *next;

struct VertexCompare {
    bool operator()(const GFXVertex *a, const GFXVertex *b) const {
        if ((sizeof(GFXVertex) % sizeof(unsigned int)) != 0) {
            return memcmp(a, b, sizeof(GFXVertex)) < 0;
        } else {
            // faster memcmp, since it gets unrolled by the compiler
            const unsigned int *ia = reinterpret_cast<const unsigned int *>(a);
            const unsigned int *ib = reinterpret_cast<const unsigned int *>(b);

            for (size_t i = 0; i < (sizeof(*a) / sizeof(*ia)); ++i) {
                if (ia[i] < ib[i]) {
                    return true;
                } else if (ia[i] > ib[i]) {
                    return false;
                }
            }

            return false;
        }
    }
};

#include <map>

void GFXOptimizeList(GFXVertex *old, int numV, GFXVertex **nw, int *nnewV, unsigned int **ind) {
    std::map<GFXVertex *, int, VertexCompare> vtxcache;

    *ind = (unsigned int *) malloc(sizeof(unsigned int) * numV);
    *nw = (GFXVertex *) malloc(numV * sizeof(GFXVertex));
    int _nnewV = *nnewV = 0;
    int i;
    bool quickpath = true;
    for (i = 0; i < numV; i++) {
        std::map<GFXVertex *, int, VertexCompare>::const_iterator it = vtxcache.find(old + i);
        if (it != vtxcache.end()) {
            if (quickpath && i > 0) {
                quickpath = false;
                memcpy(*nw, old, sizeof(GFXVertex) * size_t(i));
            }
            (*ind)[i] = it->second;
        } else {
            if (!quickpath) {
                memcpy((*nw) + _nnewV, old + i, sizeof(GFXVertex));
            }
            vtxcache[old + i] = ((*ind)[i]) = _nnewV;
            ++_nnewV;
        }
    }
    if (quickpath && i > 0) {
        memcpy(*nw, old, sizeof(GFXVertex) * size_t(i));
    }
    *nnewV = _nnewV;

    VS_LOG(trace, (boost::format("Optimized vertex list - vertices: %1% -> %2%") % numV % *nnewV));
}

void GFXVertexList::Init(enum POLYTYPE *poly,
        int numVertices,
        const GFXVertex *vertices,
        const GFXColorVertex *colors,
        int numlists,
        int *offsets,
        bool Mutable,
        unsigned int *indices) {
    vbo_data = 0;
    vbo_elements = nullptr;

    int stride = 0;

    changed = HAS_COLOR * ((colors != NULL) ? 1 : 0);
    if (numlists > 0) {
        mode = new POLYTYPE[numlists];
        unique_mode = true;
        POLYTYPE umode = *poly;
        for (int pol = 0; pol < numlists; pol++) {
            mode[pol] = poly[pol];             //PolyLookup (poly[pol]);
            if (unique_mode && (poly[pol] != umode)) {
                unique_mode = false;
            }
        }
    } else {
        mode = NULL;
        unique_mode = false;
    }
    this->numlists = numlists;
    this->numVertices = numVertices;
    if (numVertices) {
        if (vertices) {
            if (!GFX_BUFFER_MAP_UNMAP) {
                data.vertices = (GFXVertex *) malloc(sizeof(GFXVertex) * numVertices);
                memcpy(data.vertices, vertices, sizeof(GFXVertex) * numVertices);
            } else {
                data.vertices = const_cast< GFXVertex * > (vertices);                  //will *not* modify
            }
        } else if (colors) {
            if (!GFX_BUFFER_MAP_UNMAP) {
                data.colors = (GFXColorVertex *) malloc(sizeof(GFXColorVertex) * numVertices);
                memcpy(data.colors, colors, sizeof(GFXColorVertex) * numVertices);
            } else {
                data.colors = const_cast< GFXColorVertex * > (colors);
            }
        }
    } else {
        data.vertices = NULL;
        data.colors = NULL;
    }
    this->offsets = new int[numlists];
    memcpy(this->offsets, offsets, sizeof(int) * numlists);
    int i;
    unsigned int numindices = 0;
    for (i = 0; i < numlists; i++) {
        numindices += offsets[i];
    }
    display_list = 0;
    if (Mutable) {
        changed |= CHANGE_MUTABLE;
    } else {
        changed |= CHANGE_CHANGE;
    }
    if (indices) {
        stride = INDEX_BYTE;
        if (numVertices > 255) {
            stride = INDEX_SHORT;
        }
        if (numVertices > 65535) {
            stride = INDEX_INT;
        }

        index.b = (unsigned char *) malloc(static_cast<size_t>(stride) * static_cast<size_t>(numindices));
        switch (stride) {
            case INDEX_BYTE:
                VS_LOG(trace, "Optimized vertex list - using 8-bit indices");
                for (unsigned int i = 0; i < numindices; i++) {
                    index.b[i] = indices[i];
                }
                break;
            case INDEX_SHORT:
                VS_LOG(trace, "Optimized vertex list - using 16-bit indices");
                for (unsigned int i = 0; i < numindices; i++) {
                    index.s[i] = indices[i];
                }
                break;
            case INDEX_INT:
                VS_LOG(debug, "Optimized vertex list - using 32-bit indices");
                for (unsigned int i = 0; i < numindices; i++) {
                    index.i[i] = indices[i];
                }
                break;
        }
    } else {
        index.b = NULL;
    }
    changed |= stride;
    RenormalizeNormals();
    RefreshDisplayList();
    if (GFX_BUFFER_MAP_UNMAP) {
        if (!vbo_data) {
            //backstore required
            if (numVertices) {
                if (vertices) {
                    data.vertices = (GFXVertex *) malloc(sizeof(GFXVertex) * numVertices);
                    memcpy(data.vertices, vertices, sizeof(GFXVertex) * numVertices);
                } else if (colors) {
                    data.colors = (GFXColorVertex *) malloc(sizeof(GFXColorVertex) * numVertices);
                    memcpy(data.colors, colors, sizeof(GFXColorVertex) * numVertices);
                }
            }
        } else {
            if (index.b != nullptr) {
                free(index.b);
                index.b = nullptr;
            }
            data.vertices = nullptr;
            data.colors = nullptr;
        }
    }
    if (Mutable) {
        changed |= CHANGE_MUTABLE;          //for display lists
    } else {
        changed &= (~CHANGE_CHANGE);
    }
}

int GFXVertexList::numTris() const {
    int tot = 0;
    for (int i = 0; i < numlists; i++) {
        switch (mode[i]) {
            case GFXTRI:
                tot += offsets[i] / 3;
                break;
            case GFXTRISTRIP:
            case GFXTRIFAN:
            case GFXPOLY:
                tot += offsets[i] - 2;
                break;
            default:
                break;
        }
    }
    return tot;
}

int GFXVertexList::numQuads() const {
    int tot = 0;
    for (int i = 0; i < numlists; i++) {
        switch (mode[i]) {
            case GFXQUAD:
                tot += offsets[i] / 4;
                break;
            case GFXQUADSTRIP:
                tot += (offsets[i] - 2) / 2;
                break;
            default:
                break;
        }
    }
    return tot;
}

void GFXVertexList::VtxCopy(GFXVertexList *thus, GFXVertex *dst, int offset, int howmany) {
    memcpy(dst, &thus->data.vertices[offset], sizeof(GFXVertex) * howmany);
}

void GFXVertexList::ColVtxCopy(GFXVertexList *thus, GFXVertex *dst, int offset, int howmany) {
    for (int i = 0; i < howmany; i++) {
        dst[i].
                SetTexCoord(thus->data.colors[i + offset].s, thus->data.colors[i + offset].t).
                SetNormal(Vector(thus->data.colors[i + offset].i,
                thus->data.colors[i + offset].j,
                thus->data.colors[i + offset].k)).
                SetVertex(Vector(thus->data.colors[i + offset].x,
                thus->data.colors[i + offset].y,
                thus->data.colors[i + offset].z)).
                SetTangent(Vector(thus->data.colors[i + offset].tx,
                thus->data.colors[i + offset].ty,
                thus->data.colors[i + offset].tz), thus->data.colors[i + offset].tw);
    }
}

void GFXVertexList::RenormalizeNormals() {
    if (data.colors == 0 && data.vertices == 0) {
        return;
    }          //
    if (numVertices > 0) {
        Vector firstNormal;
        if (changed & HAS_COLOR) {
            firstNormal = data.colors[0].GetNormal();
        } else {
            firstNormal = data.vertices[0].GetNormal();
        }
        float mag = firstNormal.Magnitude();
        if (mag > GFX_SCALE / 1.5 && mag < GFX_SCALE * 1.5) {
            return;
        }
        if (mag < GFX_SCALE / 100 && mag < .00001) {
            firstNormal.Set(1, 0, 0);
        }
        firstNormal.Normalize();
        if (changed & HAS_COLOR) {
            data.colors[0].SetNormal(firstNormal);
        } else {
            data.vertices[0].SetNormal(firstNormal);
        }
        if (changed & HAS_COLOR) {
            for (int i = 0; i < numVertices; i++) {
                //data.colors[i].SetNormal(data.colors[i].GetNormal().Normalize());
                data.colors[i].i *= GFX_SCALE;
                data.colors[i].j *= GFX_SCALE;
                data.colors[i].k *= GFX_SCALE;
            }
        } else {
            for (int i = 0; i < numVertices; i++) {
                //data.vertices[i].SetNormal(data.vertices[i].GetNormal().Normalize());
                data.vertices[i].i *= GFX_SCALE;
                data.vertices[i].j *= GFX_SCALE;
                data.vertices[i].k *= GFX_SCALE;
            }
        }
    }
}

unsigned int GFXVertexList::GetIndex(int offset) const {
    return (changed & INDEX_BYTE)
            ? (unsigned int) (index.b[offset])
            : ((changed & INDEX_SHORT)
                    ? (unsigned int) (index.s[offset])
                    : index.i[offset]);
}

void GFXVertexList::ColIndVtxCopy(GFXVertexList *thus, GFXVertex *dst, int offset, int howmany) {
    for (int i = 0; i < howmany; i++) {
        unsigned int j = thus->GetIndex(i + offset);
        dst[i].
                SetTexCoord(thus->data.colors[j].s, thus->data.colors[j].t).
                SetNormal(Vector(thus->data.colors[j].i, thus->data.colors[j].j, thus->data.colors[j].k)).
                SetVertex(Vector(thus->data.colors[j].x, thus->data.colors[j].y, thus->data.colors[j].z)).
                SetTangent(Vector(thus->data.colors[j].tx,
                thus->data.colors[j].ty,
                thus->data.colors[j].tz), thus->data.colors[j].tw);
    }
}

void GFXVertexList::IndVtxCopy(GFXVertexList *thus, GFXVertex *dst, int offset, int howmany) {
    for (int i = 0; i < howmany; ++i) {
        if (i < 0) {
            VS_LOG(error, (boost::format("GFXVertexList::IndVtxCopy: i is less than 0. Value of i: %1%") % i));
            break;
        }
        unsigned int j = thus->GetIndex(i + offset);
        if (j < 0) {
            VS_LOG(error, (boost::format("GFXVertexList::IndVtxCopy: j is less than 0. Value of j: %1%") % j));
            break;
        } else if (j == 0) {
            VS_LOG(error, "GFXVertexList::IndVtxCopy: j is zero");
            break;
        } else if (j > thus->numVertices) {
            VS_LOG(error, (boost::format("GFXVertexList::IndVtxCopy: j (%1%) exceeds numVertices (%2%)") % j % thus->numVertices));
            break;
        }
        dst[i].
                SetTexCoord(thus->data.vertices[j].s, thus->data.vertices[j].t).
                SetNormal(Vector(thus->data.vertices[j].i, thus->data.vertices[j].j, thus->data.vertices[j].k)).
                SetVertex(Vector(thus->data.vertices[j].x, thus->data.vertices[j].y, thus->data.vertices[j].z)).
                SetTangent(Vector(thus->data.vertices[j].tx,
                thus->data.vertices[j].ty,
                thus->data.vertices[j].tz), thus->data.vertices[j].tw);
    }
}

bool GFXVertexList::hasColor() const {
    return (changed & HAS_COLOR) != 0;
}

const GFXVertex *GFXVertexList::GetVertex(int index) const {
    return data.vertices + index;
}

const GFXColorVertex *GFXVertexList::GetColorVertex(int index) const {
    return data.colors + index;
}

vega_types::SharedPtr<vega_types::ContiguousSequenceContainer<GFXVertex>> GFXVertexList::GetPolys(size_t &num_tris,
                                                                                                  size_t &num_quads,
                                                                                                  size_t &total_num_polys) {
    if (numVertices == 0) {
        num_tris = 0;
        num_quads = 0;
        total_num_polys = 0;
        return nullptr;
    }
    this->Map(true, false);
    void (*vtxcpy)(GFXVertexList *thus, GFXVertex *dst, int offset, int howmany);
    vtxcpy = (changed & HAS_COLOR)
            ? ((changed & HAS_INDEX)
                    ? ColIndVtxCopy
                    : ColVtxCopy)
            : ((changed & HAS_INDEX)
                    ? IndVtxCopy
                    : VtxCopy);
    int cur = 0;
    num_tris = numTris();
    num_quads = numQuads();
    total_num_polys = num_tris + num_quads;
//    int curtri = 0;
//    int curquad = 3 * (num_tris);
    int cur_poly = 0;
    size_t num_elems_desired{};
    for (size_t i = 0; i < numlists; ++i) {
        num_elems_desired += offsets[i];
    }
    vega_types::SharedPtr<vega_types::ContiguousSequenceContainer<GFXVertex>> return_value = vega_types::MakeShared<vega_types::ContiguousSequenceContainer<GFXVertex>>(num_elems_desired);
//    return_value->reserve(num_elems_desired);
    for (size_t idx = 0; idx < num_elems_desired; ++idx) {
        GFXVertex gfx_vertex{};
        return_value->push_back(gfx_vertex);
    }
    for (size_t i = 0; i < numlists; ++i) {
        int j;
        switch (mode[i]) {
            case GFXTRI:
                (*vtxcpy)(this, &(return_value->data()[cur_poly]), cur, offsets[i]);
                cur_poly += offsets[i];
                break;
            case GFXTRIFAN:
            case GFXPOLY:
                for (j = 1; j < offsets[i] - 1; j++) {
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), cur, 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j + 1), 1);
                }
                break;
            case GFXTRISTRIP:
                for (j = 2; j < offsets[i]; j += 2) {
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j - 2), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j - 1), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j), 1);
                    if (j + 1 < offsets[i]) {
                        //copy reverse
                        (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j), 1);
                        (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j - 1), 1);
                        (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j + 1), 1);
                    }
                }
                break;
            case GFXQUAD:
                (*vtxcpy)(this, &(return_value->data()[cur_poly]), (cur), offsets[i]);
                cur_poly += offsets[i];
                break;
            case GFXQUADSTRIP:
                for (j = 2; j < offsets[i] - 1; j += 2) {
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j - 2), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j - 1), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j + 1), 1);
                    (*vtxcpy)(this, &(return_value->data()[cur_poly++]), (cur + j), 1);
                }
                break;
            case GFXLINE:
                VS_LOG(trace, "GFXVertexList::GetPolys encountered a GFXLINE");
                break;
            case GFXLINESTRIP:
                VS_LOG(trace, "GFXVertexList::GetPolys encountered a GFXLINESTRIIP");
                break;
            case GFXPOINT:
                VS_LOG(trace, "GFXVertexList::GetPolys encountered a GFXPOINT");
                break;
            default:
                break;
        }
        cur += offsets[i];
    }
    this->UnMap();
    return return_value;
}

void GFXVertexList::LoadDrawState() {
}

POLYTYPE GFXVertexList::getPolyType(size_t idx) const {
    return mode[idx];
}

size_t GFXVertexList::getOffset(size_t idx) const {
    return offsets[idx];
}
