##
# MacPorts.cmake
#
# Vega Strike - Space Simulation, Combat and Trading
# Copyright (C) 2001-2026 The Vega Strike Contributors:
# Project creator: Daniel Horn
# Original development team: As listed in the AUTHORS file
# Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
#
# https://github.com/vegastrike/Vega-Strike-Engine-Source
#
# This file is part of Vega Strike.
#
# Vega Strike is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Vega Strike is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
#

SET(CMAKE_PREFIX_PATH "/opt/local")
SET(Boost_INCLUDE_DIR "/opt/local/libexec/boost/1.87/include" CACHE STRING "Boost install directory")
SET(Boost_DIR "/opt/local/libexec/boost/1.87/lib/cmake/Boost-1.87.0" CACHE STRING "Boost root directory")
SET(CMAKE_FIND_FRAMEWORK "LAST")
SET(CMAKE_FIND_APPBUNDLE "LAST")

SET(INSTALL_GTEST ON CACHE BOOL "Should we download and install GTest and GMock using FetchContent_MakeAvailable?" FORCE)
SET(USE_GTEST ON CACHE BOOL "Should we build and run the unit tests using GTest?" FORCE)
SET(INSTALL_SDL3 OFF CACHE BOOL "Should we download and install SDL3 using FetchContent_MakeAvailable?" FORCE)
SET(INSTALL_SDL3_IMAGE OFF CACHE BOOL "Should we download and install SDL3_image using FetchContent_MakeAvailable?" FORCE)

# Is this correct?
SET(MacPortsLibArchivePrefix CACHE "/opt/local/libarchive/")

SET(LibArchive_INCLUDE_DIR "${MacPortsLibArchivePrefix}/include")
SET(LibArchive_LIBRARY "${MacPortsLibArchivePrefix}/lib")
