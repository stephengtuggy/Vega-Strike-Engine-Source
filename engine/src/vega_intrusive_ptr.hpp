/*
 * Copyright (C) 2001-2022 Daniel Horn, Stephen G. Tuggy,
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

#ifndef VEGA_STRIKE_SRC_VEGA_INTRUSIVE_PTR_HPP_
#define VEGA_STRIKE_SRC_VEGA_INTRUSIVE_PTR_HPP_

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

template<class T> struct intrusive_deleter
{
    void operator()(T * p)
    {
        if (p) {
            intrusive_ptr_release(p);
        }
    }

    void operator()(boost::intrusive_ptr<T> p)
    {
        if (p) {
            intrusive_ptr_release(p);
        }
    }
};

template<class T> boost::shared_ptr<T> make_shared_from_intrusive(T * p)
{
    if (p) {
        intrusive_ptr_add_ref(p);
    }
    boost::shared_ptr<T> px(p, intrusive_deleter<T>());
    return px;
}

template<class T> boost::shared_ptr<T> make_shared_from_intrusive(boost::intrusive_ptr<T> p)
{
    if (p) {
        intrusive_ptr_add_ref(p);
    }
    boost::shared_ptr<T> px(p, intrusive_deleter<T>());
    return px;
}

#endif //VEGA_STRIKE_SRC_VEGA_INTRUSIVE_PTR_HPP_
