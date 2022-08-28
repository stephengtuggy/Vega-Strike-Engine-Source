/*
 * unit_base_class.cpp
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

#include "movable.h"
#include "pilot.h"
#include "mount_size.h"
#include "weapon_info.h"
#include "universe.h"
#include "gfx/camera.h"
#include "galaxy_xml.h"
#include "vs_random.h"
#include "csv.h"
#include "gfx/vsbox.h"
#include "missile.h"
#include "python/python_class.h"
#include "cmd/ai/script.h"
#include "cmd/ai/communication.h"
#include "script/flightgroup.h"
#include "xml_serializer.h"
#include "beam.h"
#include "base.h"
#include "unit_base_class.hpp"
#include <boost/format.hpp>

std::string UnitBaseClass::toString() const {
    return getFgID();
}

//std::string UnitBaseClass::getFlightgroupName() const {
//    return flightgroup_name_;
//}

int32_t UnitBaseClass::getFlightgroupSubNumber() const {
    return flightgroup_sub_number_;
}

bool UnitBaseClass::operator<(UnitBaseClass &other) const {
    return toString() < other.toString();
//    const std::string &this_flightgroup_name = this->getFlightgroupName();
//    const std::string &other_flightgroup_name = other.getFlightgroupName();
//    if (this_flightgroup_name < other_flightgroup_name) {
//        return true;
//    } else if (this_flightgroup_name > other_flightgroup_name) {
//        return false;
//    } else {
//        return this->getFlightgroupSubNumber() < other.getFlightgroupSubNumber();
//    }
}

bool UnitBaseClass::operator<(const UnitBaseClass &other) const {
    return toString() < other.toString();
//    const std::string &this_flightgroup_name = this->getFlightgroupName();
//    const std::string &other_flightgroup_name = other.getFlightgroupName();
//    if (this_flightgroup_name < other_flightgroup_name) {
//        return true;
//    } else if (this_flightgroup_name > other_flightgroup_name) {
//        return false;
//    } else {
//        return this->getFlightgroupSubNumber() < other.getFlightgroupSubNumber();
//    }
}

std::string UnitBaseClass::getFgID() const {
    if (flightgroup != nullptr) {
        return (boost::format("%1$s-%2$4d") % flightgroup->name % flightgroup_sub_number_).str();
    } else {
        return fullname;
    }
}

void UnitBaseClass::SetFaction(int new_faction) {
    this->faction = new_faction;
}

void UnitBaseClass::SetFg(Flightgroup *fg, int fg_subnumber) {
    flightgroup = fg;
//    flightgroup_subnumber = fg_subnumber;
    flightgroup_name_ = fg->name;
    flightgroup_sub_number_ = fg_subnumber;
}

std::string UnitBaseClass::getFlightgroupName() const {
    return flightgroup->name;
}

//int32_t UnitBaseClass::getFlightgroupSubNumber() const {
//    return flightgroup_subnumber;
//}

std::string UnitBaseClass::getName() const {
    return *name;
}

// TODO: Add some more logic related to potentially changing the Unit's key in various collections
void UnitBaseClass::setName(std::string new_name) {
    name = stringPoolUpsert(new_name);
}

bool UnitBaseClass::failedToLoad() const {
    return getName() == kLoadFailed;
}

bool UnitBaseClass::loadedSuccessfully() const {
    return !failedToLoad();
}

void UnitBaseClass::setFailedToLoad() {
    setName(kLoadFailed);
}
