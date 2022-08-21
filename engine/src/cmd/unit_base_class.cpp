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

#include <iostream>
#include "movable.h"
#include "pilot.h"
#include "unit_find.h"
#include <cstdint>
#include <list>
#include <math.h>
#include "base_util.h"
#include "resource/resource.h"
#include "configuration/game_config.h"
#include "energetic.h"
#include "turret.h"
#include "mount_size.h"
#include "weapon_info.h"
#include "universe.h"
#include "star_system.h"
#include "options.h"
#include "gfx/camera.h"
#include "galaxy_xml.h"
#include "vs_random.h"
#include "csv.h"
#include "gfx/warptrail.h"
#include "unit_const_cache.h"
#include "role_bitmask.h"
#include "cmd/ai/ikarus.h"
#include <algorithm>
#include "gfx/vsbox.h"
#include "gfx/cockpit_generic.h"
#include "missile.h"
#include "python/python_class.h"
#include "cmd/ai/aggressive.h"
#include "cmd/ai/flybywire.h"
#include "cmd/ai/missionscript.h"
#include "cmd/ai/script.h"
#include "cmd/ai/navigation.h"
#include "cmd/ai/communication.h"
#include "cmd/ai/turretai.h"
#include "cmd/ai/fire.h"
#include "script/flightgroup.h"
#include "script/mission.h"
#include "unit_util.h"
#include "universe_util.h"
#include "file_main.h"
#include "vs_logging.h"
#include "vsfilesystem.h"
#include "xml_serializer.h"
#include "lin_time.h"
#include "beam.h"
#include "music.h"
#include "base.h"
#include "audiolib.h"
#include "configxml.h"
#include <set>
#include "unit_generic.h"
#include "unit_base_class.hpp"
#include <boost/format.hpp>

std::string UnitBaseClass::toString() {
    return getFgID();
}

//std::string UnitBaseClass::getFlightgroupName() const {
//    return flightgroup_name_;
//}

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

const std::string UnitBaseClass::getFgID() {
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
