/*
 * unit_util.h
 *
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
 * and other Vega Strike Contributors
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


#ifndef __UNIT_UTIL_H__
#define __UNIT_UTIL_H__

#include <string>
#include "unit_generic.h"
#include "images.h"
using std::string;

namespace UnitUtil {
void setMissionRelevant(UnitPtr my_unit);
void unsetMissionRelevant(UnitPtr my_unit);
string getFactionName(UnitConstRawPtr my_unit);
int getFactionIndex(UnitConstRawPtr my_unit);
void setFactionIndex(UnitPtr my_unit, int factionname);
void RecomputeUnitUpgrades(UnitPtr my_unit);
void setFactionName(UnitPtr my_unit, string factionname);
float getFactionRelation(UnitConstRawPtr my_unit, UnitConstRawPtr their_unit);
float getRelationToFaction(UnitConstRawPtr my_unit, int other_faction);
float getRelationFromFaction(UnitConstRawPtr their_unit, int my_faction);
string getName(UnitConstRawPtr my_unit);
void setName(UnitPtr my_unit, string name);
void SetHull(UnitPtr my_unit, float hull);
string getFlightgroupName(boost::shared_ptr<Unit> my_unit);
const string &getFlightgroupNameCR(const boost::shared_ptr<Unit> my_unit);
UnitPtr getFlightgroupLeader(UnitPtr my_unit);
void orbit(UnitPtr my_unit, UnitPtr orbitee, float speed, QVector R, QVector S, QVector center);
bool setFlightgroupLeader(UnitPtr my_unit, UnitPtr un);
string getFgDirective(UnitConstRawPtr my_unit);
bool setFgDirective(UnitPtr my_unit, string inp);
int getPhysicsPriority(UnitPtr un);
int getFgSubnumber(UnitConstRawPtr my_unit);
int removeCargo(UnitPtr my_unit, string s, int quantity, bool erasezero);
bool repair(UnitPtr my_unit);
int removeWeapon(UnitPtr my_unit, string weapon, int mountoffset, bool loop_through_mounts); // -1 tells no weapon removed
float upgrade(UnitPtr my_unit, string file, int mountoffset, int subunitoffset, bool force, bool loop_through_mounts);
int addCargo(UnitPtr my_unit, Cargo carg);
int forceAddCargo(UnitPtr my_unit, Cargo carg);
bool incrementCargo(UnitPtr my_unit, float percentagechange, int quantity);
bool decrementCargo(UnitPtr my_unit, float percentagechange);
float getDistance(UnitConstRawPtr my_unit, UnitConstRawPtr un);
float getSignificantDistance(UnitConstRawPtr un, UnitConstRawPtr sig);
int hasCargo(UnitConstRawPtr my_unit, std::string mycarg);
Cargo GetCargoIndex(UnitConstRawPtr my_unit, int index);
Cargo GetCargo(UnitConstRawPtr my_unit, std::string cargname);
string getUnitSystemFile(UnitConstRawPtr my_unit);
float getCredits(UnitConstRawPtr my_unit);
void addCredits(UnitConstRawPtr my_unit, float credits);
bool isSignificant(UnitConstRawPtr my_unit);
bool isCloseEnoughToDock(UnitConstRawPtr my_unit, UnitConstRawPtr un);
bool isCapitalShip(UnitConstRawPtr my_unit);
bool isDockableUnit(UnitConstRawPtr my_unit);
bool isAsteroid(UnitConstRawPtr my_unit);
bool isSun(UnitPtr my_unit);
bool isSun(UnitRawPtr my_unit);
void switchFg(UnitPtr my_unit, string arg);
int communicateTo(UnitPtr my_unit, UnitPtr other_unit, float mood);
bool commAnimation(UnitPtr my_unit, string anim);
bool JumpTo(UnitPtr unit, string system);
int isPlayerStarship(UnitConstRawPtr un);
void setECM(UnitPtr un, int NewECM);  //short fix
int getECM(UnitConstRawPtr un); //short fix
void setSpeed(UnitPtr un, float speed);
boost::shared_ptr<Unit> owner(const UnitPtr un);
float maxSpeed(const UnitPtr un);
float maxAfterburnerSpeed(const UnitPtr un);
void performDockingOperations(UnitPtr un, UnitPtr unitToDockWith, int actuallyDockP);
float PercentOperational(UnitPtr un, string, string category, bool countHullAndArmorAsFull);
}

#endif
