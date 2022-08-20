/*
 * unit_base_class.hpp
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

#ifndef VEGA_STRIKE_SRC_CMD_UNIT_BASE_CLASS_HPP_
#define VEGA_STRIKE_SRC_CMD_UNIT_BASE_CLASS_HPP_

#include <string>
#include <cstdint>
#include <utility>
#include <iostream>
#include <boost/shared_ptr.hpp>

class UnitBaseClass {
protected:
    std::string flightgroup_name_{};
    int32_t flightgroup_sub_number_{};
public:
    // Default constructor -- forbidden
    UnitBaseClass() = delete;
    // Copy constructor -- forbidden
    UnitBaseClass(UnitBaseClass const & rhs) = delete;
    // Move constructor
    UnitBaseClass(UnitBaseClass&& rhs) = default;
    // Main constructor
    inline UnitBaseClass(std::string flightgroup_name, int32_t flightgroup_sub_number) : flightgroup_name_(std::move(flightgroup_name)), flightgroup_sub_number_(flightgroup_sub_number) {}
    // Destructor
    virtual ~UnitBaseClass() = default;

    UnitBaseClass& operator=(UnitBaseClass& other) = delete;
    virtual bool operator<(UnitBaseClass& other) const;
    virtual bool operator<(UnitBaseClass const& other) const;

    virtual std::string getFlightgroupName() const;
    virtual int32_t getFlightgroupSubNumber() const;

    virtual std::string toString();
};

inline std::ostream & operator<<(std::ostream & os, boost::shared_ptr<UnitBaseClass> unit_base_class_ptr) {
    os << unit_base_class_ptr->toString();
    return os;
}

#endif //VEGA_STRIKE_SRC_CMD_UNIT_BASE_CLASS_HPP_
