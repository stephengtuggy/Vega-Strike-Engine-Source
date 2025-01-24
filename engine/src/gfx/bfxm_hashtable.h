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
#include <vector>
#include "hashtable.h"
#include "vs_logging.h"

class Mesh;

namespace vega_gfx {

#define MESH_HASTHABLE_SIZE (503)

class bfxmHashtable : Hashtable<std::string, std::vector<Mesh *>, MESH_HASTHABLE_SIZE> {
private:
    bfxmHashtable() = default;

    inline ~bfxmHashtable() {
        VS_LOG_AND_FLUSH(info, "Destroying bfxmHashtable");
        clear();
    }

public:
    static bfxmHashtable & instance() {
        static bfxmHashtable instance;
        return instance;
    }

    static size_t hash(const int key) {
        return Hashtable::hash(key);
    }

    static size_t hash(const char *key) {
        return Hashtable::hash(key);
    }

    static size_t hash(const std::string &key) {
        return Hashtable::hash(key);
    }

    std::vector<std::vector<Mesh *> *> GetAll() const {
        return Hashtable::GetAll();
    }

    std::vector<Mesh *> *Get(const std::string &key) const {
        return Hashtable::Get(key);
    }

    void Put(const std::string &key, std::vector<Mesh *> *value) {
        Hashtable::Put(key, value);
    }

    void Delete(const std::string &key) {
        Hashtable::Delete(key);
    }
};

} // vega_gfx

#endif //VEGA_STRIKE_ENGINE_GFX_BFXM_HASHTABLE_H
