/*
 * vega_conversion.h
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

#ifndef VEGA_STRIKE_VEGA_CONVERSION_H
#define VEGA_STRIKE_VEGA_CONVERSION_H
#include <climits>
#include <string>

#include "vs_logging.h"

namespace vega_conversion {
    enum class conversion_result {
        success,
        invalid_argument,
        out_of_range,
        input_chars_remaining,
    };

    struct int_conversion_result {
        conversion_result result;
        int value;
    };

    inline int_conversion_result str_to_int(const char* input_string, const std::string& user_friendly_value_descrip,
                                            const int minimum = INT_MIN, const int maximum = INT_MAX) {
        errno = 0;
        char* end_ptr;
        const long int value = std::strtol(input_string, &end_ptr, 10);
        if (errno == EINVAL) {
            VS_LOG(error, (boost::format("Invalid %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::invalid_argument, 0};
        } else if (errno == ERANGE || value > maximum || value < minimum) {
            VS_LOG(error, (boost::format("Out-of-range %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::out_of_range, 0};
        } else if (*end_ptr == '\0') {
            errno = 0;
            return {conversion_result::success, static_cast<int>(value)};
        } else {
            VS_LOG(warning,
                   (boost::format("%1%: not completely converted (characters remaining)") % user_friendly_value_descrip
                   ));
            errno = 0;
            return {conversion_result::input_chars_remaining, static_cast<int>(value)};
        }
    }

    struct size_t_conversion_result {
        conversion_result result;
        size_t value;
    };

    inline size_t_conversion_result str_to_size_t(const char* input_string,
                                                  const std::string& user_friendly_value_descr) {
        errno = 0;
        char* end_ptr;
        const size_t value = std::strtoull(input_string, &end_ptr, 10);
        if (errno == EINVAL) {
            VS_LOG(error, (boost::format("Invalid %1%: %2%") % user_friendly_value_descr % input_string));
            errno = 0;
            return {conversion_result::invalid_argument, 0ULL};
        } else if (errno == ERANGE || value > std::numeric_limits<size_t>::max()) {
            VS_LOG(error, (boost::format("Out-of-range %1%: %2%") % user_friendly_value_descr % input_string));
            errno = 0;
            return {conversion_result::out_of_range, 0ULL};
        } else if (*end_ptr == '\0') {
            errno = 0;
            return {conversion_result::success, static_cast<size_t>(value)};
        } else {
            VS_LOG(warning,
                   (boost::format("%1%: not completely converted (characters remaining)") % user_friendly_value_descr));
            errno = 0;
            return {conversion_result::input_chars_remaining, static_cast<size_t>(value)};
        }
    }

    struct int_max_t_conversion_result {
        conversion_result result;
        intmax_t value;
    };

    inline int_max_t_conversion_result str_to_int_max_t(const char* input_string,
                                                        const std::string& user_friendly_value_descrip,
                                                        intmax_t minimum = INTMAX_MIN, intmax_t maximum = INTMAX_MAX) {
        errno = 0;
        char* end_ptr;
        const intmax_t value = std::strtoll(input_string, &end_ptr, 10);
        if (errno == EINVAL) {
            VS_LOG(error, (boost::format("Invalid %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::invalid_argument, 0LL};
        } else if (errno == ERANGE || value > maximum || value < minimum) {
            VS_LOG(error, (boost::format("Out-of-range %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::out_of_range, 0LL};
        } else if (*end_ptr == '\0') {
            errno = 0;
            return {conversion_result::success, value};
        } else {
            VS_LOG(warning,
                   (boost::format("%1%: not completely converted (characters remaining)") % user_friendly_value_descrip
                   ));
            errno = 0;
            return {conversion_result::input_chars_remaining, value};
        }
    }

    struct float_conversion_result {
        conversion_result result;
        float value;
    };

    inline float_conversion_result str_to_float(const char* input_string,
                                                const std::string& user_friendly_value_descrip,
                                                const float minimum = FLT_MIN, const float maximum = FLT_MAX) {
        errno = 0;
        char* end_ptr;
        const float value = std::strtof(input_string, &end_ptr);
        if (errno == EINVAL) {
            VS_LOG(error, (boost::format("Invalid %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::invalid_argument, 0.0F};
        } else if (errno == ERANGE || value > maximum || value < minimum) {
            VS_LOG(error, (boost::format("Out-of-range %1%: %2%") % user_friendly_value_descrip % input_string));
            errno = 0;
            return {conversion_result::out_of_range, 0.0F};
        } else if (*end_ptr == '\0') {
            errno = 0;
            return {conversion_result::success, value};
        } else {
            VS_LOG(warning,
                   (boost::format("%1%: not completely converted (characters remaining)") % user_friendly_value_descrip
                   ));
            errno = 0;
            return {conversion_result::input_chars_remaining, value};
        }
    }

    struct double_conversion_result {
        conversion_result result;
        double value;
    };

    inline double_conversion_result str_to_double(const char* input_string,
                                                  const std::string& user_friendly_value_descr,
                                                  const double minimum = DBL_MIN, const double maximum = DBL_MAX) {
        errno = 0;
        char* end_ptr;
        const double value = std::strtod(input_string, &end_ptr);
        if (errno == EINVAL) {
            VS_LOG(error, (boost::format("Invalid %1%: %2%") % user_friendly_value_descr % input_string));
            errno = 0;
            return {conversion_result::invalid_argument, 0.0};
        } else if (errno == ERANGE || value > maximum || value < minimum) {
            VS_LOG(error, (boost::format("Out-of-range %1%: %2%") % user_friendly_value_descr % input_string));
            errno = 0;
            return {conversion_result::out_of_range, 0.0};
        } else if (*end_ptr == '\0') {
            errno = 0;
            return {conversion_result::success, value};
        } else {
            VS_LOG(warning,
                   (boost::format("%1%: not completely converted (characters remaining)") % user_friendly_value_descr));
            errno = 0;
            return {conversion_result::input_chars_remaining, value};
        }
    }

    struct bool_conversion_result {
        conversion_result result;
        bool value;
    };

    inline bool_conversion_result str_to_bool(const char* input_string,
                                              const std::string& user_friendly_value_descr = "") {
        errno = 0;
        const std::string in_string(input_string);
        if (!in_string.empty() && (*in_string.begin() == 't' || *in_string.begin() == 'T' || *in_string.begin() == 'y'
            || *in_string.begin() == 'Y' || *in_string.begin() == '1' || in_string == "on" || in_string == "ON")) {
            return {conversion_result::success, true};
        } else {
            return {conversion_result::success, false};
        }
    }
}

#endif //VEGA_STRIKE_VEGA_CONVERSION_H
