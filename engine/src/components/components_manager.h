/*
 * components_manager.h
 *
 * Copyright (c) 2001-2002 Daniel Horn
 * Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
 * Copyright (c) 2019-2023 Stephen G. Tuggy, Benjamen R. Meyer, Roy Falk and other Vega Strike Contributors
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

// -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef VEGA_STRIKE_ENGINE_COMPONENTS_MANAGER_COMPONENT_H
#define VEGA_STRIKE_ENGINE_COMPONENTS_MANAGER_COMPONENT_H

#include "energy_container.h"
#include "reactor.h"

#include "afterburner.h"
#include "afterburner_upgrade.h"
#include "drive.h"
#include "drive_upgrade.h"

#include "ftl_drive.h"
#include "jump_drive.h"

#include "armor.h"
#include "hull.h"
#include "shield.h"

#include "computer.h"
#include "radar.h"
#include "cloak.h"
#include "ecm.h"
#include "repair_bot.h"
#include "ship_functions.h"


class ComponentsManager {
public:
    virtual ~ComponentsManager() = default;
// Components
    EnergyContainer fuel = EnergyContainer(ComponentType::Fuel);
    EnergyContainer energy = EnergyContainer(ComponentType::Capacitor);
    EnergyContainer ftl_energy = EnergyContainer(ComponentType::FtlCapacitor);

    // TODO: move this to a single constructor?!
    Reactor reactor = Reactor(&fuel, &energy, &ftl_energy);

    Afterburner afterburner;
    AfterburnerUpgrade afterburner_upgrade = AfterburnerUpgrade(&afterburner);
    Cloak cloak = Cloak();
    Drive drive;
    DriveUpgrade drive_upgrade = DriveUpgrade(&drive);
    FtlDrive ftl_drive = FtlDrive(&ftl_energy);
    JumpDrive jump_drive = JumpDrive(&ftl_energy);
    CRadar radar;

    Armor armor;
    Hull hull;
    Shield shield = Shield(&energy, &ftl_drive, &cloak);

    Computer computer;
    ECM ecm;
    RepairBot repair_bot;
    ShipFunctions ship_functions;

    void DamageRandomSystem();
};

#endif // VEGA_STRIKE_ENGINE_COMPONENTS_MANAGER_COMPONENT_H