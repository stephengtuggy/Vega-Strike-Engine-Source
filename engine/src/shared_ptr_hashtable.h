/*
 * shared_ptr_hashtable.h
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
#ifndef VEGA_STRIKE_ENGINE_GFX_SHARED_PTR_HASHTABLE_H
#define VEGA_STRIKE_ENGINE_GFX_SHARED_PTR_HASHTABLE_H

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <vector>
#include "gnuhash.h"

#define HASH_INTSIZE (sizeof (int)*8)
#define HASH_SALT_0 0x7EF92C3B
#define HASH_SALT_1 0x9B

namespace vega_collection {

template<class KEY, class VALUE, int SIZ>
class SharedPtrHashtable : std::unordered_map<KEY, std::shared_ptr<VALUE>> {
    typedef std::pair<KEY, std::shared_ptr<VALUE>> HashElement;
    typedef std::unordered_map<KEY, std::shared_ptr<VALUE>> supertype;

    bool destroying_hashtable{};

public:
    static size_t hash(const int key) {
        return std::unordered_map<KEY, std::shared_ptr<VALUE>>::hash(key);
    }

    static size_t hash(const char *key) {
        return std::unordered_map<KEY, std::shared_ptr<VALUE>>::hash(key);
    }

    static size_t hash(const std::string &key) {
        return std::unordered_map<KEY, std::shared_ptr<VALUE>>::hash(key);
    }

    std::vector<std::shared_ptr<VALUE>> GetAll() const {
        std::vector<std::shared_ptr<VALUE>> return_value(this->size());
        for (auto it = this->begin(); it != this->end(); ++it) {
            return_value.emplace_back(it->second);
        }
        return return_value;
    }

    std::shared_ptr<VALUE> Get(const KEY &key) const {
        auto it = this->find(key);
        if (it == this->end()) {
            return it->second;
        }
        return nullptr;
    }

    void Put(const KEY &key, std::shared_ptr<VALUE> value) {
        (*this)[key] = value;
    }

    void Delete(const KEY &key) {
        if (destroying_hashtable) {
            return;
        }
        auto it = this->find(key);
        if (it == this->end()) {
            return;
        }
        this->erase(it);
    }

    void Clear() {
        this->clear();
    }

    ~SharedPtrHashtable() {
        destroying_hashtable = true;
        Clear();
    }
};

} // vega_collection

#endif //VEGA_STRIKE_ENGINE_GFX_SHARED_PTR_HASHTABLE_H
