#!/usr/bin/env python3
##
# add_gpl_license.py
#
# Vega Strike - Space Simulation, Combat and Trading
# Copyright (C) 2001-2026 The Vega Strike Contributors:
# Project creator: Daniel Horn
# Original development team: As listed in the AUTHORS file. Specifically: daviewales
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

"""This script takes a file path (or paths) as an argument, and rewrites the file
with the Vega Strike GPLv3+ copyright notice at the top.

If no arguments are passed, the script will read from stdin.

The Copyright text is copied from the LICENSE file, which is assumed to be at
the root of the current git repository. Hence, this script needs to be run from
within a git repository containing a LICENSE file at the root. The lines at the
start of the LICENSE file, starting with 'Copyright (C)' and ending just before
'GNU GENERAL PUBLIC LICENSE', will be copied as the Copyright text for the GPL
notice. e.g. If the LICENSE file contains the following lines, they will be copied
in at the start of the GPL notice:

    Copyright (c) 2001-2002 Daniel Horn
    Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
    Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors

Usage:
    python3 add_gpl_license.py [-h|--help]
    python3 add_gpl_license.py PATH [PATH ...]
"""
from datetime import datetime, timezone
import sys
from pathlib import Path
from shutil import copystat, move
from tempfile import NamedTemporaryFile
import re

COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_AND_ALSO_REGEX = re.compile(r"^(?:Vega Strike - Space Simulation, Combat and Trading\n)?Copyright \([cC]\) (\d{4})(?:-\d{4})? *The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: ((?:by *)?[A-Za-z0-9 .<@>_]+(?:,\s*[A-Za-z0-9 .<@>_]+){0,20})\nAlso:((?:by *)?[A-Za-z0-9 .<@>_]+(?:,\s*[A-Za-z0-9 .<@>_]+){0,20})\nCurrent development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy(?:\nCopyright \([cC]\) \d{4}(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors)?)*$", re.MULTILINE)
COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_REGEX = re.compile(r"^(?:Vega Strike - Space Simulation, Combat and Trading\n)?Copyright \([cC]\) (\d{4})(?:-\d{4})? *The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: ((?:by *)?[A-Za-z0-9 .<@>_]+(?:,\s*[A-Za-z0-9 .<@>_]+){0,20})\nCurrent development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy(?:\nCopyright \([cC]\) \d{4}(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors)?)*$", re.MULTILINE)
COPYRIGHT_VEGA_CONTRIBUTORS_REGEX = re.compile(r"^(?:Vega Strike - Space Simulation, Combat and Trading\n)?Copyright \([cC]\) (\d{4})(?:-\d{4})? *The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy(?:\nCopyright \([cC]\) \d{4}(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors)?)*$", re.MULTILINE)
GENERIC_VEGA_COPYRIGHT_REGEX = re.compile(r"^Vega Strike - Space Simulation, Combat and Trading(?:\nCopyright \([cC]\) \d{4}(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors)?)*$", re.MULTILINE)

BASIC_COPYRIGHT_REGEX = re.compile(r'^Copyright +\([Cc]\) +\d{4}(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:, *)?$')
GENERIC_MULTI_COPYRIGHT_REGEX = re.compile(r"^Copyright \([cC]\) (\d{4})(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors)?(?:\nCopyright \([cC]\) (\d{4})(?:-\d{4})?(?:, *\d{4}(?:-\d{4})?)* *(?:by *)?([A-Za-z0-9 .<@>_]+(?:, *[A-Za-z0-9 .<@>_]+){0,20})(?:, *)?\n?(?: ?and other| ?The)\n? ?Vega Strike [Cc]ontributors|Largely +[Rr]ewritten +[Bb]y +[A-Za-z0-9 .<@>_]+){0,20}$", re.MULTILINE)
LARGELY_REWRITTEN_BY_REGEX = re.compile(r'^Largely +[Rr]ewritten +[Bb]y +[A-Za-z0-9 .<@>_]+$')
VEGA_STRIKE_CONTRIBUTORS_REGEX = re.compile(r'^(?: ?and other| ?The)? ?Vega Strike [Cc]ontributors$')

