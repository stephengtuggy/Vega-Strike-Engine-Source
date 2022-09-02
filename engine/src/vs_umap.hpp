/*
 * vs_umap.hpp
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Roy Falk, Stephen G. Tuggy
 * and other Vega Strike contributors
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

#ifndef VEGA_STRIKE_SRC_VS_UMAP_HPP_
#define VEGA_STRIKE_SRC_VS_UMAP_HPP_

#include <unordered_map>
#include <utility>

template<typename TKey>
using vsHash = std::hash<TKey>;

template<typename TKey>
using vsHashComp = std::hash_compare<TKey>;

template<typename TKey, typename TValue, typename THash = vsHash<TKey>, typename TKeyEq = vsHashComp<TKey>, typename TAlloc = std::allocator<std::pair<const TKey, TValue>>>
using vsUMap = std::unordered_map<TKey, TValue, THash, TKeyEq, TAlloc>;

#endif //VEGA_STRIKE_SRC_VS_UMAP_HPP_
