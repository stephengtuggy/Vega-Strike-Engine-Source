/*
 * Copyright (C) 2001-2022 Daniel Horn, Alan Shieh, pyramid3d,
 * Stephen G. Tuggy, and other Vega Strike contributors.
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
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INHANDLER_H
#define INHANDLER_H
#include "in_kb.h"
//#include "cmd_unit.h"
class Unit;

typedef void (*INDISPATCH)(int x, int y);

class InputListener {
public:
    int *mousex, *mousey;
    KBSTATE( *keystate )[KEYMAP_SIZE];

    UnitPtr parent;

    InputListener(UnitPtr parent) {
        this->parent = parent;
        //int a;
        //mousex = mousey = 0;
        //ZeroMemory(keystate, sizeof(keystate));
    }

    virtual void MoveMouse(int x, int y) {
        //mousex = x;
        //mousey = y;
    }

    virtual void MoveMouse(int *x, int *y, int num) {
    }

    virtual void KeyDown(int key) {
        //keystate[key] = 1;
    }

    virtual void KeyDown(int *key, int num) {
    }

    virtual void KeyUp(int key) {
        //keystate[key] = 0;
    }

    virtual void KeyUp(int *key, int num) {
    }

    virtual void Activate() {
    }

    virtual void Deactivate() {
    }

//InHandler &Dispatch(); // Drives the input dispatcher, for switching input states
//InHandler &Revert(); // return to the previous state
};

//const InputListener End;
#endif

