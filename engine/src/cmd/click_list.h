/**
 * click_list.h
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors
 * Copyright (C) 2022-2023 Stephen G. Tuggy, Benjamen R. Meyer
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
#ifndef VEGA_STRIKE_ENGINE_CMD_GFX_CLICK_LIST_H
#define VEGA_STRIKE_ENGINE_CMD_GFX_CLICK_LIST_H

//#include "src/vegastrike.h"
#include "gfx_generic/matrix.h"
#include "cmd/collection.h"
#include "src/star_system.h"
Vector MouseCoordinate(int x, int y); //FIXME

class ClickList {
private:
    UnitCollection *parentIter;
    StarSystem *parentSystem;
    UnitCollection *lastCollection;
    Unit *lastSelected;
public:
//gets passed in unnormalized mouse values btw 0 and g_game.x_resolution&& g_game.y_resolution
    bool queryShip(int mouseX, int mouseY, Unit *); //returns if the ship's in iterator utilizes
    ClickList(StarSystem *parSystem, UnitCollection *parentIter);

    ~ClickList() {
    }

    UnitCollection *requestIterator(int mouseX, int mouseY);
    UnitCollection *requestIterator(int minX, int minY, int maxX, int maxY);
    Unit *requestShip(int mouseX, int mouseY);
};
#endif //VEGA_STRIKE_ENGINE_CMD_GFX_CLICK_LIST_H
