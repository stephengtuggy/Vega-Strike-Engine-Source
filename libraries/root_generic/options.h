/*
 * options.h
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
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
#ifndef VEGA_STRIKE_ENGINE_OPTIONS_H
#define VEGA_STRIKE_ENGINE_OPTIONS_H

#include <string>
#include <memory>

#ifndef uint
typedef unsigned int uint;
#endif

class vs_options {
public:
    vs_options() {
    }

    ~vs_options() {
    }

    void init();

    /* Audio Options */
    int threadtime{};
    std::string missionvictorysong;
    bool Music{};
    float sound_gain{};
    float audio_ref_distance{};
    float audio_max_distance{};
    bool Doppler{};
    bool Positional{};
    float Volume{};
    float DopplerScale{};
    int frequency{};
    int MaxSingleSounds{};
    int MaxTotalSounds{};
    bool Sound{};
    bool ai_sound{};
    float explosion_closeness{};
    float loss_relationship{};
    float victory_relationship{};
    float time_between_music{};

/* Cockpit Audio Options */
    std::string comm;
    std::string scanning;
    std::string objective;
    std::string examine;
    std::string view;
    std::string repair;
    std::string manifest;
    int compress_max{};
    std::string compress_loop;
    std::string compress_change;
    std::string compress_stop;
    int compress_interval{};

    bool comm_preload{};

/* Unit Audio Options */
    std::string jumpleave;
    std::string player_armor_hit;
    std::string player_hull_hit;
    std::string player_shield_hit;

/* AI Options */
    bool AllowCivilWar{};
    bool CappedFactionRating{};
    bool AllowNonplayerFactionChange{};
    double min_relationship{};
    std::string startDockedTo;
    bool dockOnLoad{};

/* Data Options */
    std::string universe_path;
    std::string sectors;
    std::string techniquesBasePath;
    std::string unitCSV;
    std::string modUnitCSV;
    std::string cockpits;
    std::string animations;
    std::string movies;
    std::string sprites;
    std::string serialized_xml;
    std::string sharedtextures;
    std::string sharedsounds;
    std::string sharedmeshes;
    std::string sharedunits;
    std::string ai_directory;
    std::string datadir;
    std::string hqtextures;
    std::string volume_format;

/* Galaxy Options */
    bool PushValuesToMean{};
    float AmbientLightFactor{};
    float StarRadiusScale{};
    std::string DefaultAtmosphereTexture;
    float AtmosphereProbability{};
    float RingProbability{};
    float DoubleRingProbability{};
    float InnerRingRadius{};
    float OuterRingRadius{};
    std::string DefaultRingTexture;
    float SecondRingDifference{};
    float MoonRelativeToPlanet{};
    float MoonRelativeToMoon{};
    float RockyRelativeToPrimary{};
    float CompactnessScale{};
    float JumpCompactnessScale{};
    int MeanNaturalPhenomena{};
    int MeanStarBases{};
//    float    SmallUnitsMultiplier;  not used

    std::string getCallsign(int);
    std::string getPlayer(int);
    std::string getPassword(int);
};

extern std::shared_ptr<vs_options> game_options();

#endif //VEGA_STRIKE_ENGINE_OPTIONS_H
