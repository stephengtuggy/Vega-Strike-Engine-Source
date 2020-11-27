/**
 * flightgroup.cpp
 *
 * Copyright (C) Daniel Horn
 * Copyright (C) 2020 pyramid3d, Stephen G. Tuggy, and other Vega Strike
 * contributors
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


#include "gfx/aux_texture.h"
#include "mission.h"
#include "flightgroup.h"
#include "cmd/unit_generic.h"
#include "vsfilesystem.h"

Flightgroup* Flightgroup::newFlightgroup( const std::string &name,
                                          const std::string &type,
                                          const std::string &faction,
                                          const std::string &order,
                                          int num_ships,
                                          int num_waves,
                                          const std::string &logo_tex,
                                          const std::string &logo_alp,
                                          Mission *mis )
{
    Flightgroup *fg    = mis->findFlightgroup( name, faction );
    Flightgroup *fgtmp = fg;
    if (fg == nullptr) {
        fg = new Flightgroup;
    }
    fg->Init( fgtmp, name, type, faction, order, num_ships, num_waves, mis );
    if ( !logo_tex.empty() ) {
        if ( logo_alp.empty() ) {
            fg->squadLogo = new Texture( logo_tex.c_str(), 0, MIPMAP );
        } else {
            fg->squadLogo = new Texture( logo_tex.c_str(), logo_alp.c_str(), 0, MIPMAP );
        }
    }
    return fg;
}
Flightgroup::~Flightgroup()
{
    if (squadLogo) {
        delete squadLogo;
    }
}

Flightgroup&Flightgroup::operator=( Flightgroup &other )
{
    BOOST_LOG_TRIVIAL(warning) << "warning: may not work properly";
    if (squadLogo) {
        squadLogo = other.squadLogo->Clone();
    }
    return other;
}

