#! /usr/bin/env bash

find . \( -not -path './boost/**' -not -path './.git/**' -not -name 'oldcollection*' \) \( -iname '*.h' -or -iname '*.cpp' \) -print0 | xargs -0 clang-format-8 -i
