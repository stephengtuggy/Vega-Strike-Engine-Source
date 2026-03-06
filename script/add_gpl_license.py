#!/usr/bin/env python3
##
# add_gpl_license.py
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

LICENSE_TEXT = """
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

LICENSE_TEXT_REGEX = re.compile(r"""
https://github\.com/vegastrike/Vega-Strike-Engine-Source

This file is part of Vega Strike\.

Vega Strike is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
\(at your option\) any later version\.

Vega Strike is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\.\s+See the
GNU General Public License for more details\.

You should have received a copy of the GNU General Public License
along with Vega Strike\.\s+If not, see <https://www\.gnu\.org/licenses/>\.
""")

COPYRIGHT_NOTICE_YEAR_RANGE = re.compile(r"""
Vega Strike - Space Simulation, Combat and Trading
Copyright \([cC]\) (\d{4})-\d{4} The Vega Strike Contributors:
Project creator: Daniel Horn
Original development team: As listed in the AUTHORS file
Current development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy
""")

COPYRIGHT_NOTICE_SINGLE_YEAR = re.compile(r"""
Vega Strike - Space Simulation, Combat and Trading
Copyright \([cC]\) (\d{4}) The Vega Strike Contributors:
Project creator: Daniel Horn
Original development team: As listed in the AUTHORS file
Current development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy
""")

COPYRIGHT_NOTICE_WITH_SPECIFICALLY_YEAR_RANGE = re.compile(r"""
Vega Strike - Space Simulation, Combat and Trading
Copyright \([cC]\) (\d{4})-\d{4} The Vega Strike Contributors:
Project creator: Daniel Horn
Original development team: As listed in the AUTHORS file. Specifically: ([\w\s.,]+)
Current development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy
""")

COPYRIGHT_NOTICE_WITH_SPECIFICALLY_SINGLE_YEAR = re.compile(r"""
Vega Strike - Space Simulation, Combat and Trading
Copyright \([cC]\) (\d{4}) The Vega Strike Contributors:
Project creator: Daniel Horn
Original development team: As listed in the AUTHORS file. Specifically: ([\w\s.,]+)
Current development team: Roy Falk, Benjamen R\. Meyer, Stephen G\. Tuggy
""")

# Characters to use for the start, middle and end of a comment block
C_LIKE_COMMENT = ['/*', ' *', ' */', '/**', '*', '*/']
C_LIKE_COMMENT_BLOCK_START_REGEX = re.compile(r'^/\*+(.*)\r?\n')
C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX = re.compile(r'^\s*\**\s*(.*)\r?\n')
C_LIKE_COMMENT_BLOCK_END_REGEX = re.compile(r'^\s*(.*)\s*\*/\r?\n')
C_LIKE_INDIVIDUAL_COMMENT_REGEX = re.compile(r'^\s*//\s*(.*)$')
C_LIKE_BASIC_OFFSET_HEADER_COMMENT_REGEX = re.compile(r'^\s*//\s*-\*-\s*mode:\s*(.*)$')

SCRIPT_LIKE_COMMENT = ['##', '#', '#']
SCRIPT_LIKE_COMMENT_REGEX = re.compile(r'^\s*#+\s*(.*)$')

SHEBANG_REGEX = re.compile(r'^#!(.*)$')

C_LIKE_COMMENTS = ['.c', '.cpp', '.h', '.hpp', '.h.in']
SCRIPT_LIKE_COMMENTS = ['.py', '.cmake', '.txt', '.sh', '.ps1']


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


def is_middle_of_a_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line) or C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def is_end_of_a_comment(line: str, script_like_file: bool) -> bool:
    if script_like_file:
        return SCRIPT_LIKE_COMMENT_REGEX.match(line)
    else:
        return C_LIKE_COMMENT_BLOCK_END_REGEX.match(line) or C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line)


def uncomment_start(line: str, script_like_file: bool) -> str:
    if script_like_file and SCRIPT_LIKE_COMMENT_REGEX.match(line):
        return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(1)
    elif C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
        return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(1)
    elif C_LIKE_COMMENT_BLOCK_START_REGEX.match(line):
        return C_LIKE_COMMENT_BLOCK_START_REGEX.match(line).group(1)
    else:
        print(f"'{line}' is not the start of a comment")
        return line


def uncomment_middle(line: str, script_like_file: bool) -> str:
    if script_like_file and SCRIPT_LIKE_COMMENT_REGEX.match(line):
        return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(1)
    elif C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
        return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(1)
    elif C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line):
        return C_LIKE_COMMENT_BLOCK_MIDDLE_REGEX.match(line).group(1)
    else:
        print(f"'{line}' is not the middle of a comment")
        return line


def uncomment_end(line: str, script_like_file: bool) -> str:
    if script_like_file and SCRIPT_LIKE_COMMENT_REGEX.match(line):
        return SCRIPT_LIKE_COMMENT_REGEX.match(line).group(1)
    elif C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line):
        return C_LIKE_INDIVIDUAL_COMMENT_REGEX.match(line).group(1)
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
            ret_val += f"# {line}\n"
        ret_val += "#\n"
        return ret_val
    else:
        ret_val = "/*\n"
        for line in individual_lines:
            ret_val += f" * {line}\n"
        ret_val += " */\n"
        return ret_val


def upsert_license_header(filepath: Path) -> None:
    """Add GPL license notice at the start of the given file, if not present, or update it if present."""
    print(f"Adding/updating license header at start of file {filepath}")

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

    with filepath.open('r') as input_file:
        with NamedTemporaryFile('w', delete=False, newline='\n') as output_file:
            first_line: str = input_file.readline()
            if SHEBANG_REGEX.match(first_line):
                script_like_file = True
                output_file.write(first_line)
                second_line: str = input_file.readline()
                if is_start_of_a_comment(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines += uncomment_start(second_line, script_like_file)

            elif not script_like_file and C_LIKE_BASIC_OFFSET_HEADER_COMMENT_REGEX.match(first_line):
                output_file.write(first_line)
                second_line: str = input_file.readline()
                if is_start_of_a_comment(second_line, script_like_file):
                    license_header_commented += second_line
                    in_license_header_comment = True
                    license_header_uncommented_lines += uncomment_start(second_line, script_like_file)

            elif is_start_of_a_comment(first_line, script_like_file):
                license_header_commented += first_line
                in_license_header_comment = True
                license_header_uncommented_lines += uncomment_start(first_line, script_like_file)

            else:
                pass

            while in_license_header_comment:
                current_line: str = input_file.readline()

                if not current_line:
                    # We've reached the end of the initial comment block
                    in_license_header_comment = False
                    break

                if is_middle_of_a_comment(current_line, script_like_file):
                    license_header_commented += current_line
                    license_header_uncommented_lines += uncomment_middle(current_line, script_like_file)

                if is_end_of_a_comment(current_line, script_like_file):
                    # We've reached the end of the initial comment block
                    license_header_commented += current_line
                    license_header_uncommented_lines += uncomment_end(current_line, script_like_file)
                    in_license_header_comment = False
                    break

            while len(license_header_uncommented_lines) > 0 and license_header_uncommented_lines[0] == '':
                license_header_uncommented_lines.pop(0)

            if len(license_header_uncommented_lines) == 0 or license_header_uncommented_lines[0] != filepath.name:
                print(f"Header at top of file {filepath} does not start with filename")
                output_file.close()
                Path.unlink(Path(output_file.name))
                return
            else:
                output_file.write(filepath.name)
                license_header_uncommented_lines.pop(0)
                if len(license_header_uncommented_lines) == 0:
                    pass
                elif not license_header_uncommented_lines[0]:
                    license_header_uncommented_lines.pop(0)

            license_header_uncommented_concat = '\n'.join(license_header_uncommented_lines)
            start_year: str = "2001"
            current_year: str = datetime.now(timezone.utc).strftime("%Y")
            copyright_notice: str = "Vega Strike - Space Simulation, Combat and Trading\nCopyright (C) "

            if COPYRIGHT_NOTICE_YEAR_RANGE.match(license_header_uncommented_concat):
                copyright_notice_match: re.Match[str] = COPYRIGHT_NOTICE_YEAR_RANGE.match(license_header_uncommented_concat)
                start_year = copyright_notice_match.group(1)
                if start_year == current_year:
                    copyright_notice += current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    copyright_notice += start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                for i in range(0, copyright_notice.count('\n')):
                    license_header_uncommented_lines.pop(0)
            elif COPYRIGHT_NOTICE_SINGLE_YEAR.match(license_header_uncommented_concat):
                copyright_notice_match: re.Match[str] = COPYRIGHT_NOTICE_SINGLE_YEAR.match(license_header_uncommented_concat)
                start_year = copyright_notice_match.group(1)
                if start_year == current_year:
                    copyright_notice += current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    copyright_notice += start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                for i in range(0, copyright_notice.count('\n')):
                    license_header_uncommented_lines.pop(0)
            elif COPYRIGHT_NOTICE_WITH_SPECIFICALLY_YEAR_RANGE.match(license_header_uncommented_concat):
                copyright_notice_match: re.Match[str] = COPYRIGHT_NOTICE_WITH_SPECIFICALLY_YEAR_RANGE.match(license_header_uncommented_concat)
                start_year = copyright_notice_match.group(1)
                specifically_clause = copyright_notice_match.group(2)
                if start_year == current_year:
                    copyright_notice += current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    copyright_notice += start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                for i in range(0, copyright_notice.count('\n')):
                    license_header_uncommented_lines.pop(0)
            elif COPYRIGHT_NOTICE_WITH_SPECIFICALLY_SINGLE_YEAR.match(license_header_uncommented_concat):
                copyright_notice_match: re.Match[str] = COPYRIGHT_NOTICE_WITH_SPECIFICALLY_SINGLE_YEAR.match(license_header_uncommented_concat)
                start_year = copyright_notice_match.group(1)
                specifically_clause = copyright_notice_match.group(2)
                if start_year == current_year:
                    copyright_notice += current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"
                else:
                    copyright_notice += start_year + "-" + current_year + " The Vega Strike Contributors:\nProject creator: Daniel Horn\nOriginal development team: As listed in the AUTHORS file. Specifically: " + specifically_clause + "\nCurrent development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy\n"

                for i in range(0, copyright_notice.count('\n')):
                    license_header_uncommented_lines.pop(0)
            else:
                print(f"File {filepath} did not match any of the expected copyright header patterns")
                output_file.close()
                Path.unlink(Path(output_file.name))
                return

            while len(license_header_uncommented_lines) > 0 and license_header_uncommented_lines[0] == '':
                license_header_uncommented_lines.pop(0)
            copyright_notice += '\n\n'
            if LICENSE_TEXT_REGEX.match('\n'.join(license_header_uncommented_lines)):
                copyright_notice += LICENSE_TEXT
            else:
                print(f"Last few lines of copyright header in file '{filepath}' did not match expected value")
                output_file.close()
                Path.unlink(Path(output_file.name))
                return

            output_file.write(comment_block(copyright_notice, script_like_file))
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
