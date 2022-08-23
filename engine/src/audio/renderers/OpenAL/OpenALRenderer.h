/**
 * OpenALRenderer.h
 *
 * Copyright (C) Daniel Horn
 * Copyright (C) 2020 pyramid3d, Stephen G. Tuggy, and other Vega Strike
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


//
// C++ Interface: Audio::SceneManager
//
#ifndef __AUDIO_OPENAL_RENDERER_H__INCLUDED__
#define __AUDIO_OPENAL_RENDERER_H__INCLUDED__

#include "../../Exceptions.h"
#include "../../Types.h"
#include "../../Renderer.h"
#include "../../Format.h"

namespace Audio {

namespace __impl {

namespace OpenAL {
// Forward declaration of internal renderer data
struct RendererData;
};

};

/**
 * OpenAL Renderer implementation
 *
 * @remarks Audio renderer implementation based on OpenAL.
 *
 */
class OpenALRenderer : public Renderer {
protected:
    AutoPtr<__impl::OpenAL::RendererData> data;

public:
    /** Initialize the renderer with default or config-driven settings. */
    OpenALRenderer();

    ~OpenALRenderer() override;

    /** @copydoc Renderer::getSound */
    SharedPtr<Sound> getSound(
            const std::string &name,
            VSFileSystem::VSFileType type = VSFileSystem::UnknownFile,
            bool streaming = false) override;

    /** @copydoc Renderer::owns */
    bool owns(SharedPtr<Sound> sound) override;

    /** @copydoc Renderer::attach(SharedPtr<Source>) */
    void attach(SharedPtr<Source> source) override;

    /** @copydoc Renderer::attach(SharedPtr<Listener>) */
    void attach(SharedPtr<Listener> listener) override;

    /** @copydoc Renderer::detach(SharedPtr<Source>) */
    void detach(SharedPtr<Source> source) override;

    /** @copydoc Renderer::detach(SharedPtr<Listener>) */
    void detach(SharedPtr<Listener> listener) override;

    /** @copydoc Renderer::setMeterDistance */
    void setMeterDistance(Scalar distance) override;

    /** @copydoc Renderer::setDopplerFactor */
    void setDopplerFactor(Scalar factor) override;

    /** @copydoc Renderer::setOutputFormat */
    void setOutputFormat(const Format &format) override;

    /** @copydoc Renderer::beginTransaction */
    void beginTransaction() override;

    /** @copydoc Renderer::commitTransaction */
    void commitTransaction() override;
protected:

    /** Makes sure the AL context is valid, creating one if necessary */
    virtual void checkContext();

    /** Sets expected defaults into the context */
    virtual void initContext();

    /** Sets doppler effect globals into the context */
    void setupDopplerEffect();
};

};

#endif//__AUDIO_OPENAL_RENDERER_H__INCLUDED__