THIS_FILE_IS_PART_OF_VEGA_STRIKE_REGEX = re.compile(r"^(?:https://github\.com/vegastrike/Vega-Strike-Engine-Source\n+)?This file is part of Vega Strike\.\n\nVega Strike is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version [23] of the License, or\n\(at your option\) any later version\.\n\nVega Strike is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\. +See the\nGNU General Public License for more details\.\n\nYou should have received a copy of the GNU General Public License\nalong with Vega Strike\. +If not, see <https://www\.gnu\.org/licenses/>\.$", re.MULTILINE)
THIS_FILE_IS_PART_OF_VEGA_STRIKE_TEXT = """
https://github.com/vegastrike/Vega-Strike-Engine-Source

This file is part of Vega Strike.

Vega Strike is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Vega Strike is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>."""

INCORPORATED_FROM_OPCODE_PUBLIC_DOMAIN_REGEX = re.compile(r"^This file is part of OPCODE - Optimized Collision Detection\n\(http://www\.codercorner\.com/Opcode\.htm\) and has been\nincorporated into Vega Strike\n\(https://github\.com/vegastrike/Vega-Strike-Engine-Source\)\.\n\nPublic Domain$", re.MULTILINE)
INCORPORATED_FROM_OPCODE_PUBLIC_DOMAIN_TEXT = """
This file is part of OPCODE - Optimized Collision Detection
(http://www.codercorner.com/Opcode.htm) and has been
incorporated into Vega Strike
(https://github.com/vegastrike/Vega-Strike-Engine-Source).

Public Domain"""

LGPL_REGEX = re.compile(r"^This library is free software; you can redistribute it and/or\nmodify it under the terms of the GNU (?:Library|Lesser) General Public\nLicense as published by the Free Software Foundation; either\nversion (?:2|2\.0|2\.1|3|3\.0) of the License, or \(at your option\) any later version\.\n\nThis library is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\. +See the GNU\n(?:Library|Lesser) General Public License for more details\.\n\nYou should have received a copy of the GNU (?:Library|Lesser) General Public\nLicense along with this library; if not, (?:see\n<https://www\.gnu\.org/licenses/>\.|write to the Free\nSoftware Foundation, Inc\., 675 Mass Ave, Cambridge, MA 02139, USA\.)$", re.MULTILINE)
LGPL_TEXT = """
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see
<https://www.gnu.org/licenses/>."""

# Characters to use for the start, middle and end of a comment block
C_LIKE_COMMENT = ['/*', ' *', ' */', '/**', '*', '*/']
C_LIKE_COMMENT_BLOCK_START_REGEX = re.compile(r'^/\*+(.*)$')
C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX = re.compile(r'^ *\**( *)(.*)$')
C_LIKE_COMMENT_BLOCK_END_REGEX = re.compile(r'^ *(.*) *\*/$')
C_LIKE_INDIVIDUAL_COMMENT_REGEX = re.compile(r'^ *//( *)(.*)$')
C_LIKE_BASIC_OFFSET_HEADER_COMMENT_REGEX = re.compile(r'^ *// *-\*- *mode: *(.*)$')

SCRIPT_LIKE_COMMENT = ['##', '#', '#']
SCRIPT_LIKE_COMMENT_REGEX = re.compile(r'^ *#+( *)(.*)$')

SHEBANG_REGEX = re.compile(r'^#!(.*)$')

C_LIKE_COMMENTS = ['.c', '.cpp', '.h', '.hpp', '.h.in', '.yacc', '.lex']
SCRIPT_LIKE_COMMENTS = ['.py', '.cmake', '.deprecated.cmake', '.txt', '.sh', '.ps1', '.yml', '.yaml']

ROW_OF_EQUALS_SIGNS_REGEX = re.compile(r'^={5,200}$')
ROW_OF_ASTERISKS_REGEX = re.compile(r'^\*{5,200}$')
ROW_OF_SLASHES_REGEX = re.compile(r'^/{5,200}$')


def find_git_root() -> Path:
    """If we're in a git repository, return the path to the git root directory.
    Otherwise, raise an error."""

    pwd: Path = Path.cwd()
    # Check if we're already at the git root
    if (pwd/'.git').exists():
        return pwd
    # Else iterate through the parent directories until we find the git root,
    # or we hit the filesystem root directory.
    else:
        for directory in pwd.parents:
            if (directory/'.git').exists():
                return directory
        raise FileNotFoundError('Not a git repository!')


