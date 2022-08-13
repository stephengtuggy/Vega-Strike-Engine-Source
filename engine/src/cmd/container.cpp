/*
 * container.cpp
 *
 * Copyright (c) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
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


#include <stdlib.h>
#include "container.h"
#include "unit_generic.h"

UnitContainer::UnitContainer() : unit(nullptr) {
    VSCONSTRUCT1('U')
}

UnitContainer::UnitContainer(UnitPtr un) : unit(nullptr) {
    SetUnit(un);
    VSCONSTRUCT1('U');
}

UnitContainer::~UnitContainer() {
    VSDESTRUCT1
    if (unit) {
        unit->UnRef();
    }
    //bad idea...arrgh!
}

void UnitContainer::SetUnit(UnitPtr un) {
    //if the unit is null then go here otherwise if the unit is killed then go here
    if (un != NULL ? un->Killed() == true : true) {
        if (unit) {
            unit->UnRef();
        }
        unit = NULL;
        return;
    } else {
        if (unit) {
            unit->UnRef();
        }
        unit = un;
        unit->Ref();
    }
}

