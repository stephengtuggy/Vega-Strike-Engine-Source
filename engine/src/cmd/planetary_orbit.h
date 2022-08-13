/**
 * planetary_orbit.h
 *
 * Copyright (C) Daniel Horn
 * Copyright (C) 2020 Roy Falk, Stephen G. Tuggy, and other Vega Strike
 * contributors
 * Copyright (C) 2022 Stephen G. Tuggy
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


#ifndef PLANETARY_ORBIT_H
#define PLANETARY_ORBIT_H

#include "gfx/vec.h"
#include "star_system.h"
#include "ai/order.h"

class PlanetaryOrbit : public Order {
private:
    double velocity;
    double theta;
    double inittheta;

    QVector x_size;
    QVector y_size;
    QVector focus;
#define ORBIT_PRIORITY 8
#define NUM_ORBIT_AVERAGE (SIM_QUEUE_SIZE/ORBIT_PRIORITY)
    QVector orbiting_average[NUM_ORBIT_AVERAGE];
    float orbiting_last_simatom;
    int current_orbit_frame;
    bool orbit_list_filled;
protected:
///A vector containing all lihgts currently activated on current planet
    std::vector<int> lights;

public:
    PlanetaryOrbit(UnitPtr p,
            double velocity,
            double initpos,
            const QVector &x_axis,
            const QVector &y_axis,
            const QVector &Centre,
            UnitPtr target = NULL);
    ~PlanetaryOrbit();
    void Execute();
};

#endif // PLANETARY_ORBIT_H
