/**
* faction_xml_server.cpp
*
* Copyright (c) 2001-2002 Daniel Horn
* Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
* Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors
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

#ifndef __FACTION_XML_SERVER_CPP__
#define __FACTION_XML_SERVER_CPP__

#include "root_generic/faction_generic.h"

//by chuck starchaser
/*
void Faction::LoadXML( const char *filename, char *xmlbuffer, int buflength )
{
}
*/
//Now, that is fast! :)

// Server crashes on startup with empty LoadXML! Factions are important.
#include "faction_xml.cpp"

#endif

