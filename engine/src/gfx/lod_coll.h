/*
 * lod_coll.h
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
#ifndef VEGA_STRIKE_ENGINE_GFX_LOD_COLL_H
#define VEGA_STRIKE_ENGINE_GFX_LOD_COLL_H

#include "mesh.h"

namespace vega_gfx {

/**
* Collection of Levels of Detail (LODs)
*/
class LodColl: public std::enable_shared_from_this<LodColl> {
private:
    struct Private { explicit Private() = default; };

    std::string hash_name_{};
    std::shared_ptr<Mesh> original_mesh_{};
    std::map<float, std::shared_ptr<Mesh>> levels_of_detail_{};
    int faction_{};
    std::shared_ptr<Flightgroup> flightgroup_{};
    std::vector<std::string> override_textures_{};

public:
    explicit LodColl(Private) {}
    LodColl(const LodColl&) = delete;
    LodColl& operator=(const LodColl&) = delete;
    LodColl(LodColl&&) noexcept;
    LodColl& operator=(LodColl&&) noexcept;

    static std::shared_ptr<LodColl> Create(const std::string& hash_name, std::shared_ptr<Mesh> original);

    size_t NumberOfLODs() const;

private:
    void LoadMesh(const std::string& hash_name, std::shared_ptr<Mesh> original);
};

} // vega_gfx

#endif //VEGA_STRIKE_ENGINE_GFX_LOD_COLL_H
