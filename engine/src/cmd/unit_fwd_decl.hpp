/*
 * unit_fwd_decl.hpp
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy, Roy Falk,
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

#ifndef VEGA_STRIKE_SRC_CMD_UNIT_FWD_DECL_HPP_
#define VEGA_STRIKE_SRC_CMD_UNIT_FWD_DECL_HPP_

#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "vega_intrusive_ptr.hpp"

class Unit;

using UnitRawPtr = Unit *;
using UnitConstRawPtr = Unit const *;
using UnitIntrusivePtr = boost::intrusive_ptr<Unit>;
using UnitSharedPtr = boost::shared_ptr<Unit>;
using UnitWeakPtr = boost::weak_ptr<Unit>;

using UnitPtr = UnitSharedPtr;
using UnitParentPtr = UnitWeakPtr;
using UnitPtrForPy = UnitSharedPtr; //UnitRawPtr;

using UnitRef = Unit &;

#endif //VEGA_STRIKE_SRC_CMD_UNIT_FWD_DECL_HPP_
