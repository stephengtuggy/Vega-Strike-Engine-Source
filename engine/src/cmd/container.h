/*
 * container.h
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy
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


#ifndef _UNITCONTAINER_H_
#define _UNITCONTAINER_H_

#include "debug_vs.h"

class Unit;

class UnitContainer {
protected:
    UnitPtr unit;
public:
    UnitContainer();
    UnitContainer(UnitPtr);

    UnitContainer(const UnitContainer &un) {
        VSCONSTRUCT1('U')
        unit = 0;
        SetUnit(un.unit);
    }

    const UnitContainer &operator=(const UnitContainer &a) {
        SetUnit(a.unit);
        return a;
    }

    bool operator==(const UnitPtr oth) const {
        return unit == oth;
    }

    bool operator!=(const UnitPtr oth) const {
        return unit != oth;
    }

    bool operator==(const UnitContainer &oth) const {
        return unit == oth.unit;
    }

    bool operator!=(const UnitContainer &oth) const {
        return unit != oth.unit;
    }

    ~UnitContainer();
    void SetUnit(UnitPtr);
    UnitPtr GetUnit();

    const UnitPtr GetConstUnit() const {
        return unit;
    }
};

#endif

