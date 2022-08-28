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

//#include "configuration/game_config.h"
//#include "configuration/configuration.h"
//#include "role_bitmask.h"
//#include "SharedPool.h"
//#include "collide_map.h"
//#include "vsfilesystem.h"
//#include "gfx/cockpit_generic.h"
//#include "star_system_generic.h"
//#include "faction_generic.h"
//#include "script/flightgroup.h"
//#include "unit_collection.hpp"
//#include "container.h"
//#include "xml_support.h"
//#include "gfxlib_struct.h"
//#include "gfx/quaternion.h"
//#include "gfx/matrix.h"
//#include "vega_cast_utils.hpp"
//#include <map>
//#include <set>
//#include "vs_globals.h"
//#include "vegastrike.h"
//#include "damage/damage.h"
//#include "mount.h"
//#include "jump_capable.h"
//#include "carrier.h"
//#include "energetic.h"
//#include "intelligent.h"
//#include "computer.h"
//#include "movable.h"
//#include "drawable.h"
//#include "damageable.h"
//#include "audible.h"
//#include "armed.h"
#include <string>
#include <cstdint>
#include <utility>
#include <iostream>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "vega_intrusive_ptr.hpp"
#include "vega_string_pool.hpp"

class Flightgroup;

static const char *const kLoadFailed = "LOAD_FAILED";
class UnitBaseClass : public boost::intrusive_ref_counter<UnitBaseClass, boost::thread_safe_counter> {
protected:
    std::string flightgroup_name_{};
    int32_t flightgroup_sub_number_{};
    VegaStringPtr filename;
//if the unit is a planet, this contains the long-name 'mars-station'
    std::string fullname;
public:
    // Default constructor
    UnitBaseClass() = default;
    // Copy constructor -- forbidden
    UnitBaseClass(UnitBaseClass const & rhs) = delete;
    // Move constructor
    UnitBaseClass(UnitBaseClass&& rhs) = default;
    // Main constructor
    inline UnitBaseClass(std::string flightgroup_name, int32_t flightgroup_sub_number) : flightgroup_name_(std::move(flightgroup_name)), flightgroup_sub_number_(flightgroup_sub_number) {}
    inline UnitBaseClass(boost::shared_ptr<std::string> flightgroup_name, int flightgroup_sub_number) : flightgroup_name_(*flightgroup_name), flightgroup_sub_number_(flightgroup_sub_number) {}
    // Destructor
    virtual ~UnitBaseClass() = default;

    UnitBaseClass& operator=(UnitBaseClass& other) = delete;
    virtual bool operator<(UnitBaseClass& other) const;
    virtual bool operator<(UnitBaseClass const& other) const;

    virtual std::string getFlightgroupName() const;
    virtual int32_t getFlightgroupSubNumber() const;

    virtual std::string toString() const;
    //get the full flightgroup ID (i.e 'green-00004')
    virtual std::string getFgID() const;
    virtual void SetFaction(int new_faction);
    void SetFg(Flightgroup *fg, int fg_snumber);
    //the flightgroup this ship is in
    Flightgroup *flightgroup = nullptr;
    //the flightgroup subnumber
    int32_t flightgroup_subnumber = 0;
    //The faction of this unit
    int faction = 0;

    //get the flightgroup description
    Flightgroup *getFlightgroup() const {
        return flightgroup;
    }

    //get the subnumber
    int32_t getFgSubnumber() const {
        return flightgroup_subnumber;
    }

    void setFullname(std::string new_name) {
        fullname = new_name;
    }

    const std::string &getFullname() const {
        return fullname;
    }

    const std::string &getFilename() const {
        return *filename.get();
    }

    virtual std::string getName() const;
    virtual void setName(std::string new_name);

    bool failedToLoad() const;
    bool loadedSuccessfully() const;
    void setFailedToLoad();

    // The name (type) of this unit shouldn't be public
    VegaStringPtr name;
};

inline std::ostream & operator<<(std::ostream & os, boost::shared_ptr<UnitBaseClass> unit_base_class_ptr) {
    os << unit_base_class_ptr->toString();
    return os;
}

#endif //VEGA_STRIKE_SRC_CMD_UNIT_BASE_CLASS_HPP_
