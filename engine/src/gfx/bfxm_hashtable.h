/*
 * bfxm_hashtable.h
 *
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Daniel Horn, project creator
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
#ifndef VEGA_STRIKE_ENGINE_GFX_BFXM_HASHTABLE_H
#define VEGA_STRIKE_ENGINE_GFX_BFXM_HASHTABLE_H

#include <string>
#include <utility>
#include <vector>
#include <deque>
#include "hashtable.h"
#include "shared_ptr_hashtable.h"
#include "vs_logging.h"

class Mesh;

namespace vega_gfx {

constexpr int kMeshHashtableSize = 503;

class bfxmHashtable : vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize> {
private:
    bfxmHashtable() = default;

    inline ~bfxmHashtable() {
        VS_LOG_AND_FLUSH(info, "Destroying bfxmHashtable");
        Clear();
    }

public:
    static bfxmHashtable & instance() {
        static bfxmHashtable instance;
        return instance;
    }

    static size_t hash(const int key) {
        return vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::hash(key);
    }

    static size_t hash(const char *key) {
        return vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::hash(key);
    }

    static size_t hash(const std::string &key) {
        return vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::hash(key);
    }

    std::vector<std::shared_ptr<std::deque<std::shared_ptr<Mesh>>>> GetAll() const {
        return vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::GetAll();
    }

    std::shared_ptr<std::deque<std::shared_ptr<Mesh>>> Get(const std::string &key) const {
        return vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::Get(key);
    }

    void Put(const std::string &key, std::shared_ptr<std::deque<std::shared_ptr<Mesh>>> value) {
        vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::Put(key, std::move(value));
    }

    void Delete(const std::string &key) {
        vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::Delete(key);
    }

    void Clear() {
        vega_collection::SharedPtrHashtable<std::string, std::deque<std::shared_ptr<Mesh>>, kMeshHashtableSize>::Clear();
    }
};

} // vega_gfx

#endif //VEGA_STRIKE_ENGINE_GFX_BFXM_HASHTABLE_H
