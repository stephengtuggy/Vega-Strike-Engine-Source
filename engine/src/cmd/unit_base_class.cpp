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

#include "unit_base_class.hpp"
#include <boost/format.hpp>

std::string UnitBaseClass::toString() {
    return (boost::format("%1%-%2%") % getFlightgroupName() % getFlightgroupSubNumber()).str();
}

std::string UnitBaseClass::getFlightgroupName() const {
    return flightgroup_name_;
}

int32_t UnitBaseClass::getFlightgroupSubNumber() const {
    return flightgroup_sub_number_;
}

bool UnitBaseClass::operator<(UnitBaseClass &other) const {
    const std::string &this_flightgroup_name = this->getFlightgroupName();
    const std::string &other_flightgroup_name = other.getFlightgroupName();
    if (this_flightgroup_name < other_flightgroup_name) {
        return true;
    } else if (this_flightgroup_name > other_flightgroup_name) {
        return false;
    } else {
        return this->getFlightgroupSubNumber() < other.getFlightgroupSubNumber();
    }
}

bool UnitBaseClass::operator<(const UnitBaseClass &other) const {
    const std::string &this_flightgroup_name = this->getFlightgroupName();
    const std::string &other_flightgroup_name = other.getFlightgroupName();
    if (this_flightgroup_name < other_flightgroup_name) {
        return true;
    } else if (this_flightgroup_name > other_flightgroup_name) {
        return false;
    } else {
        return this->getFlightgroupSubNumber() < other.getFlightgroupSubNumber();
    }
}
