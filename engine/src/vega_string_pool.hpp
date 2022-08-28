/*
 * vega_string_pool.hpp
 *
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

#ifndef VEGA_STRIKE_SRC_VEGA_STRING_POOL_HPP_
#define VEGA_STRIKE_SRC_VEGA_STRING_POOL_HPP_

#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

typedef boost::multi_index_container<boost::shared_ptr<std::string>,
        boost::multi_index::indexed_by<boost::multi_index::ordered_unique<boost::multi_index::identity<std::string>
        >>> VegaStringPool;
typedef boost::shared_ptr<std::string> VegaStringPtr;

VegaStringPool& stringPoolSingleton() {
    static VegaStringPool kStringPool{};
    return kStringPool;
}

VegaStringPtr stringPoolUpsert(std::string my_str) {
    if (!stringPoolSingleton().contains(my_str)) {
        stringPoolSingleton().get<0>().insert(boost::make_shared<std::string>(my_str));
    }
    return *(stringPoolSingleton().get<0>().find(my_str));
}

VegaStringPtr stringPoolUpsert(const char * my_str) {
    std::string temp_string{my_str};
    return stringPoolUpsert(temp_string);
}

VegaStringPtr stringPoolUpsert(char * my_str) {
    std::string temp_string{my_str};
    return stringPoolUpsert(temp_string);
}

#endif //VEGA_STRIKE_SRC_VEGA_STRING_POOL_HPP_
