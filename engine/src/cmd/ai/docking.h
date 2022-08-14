/*
 * docking.h
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


#include "navigation.h"
namespace Orders {
class DockingOps : public MoveTo {
    UnitContainer docking;
    void *formerOwnerDoNotDereference;
    enum { GETCLEARENCE, DOCKING, DOCKED, UNDOCKING } state;
    Order *oldstate;
    float timer;
    int port;
    bool physicallyDock;
    bool facedtarget;
    bool keeptrying;
public:
    QVector Movement(UnitPtr utdw);
    DockingOps(UnitPtr unitToDockWith, Order *oldstate, bool physicallyDock, bool keeptrying);
    virtual void SetParent(UnitPtr par);
    void Execute();
    bool RequestClearence(UnitPtr);
    bool DockToTarget(UnitPtr);
    bool PerformDockingOperations(UnitPtr);
    bool Undock(UnitPtr);
    virtual void Destroy();
    void RestoreOldAI();
};
}

