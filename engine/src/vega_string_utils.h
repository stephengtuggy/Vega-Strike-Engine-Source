/*
 * vega_string_utils.h
 *
 * Copyright (C) 2001-2023 Daniel Horn, Stephen G. Tuggy, Benjamen R. Meyer,
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
#ifndef VEGA_STRIKE_ENGINE_VEGA_STRING_UTILS_H
#define VEGA_STRIKE_ENGINE_VEGA_STRING_UTILS_H

#include "src/vs_logging.h"
#include "src/vs_exit.h"

inline char *vega_str_dup(const char *string) {
#if _XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200809L
    return strdup(string);
#elif defined (_WINDOWS)
    return _strdup(string);
#else //_XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200809L
    size_t buf_size = strlen(string) + 1;
    char *alloc;
    alloc = (char *)malloc(buf_size);
    if (alloc == nullptr)
    {
        VS_LOG_AND_FLUSH(fatal, "Out of memory");
        VSExit(-1);
    }
    strncpy(alloc, string, buf_size);
    alloc[buf_size - 1] = '\0';
    return alloc;
#endif //_XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200809L
}

#endif //VEGA_STRIKE_ENGINE_VEGA_STRING_UTILS_H