def get_reference_copyright_notice(license_file_path: Path) -> str:
    """Return lines starting with 'Copyright (C)' from the start of a given
    LICENSE file. Stop reading the file after the license title is found:
        GENERAL PUBLIC LICENSE
    """
    copyright_lines: list[str] = []
    with license_file_path.open('r') as file:
        for line in file:
            if 'GENERAL PUBLIC LICENSE' in line:
                break
            copyright_lines.append(line)

    return ''.join(copyright_lines).removesuffix('\n').removesuffix('\r')


def is_a_comment(line: str, comment_prefixes: list[str]) -> bool:
    for prefix in comment_prefixes:
        if line.startswith(prefix):
            return True
    return False


def is_a_comment_2(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return is_a_comment(line, SCRIPT_LIKE_COMMENT)
    else:
        return is_a_comment(line, C_LIKE_COMMENT)


def is_start_of_a_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_COMMENT_BLOCK_START_REGEX.match(line) or C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def is_start_of_a_block_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return False
    else:
        return C_LIKE_COMMENT_BLOCK_START_REGEX.match(line)


def is_middle_of_a_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line) or C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def is_middle_of_a_block_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return False
    else:
        return C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line)


def is_individually_commented(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def is_end_of_a_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_COMMENT_BLOCK_END_REGEX.match(line) or C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def uncomment_start(line: str, script_like_file: bool) -> str:
    if script_like_file:
        if SCRIPT_LIKE_COMMENT_REGEX.match(line):
            return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(2)
        else:
            print(f"'{line}' is not the start of a comment")
            return line
    else:
        if C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
            return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(2)
        elif C_LIKE_COMMENT_BLOCK_START_REGEX.match(line):
            return C_LIKE_COMMENT_BLOCK_START_REGEX.match(line).group(1)
        else:
            print(f"'{line}' is not the start of a comment")
            return line


def uncomment_middle(line: str, script_like_file: bool, num_prefix_whitespace_chars: int) -> tuple[str, int]:
    if num_prefix_whitespace_chars < 0:
        if script_like_file:
            if SCRIPT_LIKE_COMMENT_REGEX.match(line):
                num_prefix_whitespace_chars = len(SCRIPT_LIKE_COMMENT_REGEX.match(line).group(1))
                return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(2), num_prefix_whitespace_chars
            else:
                print(f"'{line}' is not the middle of a comment")
                return line, num_prefix_whitespace_chars
        else:
            if C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
                num_prefix_whitespace_chars = len(C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(1))
                return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(2), num_prefix_whitespace_chars
            elif C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line):
                num_prefix_whitespace_chars = len(C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line).group(1))
                return C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line).group(2), num_prefix_whitespace_chars
            else:
                print(f"'{line}' is not the middle of a comment")
                return line, num_prefix_whitespace_chars
    else:
        prefix_whitespace_chars: str = f"{num_prefix_whitespace_chars}"
        if script_like_file:
            if SCRIPT_LIKE_COMMENT_REGEX.match(line):
                comment_regex_adjusted = re.compile(r"^ *#+ {0," + prefix_whitespace_chars + "}(.*)$")
                return comment_regex_adjusted.match(line).group(1), num_prefix_whitespace_chars
            else:
                print(f"'{line}' is not the middle of a comment")
                return line, num_prefix_whitespace_chars
        else:
            if C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
                comment_regex_adjusted = re.compile(r"^ *// {0," + prefix_whitespace_chars + "}(.*)$")
                return comment_regex_adjusted.match(line).group(1), num_prefix_whitespace_chars
            elif C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line):
                comment_regex_adjusted = re.compile(r"^ *\** {0," + prefix_whitespace_chars + "}(.*)$")
                return comment_regex_adjusted.match(line).group(1), num_prefix_whitespace_chars
            else:
                print(f"'{line}' is not the middle of a comment")
                return line, num_prefix_whitespace_chars


