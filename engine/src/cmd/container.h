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
//#include "unit_base_class.hpp"
#include "unit_fwd_decl.hpp"
//#include "vega_intrusive_ptr.hpp"
#include "unit_generic.h"

class UnitContainer {
protected:
    UnitSharedPtr unit;
public:
    UnitContainer();
    explicit UnitContainer(UnitRawPtr un);

    UnitContainer(const UnitContainer &un) {
        VSCONSTRUCT1('U')
        //unit = nullptr;
        unit.reset();
        SetUnit(un.unit);
    }

    UnitContainer &operator=(const UnitContainer &a) {
        SetUnit(a.unit);
        return *this;
    }

    bool operator==(UnitConstRawPtr oth) const {
        return unit.get() == oth;
    }

    bool operator!=(UnitConstRawPtr oth) const {
        return unit.get() != oth;
    }

    bool operator==(const UnitContainer &oth) const {
        return unit == oth.unit;
    }

    bool operator!=(const UnitContainer &oth) const {
        return unit != oth.unit;
    }

    virtual ~UnitContainer();
    void SetUnit(Unit * un);
    void SetUnit(UnitSharedPtr un);
    void SetUnit(UnitWeakPtr un);
    UnitRawPtr GetUnit() const;

    UnitConstRawPtr GetConstUnit() const {
        return unit.get();
    }

    UnitWeakPtr getWeakUnitPtr() {
        return UnitWeakPtr(unit);
    }

    UnitSharedPtr getSharedUnitPtr() {
        return unit;
    }

    virtual void setNull() {
        if (unit) {
            unit.reset();
        }
    }
};

#endif

