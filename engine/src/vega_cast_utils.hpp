/*
 * vega_cast_utils.hpp
 *
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

#ifndef VEGA_STRIKE_SRC_VEGA_CAST_UTILS_HPP_
#define VEGA_STRIKE_SRC_VEGA_CAST_UTILS_HPP_

#include <typeinfo>
#include <cstdlib>
#include "vs_logging.h"
#include "vs_globals.h"

template<class TargetType, class SourceType>
inline TargetType* vega_dynamic_cast_ptr(SourceType* from) {
    TargetType* ret_val = nullptr;
    try {
        ret_val = dynamic_cast<TargetType*>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type %2%* to %3%*") % e.what() % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    if (ret_val == nullptr) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type %1%* to %2%* -- nullptr encountered") % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    return ret_val;
}

template<class TargetType, class SourceType>
inline TargetType const * vega_dynamic_const_cast_ptr(SourceType const * from) {
    TargetType* ret_val = nullptr;
    try {
        ret_val = dynamic_cast<TargetType const *>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type const %2%* to const %3%*") % e.what() % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    if (ret_val == nullptr) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type const %1%* to const %2%* -- nullptr encountered") % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    return ret_val;
}

template<class TargetType, class SourceType>
inline TargetType& vega_dynamic_cast_ref(SourceType& from) {
    try {
        return dynamic_cast<TargetType &>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type %2%& to %3%&") % e.what() % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
}

template<class TargetType, class SourceType>
inline boost::shared_ptr<TargetType> vega_dynamic_cast_boost_shared_ptr(boost::shared_ptr<SourceType> from) {
    boost::shared_ptr<TargetType> return_value{nullptr};
    try {
        return_value = boost::dynamic_pointer_cast<TargetType>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type boost::shared_ptr<%2%> to boost::shared_ptr<%3%>") % e.what() % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    if (!return_value) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type boost::shared_ptr<%1%> to boost::shared_ptr<%2%>") % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    return return_value;
}

template<class TargetType>
inline boost::shared_ptr<TargetType> vega_dynamic_cast_boost_shared_ptr_from_void(boost::shared_ptr<void> from) {
    boost::shared_ptr<TargetType> return_value{nullptr};
    try {
        return_value = boost::dynamic_pointer_cast<TargetType>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type boost::shared_ptr<%2%> to boost::shared_ptr<%3%>") % e.what() % typeid(void).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    if (!return_value) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type boost::shared_ptr<%1%> to boost::shared_ptr<%2%>") % typeid(void).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    return return_value;
}
template<class SourceType>
inline boost::shared_ptr<void> vega_dynamic_cast_boost_shared_ptr_to_void(boost::shared_ptr<SourceType> from) {
    boost::shared_ptr<void> return_value{nullptr};
    try {
        return_value = boost::dynamic_pointer_cast<void>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type boost::shared_ptr<%2%> to boost::shared_ptr<%3%>") % e.what() % typeid(SourceType).name() % typeid(void).name()));
        VSExit(-422);
    }
    if (!return_value) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type boost::shared_ptr<%1%> to boost::shared_ptr<%2%>") % typeid(SourceType).name() % typeid(void).name()));
        VSExit(-422);
    }
    return return_value;
}

template<class TargetType, class SourceType>
inline const boost::shared_ptr<TargetType> vega_dynamic_const_cast_boost_shared_ptr(const boost::shared_ptr<SourceType> from) {
    boost::shared_ptr<TargetType> return_value{nullptr};
    try {
        return_value = boost::dynamic_pointer_cast<TargetType>(from);
    } catch (std::bad_cast& e) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Error '%1%' casting type boost::shared_ptr<%2%> to boost::shared_ptr<%3%>") % e.what() % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    if (!return_value) {
        VS_LOG_AND_FLUSH(fatal, (boost::format("Fatal Failure to Cast type boost::shared_ptr<%1%> to boost::shared_ptr<%2%>") % typeid(SourceType).name() % typeid(TargetType).name()));
        VSExit(-422);
    }
    return return_value;
}

#endif //VEGA_STRIKE_SRC_VEGA_CAST_UTILS_HPP_
