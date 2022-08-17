/*
 * unit_collection.hpp
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
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
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VEGA_STRIKE_SRC_CMD_UNIT_COLLECTION_HPP_
#define VEGA_STRIKE_SRC_CMD_UNIT_COLLECTION_HPP_

//#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
//#endif

#include <utility>
#include <string>
#include <cstdint>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "unit_generic.h"
//class Unit;

//using ::boost::multi_index_container;
//using namespace ::boost::multi_index;


struct UnitSequenced {};
struct UnitIdentity {};
struct UnitFlightgroup {};

typedef ::boost::multi_index_container<Unit,
        ::boost::multi_index::indexed_by<
                ::boost::multi_index::sequenced<::boost::multi_index::tag<UnitSequenced>>,
                ::boost::multi_index::ordered_unique<::boost::multi_index::tag<UnitIdentity>, ::boost::multi_index::identity<Unit>>,
                ::boost::multi_index::ordered_unique<::boost::multi_index::tag<UnitFlightgroup>,
                        ::boost::multi_index::composite_key<
        Unit,
        BOOST_MULTI_INDEX_CONST_MEM_FUN(Unit, ::std::string, getFlightgroupName),
        BOOST_MULTI_INDEX_CONST_MEM_FUN(Unit, int32_t, getFlightgroupSubNumber)
>>>> UnitMultiIndexContainer;
typedef UnitMultiIndexContainer VegaUnitCollection;
typedef UnitMultiIndexContainer::index_iterator<UnitSequenced> UnitSequencedIterator;
typedef UnitMultiIndexContainer::index_const_iterator<UnitSequenced> UnitSequencedConstIterator;
typedef UnitMultiIndexContainer::index_iterator<UnitIdentity> UnitIdentityIterator;
typedef UnitMultiIndexContainer::index_const_iterator<UnitIdentity> UnitIdentityConstIterator;
typedef UnitMultiIndexContainer::index_iterator<UnitFlightgroup> UnitFlightgroupIterator;
typedef UnitMultiIndexContainer::index_const_iterator<UnitFlightgroup> UnitFlightgroupConstIterator;
typedef UnitSequencedIterator un_iter;
typedef UnitSequencedConstIterator un_kiter;

#endif //VEGA_STRIKE_SRC_CMD_UNIT_COLLECTION_HPP_
