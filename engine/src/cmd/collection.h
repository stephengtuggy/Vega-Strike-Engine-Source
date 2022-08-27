/*
 * collection.h
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
 * the Free Software Foundation, either version 2 of the License, or
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


/* unitCollection.h
 *
 *****/

#ifndef _UNITCOLLECTION_H_
#define _UNITCOLLECTION_H_

//Collection type:
//#define USE_OLD_COLLECTION
#define USE_STL_COLLECTION
//#define USE_BOOST_MULTI_INDEX
//#define USE_STL_COLLECTION_OF_SMART_PTRS

#if defined (USE_OLD_COLLECTION)
#include "oldcollection.h"
#elif defined (USE_STL_COLLECTION)
    #include "vega_stl_collection.hpp"
#elif defined (USE_BOOST_MULTI_INDEX)
    #include "unit_collection.hpp"
#elif defined (USE_STL_COLLECTION_OF_SMART_PTRS)
    #include "vega_stl_collection_of_smart_ptrs.hpp"
#else
#error "No collection type chosen in collection.h:10"
#endif //USE_STL_COLLECTION

#endif

