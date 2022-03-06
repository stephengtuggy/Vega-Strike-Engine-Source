/*
 * Copyright (C) 2001-2022 Daniel Horn, Konstantinos Arvanitis,
 * pyramid3d, Stephen G. Tuggy, and other Vega Strike contributors.
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
#ifndef VSR_TOOLS_H_
#define VSR_TOOLS_H_
#include <stdint.h>
/** The standard header found in all Vega Strike resource package files. */
struct VSRHeader {
    char magic[4];  ///Must always be 'V' 'S' 'R' '\0'
    uint32_t entries; ///The number of files in this package.
    uint32_t entryTableOffset;  ///The offset of the entry table in this file.
};

/// A single entry in the Vega Strike resource package file table of entries.
struct VSRPEntry {
    uint32_t fileLength; ///The total length of this file.
    uint32_t offset;    ///The offset this file starts in the package.
    char filename[256];    ///The complete path to the file, terminating with '\0'.
};
#endif //VSR_TOOLS_H_
