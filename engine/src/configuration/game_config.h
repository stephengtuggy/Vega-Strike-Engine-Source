/**
 * game_config.h
 *
 * Copyright (C) 2020-2022 Roy Falk, Stephen G. Tuggy, David Wales,
 * and other Vega Strike Contributors
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


#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <set>
#include <map>
#include <exception>
#include <iostream>

namespace pt = boost::property_tree;

using std::string;
using pt::ptree;

// TODO: test this functionality, especially the subsection...

class GameConfig {
private:
    static std::map<string, string> variables;
    static string DEFAULT_ERROR_VALUE;

    static inline string _GetVariable(string const &section, string const &name) {
        string const key = section + "." + name;
        if (variables.count(key)) {
            return variables[key];
        }
        return DEFAULT_ERROR_VALUE;
    }

    static inline string _GetVariable(string const &section,
            string const &sub_section,
            string const &name) {
        string const key = section + "." + sub_section + "." + name;
        if (variables.count(key)) {
            return variables[key];
        }
        return DEFAULT_ERROR_VALUE;
    }

public:
    static void LoadGameConfig(const string &filename);
    template<class T>
    static inline T GetVariable(string const &section, string const &name, T default_value) = delete;

    template<class T>
    static inline T GetVariable(string const &section, string const &sub_section,
            string const &name, T default_value) = delete;

};

// Template Specialization
template<>
inline bool GameConfig::GetVariable(string const &section, string const &name, bool default_value) {
    string result = _GetVariable(section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    boost::algorithm::to_lower(result);
    return result == "true";
}

template<>
inline float GameConfig::GetVariable(string const &section, string const &name, float default_value) {
    string result = _GetVariable(section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stof(result);
}

template<>
inline double GameConfig::GetVariable(string const &section, string const &name, double default_value) {
    string result = _GetVariable(section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stod(result);
}

template<>
inline int GameConfig::GetVariable(string const &section, string const &name, int default_value) {
    string result = _GetVariable(section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stoi(result);
}

template <>
inline std::string GameConfig::GetVariable(string const &section, string const &name, std::string default_value)
{
    string result = _GetVariable(section, name);
    if(result == DEFAULT_ERROR_VALUE) return default_value;
    return result;
}

// With Subsection
template<>
inline bool GameConfig::GetVariable(string const &section, string const &sub_section,
        string const &name, bool default_value) {
    string result = _GetVariable(section, sub_section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    boost::algorithm::to_lower(result);
    return result == "true";
}

template<>
inline float GameConfig::GetVariable(string const &section, string const &sub_section,
        string const &name, float default_value) {
    string result = _GetVariable(section, sub_section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stof(result);
}

template<>
inline double GameConfig::GetVariable(string const &section, string const &sub_section,
        string const &name, double default_value) {
    string result = _GetVariable(section, sub_section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stod(result);
}

template<>
inline int GameConfig::GetVariable(string const &section, string const &sub_section,
        string const &name, int default_value) {
    string result = _GetVariable(section, sub_section, name);
    if (result == DEFAULT_ERROR_VALUE) {
        return default_value;
    }
    return std::stoi(result);
}

#endif // GAME_CONFIG_H
