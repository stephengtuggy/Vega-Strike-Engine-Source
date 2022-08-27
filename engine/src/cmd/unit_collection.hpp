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

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

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
#include "unit_base_class.hpp"
//#include "unit_collection.hpp"
//class Unit;

//using ::boost::multi_index_container;
//using namespace ::boost::multi_index;


struct UnitSequenced {};
struct UnitByIdentity {};
struct UnitFlightgroup {};

typedef ::boost::multi_index_container<::boost::shared_ptr<UnitBaseClass>,
        ::boost::multi_index::indexed_by<
                ::boost::multi_index::sequenced<::boost::multi_index::tag<UnitSequenced>>,
                ::boost::multi_index::ordered_unique<::boost::multi_index::tag<UnitByIdentity>,
                        ::boost::multi_index::identity<UnitBaseClass>>,
                ::boost::multi_index::ordered_unique<::boost::multi_index::tag<UnitFlightgroup>,
                        BOOST_MULTI_INDEX_CONST_MEM_FUN(UnitBaseClass, ::std::string, getFgID)
                >
        >
> UnitBaseClassMultiIndexContainer;
typedef UnitBaseClassMultiIndexContainer VegaUnitCollection;
typedef VegaUnitCollection::index<UnitSequenced>::type BySequence;
typedef VegaUnitCollection::index_iterator<UnitSequenced>::type UnitSequencedIterator;
typedef VegaUnitCollection::index_const_iterator<UnitSequenced>::type UnitSequencedConstIterator;
typedef VegaUnitCollection::index_iterator<UnitByIdentity>::type UnitIdentityIterator;
typedef VegaUnitCollection::index_const_iterator<UnitByIdentity>::type UnitIdentityConstIterator;
typedef VegaUnitCollection::index_iterator<UnitFlightgroup>::type UnitFlightgroupIterator;
typedef VegaUnitCollection::index_const_iterator<UnitFlightgroup>::type UnitFlightgroupConstIterator;

#endif //VEGA_STRIKE_SRC_CMD_UNIT_COLLECTION_HPP_
