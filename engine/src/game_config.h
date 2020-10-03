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



class GameConfig
{
private:
    static std::map<string, string> variables;
    static string DEFAULT_ERROR_VALUE;
    static inline string _GetVariable(string const &section, string const &name)
    {
        string const key = section + "." + name;
        if (variables.count(key))
            return variables[key];
        return DEFAULT_ERROR_VALUE;
    }
public:
    static void LoadGameConfig(const string &filename);
    template <class T>
    static inline T GetVariable(string const &section, string const &name, T default_value) = delete;
};



template <>
inline bool GameConfig::GetVariable(string const &section, string const &name, bool default_value)
{
    string result = _GetVariable(section, name);
    if(result == DEFAULT_ERROR_VALUE) return default_value;
    boost::algorithm::to_lower(result);
    return result == "true";
}

template <>
inline float GameConfig::GetVariable(string const &section, string const &name, float default_value)
{
    string result = _GetVariable(section, name);
    if(result == DEFAULT_ERROR_VALUE) return default_value;
    return std::stof(result);
}

template <>
inline double GameConfig::GetVariable(string const &section, string const &name, double default_value)
{
    string result = _GetVariable(section, name);
    if(result == DEFAULT_ERROR_VALUE) return default_value;
    return std::stod(result);
}

template <>
inline int GameConfig::GetVariable(string const &section, string const &name, int default_value)
{
    string result = _GetVariable(section, name);
    if(result == DEFAULT_ERROR_VALUE) return default_value;
    return std::stoi(result);
}

#endif // GAME_CONFIG_H
