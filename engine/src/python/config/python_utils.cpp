/*
 * python_utils.cpp
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

// -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "python_utils.h"

#include <iostream>
#include <boost/python.hpp>
#include <boost/filesystem.hpp>

//#include "root_generic/vsfilesystem.h"
//#include "src/vs_logging.h"

using namespace boost::python;
using namespace boost::filesystem;

#define Q(x) #x
#define QUOTE(x) Q(x)

// This is a kludge. It runs python before
// just to get the python paths.
std::string GetPythonPath() {
    Py_Initialize();
    wchar_t* w_path_ptr = Py_GetPath();
    Py_Finalize();

    std::wstring w_path_w( w_path_ptr );
    std::string path( w_path_w.begin(), w_path_w.end() );

    return path;
}



// A utility function to call a function and get a PyObject as a result
PyObject* GetClassFromPython(
    const std::string build_path,
    const std::string path_string,
    const std::string datadir,
    const std::string module_name,
    const std::string function_name) {
    boost::filesystem::path full_path = boost::filesystem::path(path_string);
    boost::filesystem::current_path(full_path);

    if(!boost::filesystem::exists(module_name + ".py")) {
        // TODO: throw exception
        return nullptr;
    }

    PyPreConfig py_pre_config;
    PyPreConfig_InitPythonConfig(&py_pre_config);

    PyStatus status;

    status = Py_PreInitialize(&py_pre_config);
    if (PyStatus_Exception(status)) {
        std::cerr << "Python pre-initialization failed" << std::endl << std::flush;
        Py_ExitStatusException(status);
    }

    const std::string python_path = GetPythonPath();
    const std::wstring python_path_w(python_path.begin(), python_path.end());
    const std::wstring path_string_w = std::wstring(path_string.begin(), path_string.end());
    const std::string python_sitelib_path = QUOTE(Python_SITELIB);
    const std::wstring python_sitelib_path_wstring = std::wstring(python_sitelib_path.begin(), python_sitelib_path.end());
    const std::wstring build_path_w = std::wstring(build_path.begin(), build_path.end());
    const std::wstring data_path_w = std::wstring(datadir.begin(), datadir.end());

    PyWideStringList python_path_py_wide_string_list{};
    status = PyWideStringList_Append(&python_path_py_wide_string_list, python_path_w.c_str());
    if (PyStatus_Exception(status)) {
        std::cerr << "Python path list append 1 failed" << std::endl << std::flush;
        PyErr_Print();
        Py_ExitStatusException(status);
    }
    status = PyWideStringList_Append(&python_path_py_wide_string_list, path_string_w.c_str());
    if (PyStatus_Exception(status)) {
        std::cerr << "Python path list append 2 failed" << std::endl << std::flush;
        PyErr_Print();
        Py_ExitStatusException(status);
    }
    status = PyWideStringList_Append(&python_path_py_wide_string_list, python_sitelib_path_wstring.c_str());
    if (PyStatus_Exception(status)) {
        std::cerr << "Python path list append 3 failed" << std::endl << std::flush;
        PyErr_Print();
        Py_ExitStatusException(status);
    }
    status = PyWideStringList_Append(&python_path_py_wide_string_list, build_path_w.c_str());
    if (PyStatus_Exception(status)) {
        std::cerr << "Python path list append 4 failed" << std::endl << std::flush;
        PyErr_Print();
        Py_ExitStatusException(status);
    }
    status = PyWideStringList_Append(&python_path_py_wide_string_list, data_path_w.c_str());
    if (PyStatus_Exception(status)) {
        std::cerr << "Python path list append 5 failed" << std::endl << std::flush;
        PyErr_Print();
        Py_ExitStatusException(status);
    }

    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    config.module_search_paths = python_path_py_wide_string_list;
    config.isolated = 0;

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        std::cerr << "Python config initialization failed" << std::endl << std::flush;
        PyErr_Print();
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
    }

    PyObject* module = PyImport_ImportModule(module_name.c_str());

    if(!module) {
        std::cerr << "Python module import failed (PyImport_ImportModule is null)" << std::endl << std::flush;
        PyErr_Print();
        Py_Finalize();
        // TODO: throw exception
        return nullptr;
    }

    PyObject* function = PyObject_GetAttrString(module,function_name.c_str());
    if(!function) {
        std::cerr << "PyObject_GetAttrString is null" << std::endl << std::flush;
        PyErr_Print();
        Py_Finalize();
        // TODO: throw exception
        return nullptr;
    }


    PyObject* pyResult = PyObject_CallObject(function, nullptr);
    Py_Finalize();

    return pyResult;
}
