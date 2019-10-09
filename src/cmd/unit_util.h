#ifndef __UNIT_UTIL_H__
#define __UNIT_UTIL_H__
#include <string>
#include "unit_generic.h"
#include "images.h"
using std::string;

namespace UnitUtil
{
void          setMissionRelevant(std::shared_ptr<Unit> my_unit);
void          unsetMissionRelevant(std::shared_ptr<Unit> my_unit);
string        getFactionName(const std::shared_ptr<Unit> my_unit);
int           getFactionIndex(const std::shared_ptr<Unit> my_unit);
void          setFactionIndex(std::shared_ptr<Unit> my_unit, int factionname);
void          RecomputeUnitUpgrades(std::shared_ptr<Unit> my_unit);
void          setFactionName(std::shared_ptr<Unit> my_unit, string factionname);
float         getFactionRelation(const std::shared_ptr<Unit> my_unit, const std::shared_ptr<Unit> their_unit);
float         getRelationToFaction(const std::shared_ptr<Unit> my_unit, int other_faction);
float         getRelationFromFaction(const std::shared_ptr<Unit> their_unit, int my_faction);
string        getName(const std::shared_ptr<Unit> my_unit);
void          setName(std::shared_ptr<Unit> my_unit, string name);
void          SetHull(std::shared_ptr<Unit> my_unit, float hull);
string        getFlightgroupName(const std::shared_ptr<Unit> my_unit);
const string &getFlightgroupNameCR(const std::shared_ptr<Unit> my_unit);
std::shared_ptr<Unit> getFlightgroupLeader(std::shared_ptr<Unit> my_unit);
void          orbit(std::shared_ptr<Unit> my_unit, std::shared_ptr<Unit> orbitee, float speed, QVector R, QVector S, QVector center);
bool          setFlightgroupLeader(std::shared_ptr<Unit> my_unit, std::shared_ptr<Unit> un);
string        getFgDirective(const std::shared_ptr<Unit> my_unit);
bool          setFgDirective(std::shared_ptr<Unit> my_unit, string inp);
int           getPhysicsPriority(std::shared_ptr<Unit> un);
int           getFgSubnumber(const std::shared_ptr<Unit> my_unit);
int           removeCargo(std::shared_ptr<Unit> my_unit, string s, int quantity, bool erasezero);
bool          repair(std::shared_ptr<Unit> my_unit);
int           removeWeapon(std::shared_ptr<Unit> my_unit, string weapon, int mountoffset, bool loop_through_mounts); // -1 tells no weapon removed
float         upgrade(std::shared_ptr<Unit> my_unit, string file, int mountoffset, int subunitoffset, bool force, bool loop_through_mounts);
int           addCargo(std::shared_ptr<Unit> my_unit, Cargo carg);
int           forceAddCargo(std::shared_ptr<Unit> my_unit, Cargo carg);
bool          incrementCargo(std::shared_ptr<Unit> my_unit, float percentagechange, int quantity);
bool          decrementCargo(std::shared_ptr<Unit> my_unit, float percentagechange);
float         getDistance(const std::shared_ptr<Unit> my_unit, const std::shared_ptr<Unit> un);
float         getSignificantDistance(const std::shared_ptr<Unit> un, const std::shared_ptr<Unit> sig);
int           hasCargo(const std::shared_ptr<Unit> my_unit, std::string mycarg);
Cargo         GetCargoIndex(const std::shared_ptr<Unit> my_unit, int index);
Cargo         GetCargo(const std::shared_ptr<Unit> my_unit, std::string cargname);
string        getUnitSystemFile(const std::shared_ptr<Unit> my_unit);
float         getCredits(const std::shared_ptr<Unit> my_unit);
void          addCredits(const std::shared_ptr<Unit> my_unit, float credits);
bool          isSignificant(const std::shared_ptr<Unit> my_unit);
bool          isCloseEnoughToDock(const std::shared_ptr<Unit> my_unit, const std::shared_ptr<Unit> un);
bool          isCapitalShip(const std::shared_ptr<Unit> my_unit);
bool          isDockableUnit(const std::shared_ptr<Unit> my_unit);
bool          isAsteroid(const std::shared_ptr<Unit> my_unit);
bool          isSun(const std::shared_ptr<Unit> my_unit);
void          switchFg(std::shared_ptr<Unit> my_unit, string arg);
int           communicateTo(std::shared_ptr<Unit> my_unit, std::shared_ptr<Unit> other_unit, float mood);
bool          commAnimation(std::shared_ptr<Unit> my_unit, string anim);
bool          JumpTo(std::shared_ptr<Unit> unit, string system);
int           isPlayerStarship(const std::shared_ptr<Unit> un);
void          setECM(std::shared_ptr<Unit> un, int NewECM); // short fix
int           getECM(const std::shared_ptr<Unit> un);       // short fix
void          setSpeed(std::shared_ptr<Unit> un, float speed);
std::shared_ptr<Unit> owner(const std::shared_ptr<Unit> un);
float         maxSpeed(const std::shared_ptr<Unit> un);
float         maxAfterburnerSpeed(const std::shared_ptr<Unit> un);
void          performDockingOperations(std::shared_ptr<Unit> un, std::shared_ptr<Unit> unitToDockWith, int actuallyDockP);
float         PercentOperational(std::shared_ptr<Unit> un, string, string category, bool countHullAndArmorAsFull);
} // namespace UnitUtil

#endif