def uncomment_end(line: str, script_like_file: bool) -> str:
    if script_like_file:
        if SCRIPT_LIKE_COMMENT_REGEX.match(line):
            return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(2)
        else:
            print(f"'{line}' is not the end of a comment")
            return line
    else:
        if C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
            return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(2)
        elif C_LIKE_COMMENT_BLOCK_END_REGEX.match(line):
            return C_LIKE_COMMENT_BLOCK_END_REGEX.match(line).group(1)
        else:
            print(f"'{line}' is not the end of a comment")
            return line


def comment_block(block: str, script_like_file: bool) -> str:
    individual_lines: list[str] = block.splitlines()
    ret_val: str = ''
    if script_like_file:
        ret_val = "##\n"
        for line in individual_lines:
            if line == '':
                ret_val += "#\n"
            else:
                ret_val += f"# {line}\n"
        ret_val += "#\n"
        return ret_val
    else:
        ret_val = "/*\n"
        for line in individual_lines:
            if line == '':
                ret_val += " *\n"
            else:
                ret_val += f" * {line}\n"
        ret_val += " */\n"
        return ret_val


def upsert_license_header(filepath: Path) -> None:
    """Add GPL license notice at the start of the given file, if not present, or update it if present."""
    print(f"Adding/updating license header at start of file '{filepath}'")

    # Sanity checks
    if not filepath.exists():
        print(f"{filepath} does not exist")
        return

    if not filepath.is_file():
        print(f"{filepath} is not a file")
        return

    suffix: str = ''.join(filepath.suffixes)
    script_like_file: bool = suffix in SCRIPT_LIKE_COMMENTS
    license_header_commented: str = ''
    license_header_uncommented_lines: list[str] = []
    in_license_header_comment: bool = False
    individually_commented: bool = False
    start_year: str = "2001"
    current_year: str = datetime.now(timezone.utc).strftime("%Y")
    copyright_current_year_regex = re.compile(r'^Copyright +\([Cc]\) +' + current_year)
    copyright_current_year_regex_2 = re.compile(r'^Copyright +\([Cc]\) +\d{4}-' + current_year)
    already_read_lines: str = ""

    # Open our input and output files
    with filepath.open('r') as input_file:
        with NamedTemporaryFile('w', delete=False, newline='\n') as output_file:
            # Parse the beginning of the input file
            first_line: str = input_file.readline()
            if SHEBANG_REGEX.match(first_line):
                script_like_file = True
                output_file.write(first_line)
                second_line: str = input_file.readline()
                if is_start_of_a_comment(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines.append(uncomment_start(second_line, script_like_file))
                    individually_commented = True
                elif second_line == '\n':
                    third_line: str = input_file.readline()
                    if is_start_of_a_comment(third_line, script_like_file):
                        output_file.write(second_line)
                        license_header_commented += third_line
                        in_license_header_comment = True
                        license_header_uncommented_lines.append(uncomment_start(third_line, script_like_file))
                        individually_commented = True
                    else:
                        already_read_lines = third_line
                else:
                    already_read_lines = second_line

            elif not script_like_file and C_LIKE_BASIC_OFFSET_HEADER_COMMENT_REGEX.match(first_line):
                output_file.write(first_line)
                second_line: str = input_file.readline()
                if is_start_of_a_comment(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines.append(uncomment_start(second_line, script_like_file))
                    if is_start_of_a_block_comment(second_line, script_like_file):
                        individually_commented = False
                    else:
                        individually_commented = True
                elif second_line == '\n':
                    third_line: str = input_file.readline()
                    if is_start_of_a_comment(third_line, script_like_file):
                        output_file.write(second_line)
                        license_header_commented += third_line
                        in_license_header_comment = True
                        license_header_uncommented_lines.append(uncomment_start(third_line, script_like_file))
                        if is_start_of_a_block_comment(third_line, script_like_file):
                            individually_commented = False
                        else:
                            individually_commented = True
                    else:
                        already_read_lines = third_line
                else:
                    already_read_lines = second_line

            elif is_start_of_a_block_comment(first_line, script_like_file):
                license_header_commented += first_line
                in_license_header_comment = True
                license_header_uncommented_lines.append(uncomment_start(first_line, script_like_file))
                individually_commented = False

            elif ROW_OF_SLASHES_REGEX.match(first_line):
                output_file.write(first_line)
                second_line: str = input_file.readline()
                if is_start_of_a_block_comment(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines.append(uncomment_start(second_line, script_like_file))
                    individually_commented = False
                elif is_individually_commented(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines.append(uncomment_start(second_line, script_like_file))
                    individually_commented = True
                else:
                    already_read_lines = second_line

            elif is_individually_commented(first_line, script_like_file):
                license_header_commented += first_line
                in_license_header_comment = True
                license_header_uncommented_lines.append(uncomment_start(first_line, script_like_file))
                individually_commented = True

            else:
                already_read_lines = first_line

            # Continue parsing the input file up to the end of the initial comment block (if any)
            num_prefix_whitespace_chars: int = -1
            while in_license_header_comment:
                current_line: str = input_file.readline()

                if individually_commented:
                    if current_line == '\n':
                        # We've reached the end of the initial comment block
                        in_license_header_comment = False
                        break

                    elif is_individually_commented(current_line, script_like_file):
                        license_header_commented += current_line
                        uncommented_line, num_prefix_whitespace_chars = uncomment_middle(current_line, script_like_file, num_prefix_whitespace_chars)
                        license_header_uncommented_lines.append(uncommented_line)

                    else:
                        in_license_header_comment = False
                        if len(current_line) > 1:
                            already_read_lines = current_line
                        break
                else:
                    if is_end_of_a_comment(current_line, script_like_file):
                        # We've reached the end of the initial comment block
                        license_header_commented += current_line
                        license_header_uncommented_lines.append(uncomment_end(current_line, script_like_file))
                        in_license_header_comment = False
                        break

                    else:
                        license_header_commented += current_line
                        uncommented_line, num_prefix_whitespace_chars = uncomment_middle(current_line, script_like_file, num_prefix_whitespace_chars)
                        license_header_uncommented_lines.append(uncommented_line)

            # Trim leading blank lines
            while len(license_header_uncommented_lines) > 0 and license_header_uncommented_lines[0] == '':
                license_header_uncommented_lines.pop(0)

            # Handle filename clause
            output_copyright_notice: str = ''
            at_file_regex = re.compile(r"^ *@file *: *" + filepath.name + r"$", re.MULTILINE)
            if len(license_header_uncommented_lines) == 0:
                print(f"Copyright header at top of file '{filepath}' missing at least the last half. Adding")
                output_copyright_notice += filepath.name
                output_copyright_notice += "\n\nVega Strike - Space Simulation, Combat and Trading\nCopyright (C) 2001-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                output_copyright_notice += THIS_FILE_IS_PART_OF_VEGA_STRIKE_TEXT
                if individually_commented:
                    output_file.write(comment_block(output_copyright_notice, script_like_file) + '\n')
                else:
                    output_file.write(comment_block(output_copyright_notice, script_like_file))
                if already_read_lines:
                    output_file.write(already_read_lines)
                output_file.write(input_file.read())
                output_file.close()
                # Copy original file attributes and permissions to temp file
                copystat(filepath, output_file.name)
                # Move temp file into place
                move(output_file.name, filepath)
                return
            elif license_header_uncommented_lines[0] == filepath.name:
                output_copyright_notice += filepath.name + '\n'
                license_header_uncommented_lines.pop(0)
            elif at_file_regex.match(license_header_uncommented_lines[0]):
                output_copyright_notice += license_header_uncommented_lines[0] + '\n'
                license_header_uncommented_lines.pop(0)
            elif len(license_header_uncommented_lines) > 1 and ROW_OF_EQUALS_SIGNS_REGEX.match(license_header_uncommented_lines[0]) and at_file_regex.match(license_header_uncommented_lines[1]):
                output_copyright_notice += license_header_uncommented_lines.pop(0) + '\n'
                output_copyright_notice += license_header_uncommented_lines.pop(0) + '\n'
                while not ROW_OF_EQUALS_SIGNS_REGEX.match(license_header_uncommented_lines[0]):
                    output_copyright_notice += license_header_uncommented_lines.pop(0) + '\n'
                output_copyright_notice += license_header_uncommented_lines.pop(0) + '\n'
            else:
                found_copyright: bool = False
                for current_line in license_header_uncommented_lines:
                    if 'Copyright' in current_line:
                        found_copyright = True
                        break
                if not found_copyright:
                    print(f"No copyright header at top of file '{filepath}'. Adding")
                    output_copyright_notice += filepath.name
                    output_copyright_notice += "\n\nVega Strike - Space Simulation, Combat and Trading\nCopyright (C) 2001-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                    output_copyright_notice += THIS_FILE_IS_PART_OF_VEGA_STRIKE_TEXT
                    old_header: str = '\n'.join(license_header_uncommented_lines)
                    if individually_commented:
                        output_file.write(comment_block(output_copyright_notice, script_like_file) + '\n\n')
                    else:
                        output_file.write(comment_block(output_copyright_notice, script_like_file) + '\n')
                    if individually_commented:
                        output_file.write(comment_block(old_header, script_like_file) + '\n\n')
                    else:
                        output_file.write(comment_block(old_header, script_like_file) + '\n')
                    if already_read_lines:
                        output_file.write(already_read_lines)
                    output_file.write(input_file.read())
                    output_file.close()
                    # Copy original file attributes and permissions to temp file
                    copystat(filepath, output_file.name)
                    # Move temp file into place
                    move(output_file.name, filepath)
                    return

            # Trim leading blank lines again after filename clause
            while len(license_header_uncommented_lines) > 0 and license_header_uncommented_lines[0] == '':
                license_header_uncommented_lines.pop(0)

            # Handle copyright clause and list of authors
            license_header_uncommented_concat_1 = '\n'.join(license_header_uncommented_lines)
            output_copyright_notice += "\nVega Strike - Space Simulation, Combat and Trading\n"

            if COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_AND_ALSO_REGEX.match(license_header_uncommented_concat_1):
                print(f"File '{filepath}': Copyright Vega Contributors with Specifically and Also clauses")
                copyright_notice_match: re.Match[str] = COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_AND_ALSO_REGEX.match(license_header_uncommented_concat_1)
                start_year = copyright_notice_match.group(1)
                specifically_clause = copyright_notice_match.group(2)
                also_clause = copyright_notice_match.group(3)
                if start_year == current_year:
                    output_copyright_notice += "Copyright (C) " + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nAlso: " + also_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    output_copyright_notice += "Copyright (C) " + start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nAlso: " + also_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                match_group_split_lines = copyright_notice_match.group(0).splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            elif COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_REGEX.match(license_header_uncommented_concat_1):
                print(f"File '{filepath}': Copyright Vega Contributors with Specifically clause")
                copyright_notice_match: re.Match[str] = COPYRIGHT_VEGA_CONTRIBUTORS_WITH_SPECIFICALLY_REGEX.match(license_header_uncommented_concat_1)
                start_year = copyright_notice_match.group(1)
                specifically_clause = copyright_notice_match.group(2)
                if start_year == current_year:
                    output_copyright_notice += "Copyright (C) " + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    output_copyright_notice += "Copyright (C) " + start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                match_group_split_lines = copyright_notice_match.group(0).splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            elif COPYRIGHT_VEGA_CONTRIBUTORS_REGEX.match(license_header_uncommented_concat_1):
                print(f"File '{filepath}': Copyright Vega Contributors without Specifically")
                copyright_notice_match: re.Match[str] = COPYRIGHT_VEGA_CONTRIBUTORS_REGEX.match(license_header_uncommented_concat_1)
                start_year = copyright_notice_match.group(1)
                if start_year == current_year:
                    output_copyright_notice += "Copyright (C) " + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    output_copyright_notice += "Copyright (C) " + start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                match_group_split_lines = copyright_notice_match.group(0).splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            elif GENERIC_VEGA_COPYRIGHT_REGEX.match(license_header_uncommented_concat_1):
                print(f"File '{filepath}': Generic Vega Strike Copyright")
                found_current_year: bool = False
                license_header_uncommented_split_lines_1 = license_header_uncommented_concat_1.splitlines()
                license_header_uncommented_split_lines_1.pop(0)
                for current_copyright_line in license_header_uncommented_split_lines_1:
                    if BASIC_COPYRIGHT_REGEX.match(current_copyright_line) or LARGELY_REWRITTEN_BY_REGEX.match(current_copyright_line) or VEGA_STRIKE_CONTRIBUTORS_REGEX.match(current_copyright_line):
                        output_copyright_notice += current_copyright_line + '\n'
                        license_header_uncommented_lines.pop(0)
                        if copyright_current_year_regex.match(current_copyright_line):
                            found_current_year = True
                        elif copyright_current_year_regex_2.match(current_copyright_line):
                            found_current_year = True
                    else:
                        license_header_uncommented_lines.pop(0)
                        break
                if not found_current_year:
                    output_copyright_notice += "Copyright (C) " + current_year + " The Vega Strike Contributors\n"
            elif GENERIC_MULTI_COPYRIGHT_REGEX.match(license_header_uncommented_concat_1):
                print(f"File '{filepath}': Generic multi-line copyright")
                found_current_year: bool = False
                license_header_uncommented_split_lines_1 = license_header_uncommented_concat_1.splitlines()
                for current_copyright_line in license_header_uncommented_split_lines_1:
                    if BASIC_COPYRIGHT_REGEX.match(current_copyright_line) or LARGELY_REWRITTEN_BY_REGEX.match(current_copyright_line) or VEGA_STRIKE_CONTRIBUTORS_REGEX.match(current_copyright_line):
                        output_copyright_notice += current_copyright_line + '\n'
                        license_header_uncommented_lines.pop(0)
                        if copyright_current_year_regex.match(current_copyright_line):
                            found_current_year = True
                        elif copyright_current_year_regex_2.match(current_copyright_line):
                            found_current_year = True
                    else:
                        license_header_uncommented_lines.pop(0)
                        break
                if not found_current_year:
                    output_copyright_notice += "Copyright (C) " + current_year + " The Vega Strike Contributors\n"

            # Trim leading blank lines again after copyright clause(s)
            while len(license_header_uncommented_lines) > 0 and license_header_uncommented_lines[0] == '':
                license_header_uncommented_lines.pop(0)

            license_header_uncommented_concat_2 = '\n'.join(license_header_uncommented_lines)

            if THIS_FILE_IS_PART_OF_VEGA_STRIKE_REGEX.match(license_header_uncommented_concat_2):
                print(f"File '{filepath}': This file is part of Vega Strike")
                output_copyright_notice += THIS_FILE_IS_PART_OF_VEGA_STRIKE_TEXT
                match_group_split_lines = license_header_uncommented_concat_2.splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            elif INCORPORATED_FROM_OPCODE_PUBLIC_DOMAIN_REGEX.match(license_header_uncommented_concat_2):
                print(f"File '{filepath}': Incorporated from OPCODE; Public Domain")
                output_copyright_notice += INCORPORATED_FROM_OPCODE_PUBLIC_DOMAIN_TEXT
                match_group_split_lines = license_header_uncommented_concat_2.splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            elif LGPL_REGEX.match(license_header_uncommented_concat_2):
                print(f"File '{filepath}': GNU LGPL")
                output_copyright_notice += LGPL_TEXT
                match_group_split_lines = license_header_uncommented_concat_2.splitlines()
                for i in match_group_split_lines:
                    license_header_uncommented_lines.pop(0)
            else:
                print(f"File '{filepath}' did not match any of the expected copyright header patterns")
                output_file.close()
                Path.unlink(Path(output_file.name))
                return

            if individually_commented:
                output_file.write(comment_block(output_copyright_notice, script_like_file) + '\n')
            else:
                output_file.write(comment_block(output_copyright_notice, script_like_file))
            if len(already_read_lines) > 0:
                output_file.write(already_read_lines)
            output_file.write(input_file.read())

    # Copy original file attributes and permissions to temp file
    copystat(filepath, output_file.name)
    # Move temp file into place
    move(output_file.name, filepath)


def main():
    if len(sys.argv) > 1 and sys.argv[1] in ('-h', '--help'):
        print(__doc__)
        return
    elif len(sys.argv) > 1:
        files = sys.argv[1:]
    else:
        files = sys.stdin

    for filepath in map(Path, (f.removesuffix('\n') for f in files)):
        upsert_license_header(filepath)

if __name__ == '__main__':
    main()
