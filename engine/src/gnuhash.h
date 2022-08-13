/*
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
 * and other Vega Strike contributors.
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

#ifndef _GNUHASH_H_
#define _GNUHASH_H_
#include <unordered_map>

#define vsUMap     std::unordered_map
#define vsHashComp std::hash_compare
#define vsHash     std::hash
class Unit;

namespace std {

// defining things in std:: for basic types is ... frowned upon: http://eel.is/c++draft/namespace.std#1
template<>
struct hash<void *> {
    hash<size_t> a;
public:
    size_t operator()(const void *key) const {
        return a((size_t) key);
    }
};
template<>
struct hash<const void *> {
    hash<size_t> a;
public:
    size_t operator()(const void *const &key) const {
        return a((size_t) key);
    }
};

template<>
struct hash<const UnitPtr> {
    hash<size_t> a;
public:
    size_t operator()(const UnitPtr const &key) const {
        return a((size_t) key >> 4);
    }
};
template<>
struct hash<std::pair<UnitPtr, UnitPtr> > {
    hash<size_t> a;
public:
    size_t operator()(const std::pair<UnitPtr, UnitPtr> &key) const {
        return (size_t) ((size_t) (a((int) (((size_t) key.first) >> 4)))
                ^ ((size_t) a((int) (((size_t) key.second) >> 4))));
    }
};
#ifdef __GNUC__
//Minimum declaration needed by SharedPool.h
template<class Key, class Traits = std::less<Key> >
class hash_compare {
public:
    static const size_t bucket_size = 4;
    static const size_t min_buckets = 8;
};
#endif
}

#endif //def _GNUHASH_H_

