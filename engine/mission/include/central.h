/*
 * Copyright (C) 2001-2023 Daniel Horn, David Ranger, pyramid3d, Stephen G. Tuggy, Benjamen R. Meyer,
 * and other Vega Strike contributors.
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
#ifndef VEGA_STRIKE_ENGINE_MISSION_CENTRAL_H
#define VEGA_STRIKE_ENGINE_MISSION_CENTRAL_H

#define MISSION_VERSION "1.0.0.1"
#define MISSION_PROGRAM "Vega Strike"

#include <expat.h>
#include <glob.h>
#include <gtk/gtk.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "mission/include/general.h"
#include "mission/include/display_gtk.h"
#include "mission/include/file.h"
#include "mission/include/easydom.h"
#include "mission/include/xml_support.h"
#include "mission/include/hashtable.h"

#ifndef MAX_READ
#define MAX_READ 1024	// Maximum number of characters to read from a line
#endif

#define EXT_MISSION "*.mission"
#define EXT_CAMPAIGN "*.campaign"

#define DIR_MISSION "./mission/"
char *Start(int run_vegastrike);
void RunMission(void);

struct mission_data {
    char *name;
    char *author;
    char *description;
    char *briefing;
    char *path;
    char *numplayers;
};

extern glob_t *MISSIONS;
extern struct mission_data DATA;

#endif //VEGA_STRIKE_ENGINE_MISSION_CENTRAL_H
