/*
 * lod_coll.cpp
 *
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Daniel Horn, project creator
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
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

#include "lod_coll.h"

namespace vega_gfx {

size_t LodColl::NumberOfLODs() const {
    return levels_of_detail_.size();
}

LodColl::LodColl(LodColl && other) noexcept : hash_name_{std::move(other.hash_name_)},
                                              original_mesh_{std::move(other.original_mesh_)},
                                              levels_of_detail_{std::move(other.levels_of_detail_)},
                                              faction_{other.faction_},
                                              flightgroup_{std::move(other.flightgroup_)},
                                              override_textures_{std::move(other.override_textures_)}
                                              {}

LodColl & LodColl::operator=(LodColl && other) noexcept {
    hash_name_ = std::move(other.hash_name_);
    original_mesh_ = std::move(other.original_mesh_);
    levels_of_detail_ = std::move(other.levels_of_detail_);
    faction_ = other.faction_;
    flightgroup_ = std::move(other.flightgroup_);
    override_textures_ = std::move(other.override_textures_);
    return *this;
}

std::shared_ptr<LodColl> LodColl::Create(const std::string &filename, const int faction, Flightgroup *fg, const std::vector<std::string> &override_texture) {
    std::shared_ptr<LodColl> return_value = std::make_shared<LodColl>(Private());

    // return_value->LoadMesh(filename, original);
    return return_value;
}

// void LodColl::LoadMesh(const std::string &hash_name, std::shared_ptr<Mesh> original) {
//     this->hash_name_ = hash_name;
//     this->original_mesh_ = original;
//     // ...
// }

} // vega_gfx
