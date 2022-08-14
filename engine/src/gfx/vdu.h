/*
 * vdu.h
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


#ifndef _GFX_VDU_H_
#define _GFX_VDU_H_

#include "cmd/unit_generic.h"
#include "sprite.h"
#include <string>
#include <vector>

class Unit;
class TextPlane;

void DrawObjectivesTextPlane(TextPlane *tp, int scrolloffset, UnitPtr parent);

/**
 * Holds information for dedicated computer output
 * on a fighter craft. Can print out target info, local damage info
 * Local weapon info or Nav info.
 */
class GameCockpit;
class VDU : public VSSprite {
private:
    class Animation *comm_ani;
    UnitContainer communicating;
    class Animation *webcam;
    VIEWSTYLE viewStyle;
    float *StartArmor;
    float *maxhull;
///Holds a pointer to the font used to render text in the vdu
    TextPlane *tp;
///potential modes this computer is equipped for
    unsigned int posmodes;
///The current mode (VDU_MODE) that this unit is in is in back of this vector
    std::vector<unsigned int> thismode;
///Rows and columns of text this VDU can display
    short rows, cols;
    bool got_target_info;
//We received a packet with taret data by network : we update then the concerned UNIT
//DrawVDUObjective is outdated
//int DrawVDUObjective (void * obj, int offset);//obj is of type Mission::Objective
    bool SetWebcamAnimation();
    void DrawWebcam(UnitPtr parent);
///Draws all of the objectives created by missions.
    void DrawVDUObjectives(UnitPtr parent);
///Draws a shield display in the VDU
    void DrawVDUShield(UnitPtr parent);
///Draws the target display of UnitPtr target (only call with nonNULL)
    void DrawTarget(GameCockpit *parentcp, UnitPtr parent, UnitPtr target);
///Draws a list of cargo contents
    void DrawManifest(UnitPtr parent, UnitPtr target);
///Draws the message from the messagecenter
    void DrawMessages(GameCockpit *parentcp, UnitPtr target);
    void DrawScanningMessage();
///Draws the nav point this unit is heading to
    void DrawNav(GameCockpit *parentcp, UnitPtr parent, UnitPtr target, const Vector &navPoint);
///Draws the comm screen this unit can use to communicate
    void DrawComm();
///Draws a "scanning target" message
    void DrawSanningMessage();
///Draws the damage this unit has sustained
    void DrawDamage(UnitPtr parent);
///Draws the weapons activated on current unit
    void DrawWeapon(UnitPtr parent);
///Draws the sprite representing the current starship. Returns proper location and size for future use (with weaps and damage display)
    void DrawTargetSpr(VSSprite *s, float percent, float &x, float &y, float &w, float &h);
///draws the target camera
    void DrawStarSystemAgain(float x, float y, float w, float h, VIEWSTYLE viewStyle, UnitPtr parent, UnitPtr target);
public:
    void ReceivedTargetData() {
        got_target_info = true;
    }

    bool staticable() const;

    unsigned int getMode() const {
        return thismode.back();
    }

    int scrolloffset;
///Alert! To add a mode must change const array on the bottom. VIEW must remain LAST
    enum VDU_MODE {
        TARGET = 0x1,
        NAV = 0x2,
        OBJECTIVES = 0x4,
        COMM = 0x8,
        WEAPON = 0x10,
        DAMAGE = 0x20,
        SHIELD = 0x40,
        MANIFEST = 0x80,
        TARGETMANIFEST = 0x100,
        VIEW = 0x200,
        MSG = 0x400,
        SCANNING = 0x800,
        NETWORK = 0x1000,
        WEBCAM = 0x2000
    };
    VDU(const char *file,
            TextPlane *textp,
            unsigned short modes,
            short rows,
            short cols,
            float *MaxArmor,
            float *maxhull);
///Draws the entire VDU, all data, etc
    void Draw(GameCockpit *parentcp, UnitPtr parent, const GFXColor &c);
///Changes the mode of the current VDU to another legal mode
    void SwitchMode(UnitPtr parent);
    void SetViewingStyle(VIEWSTYLE vm);
    void Scroll(int howmuch);
    bool SetCommAnimation(Animation *ani, UnitPtr unit, bool force);
    UnitPtr GetCommunicating();
    bool CheckCommAnimation(UnitPtr un) const; //returns true if unit is talkin or uninterruptable going on
};

int parse_vdu_type(const char *s);

GFXColor MountColor(Mount *mnt);
#endif

