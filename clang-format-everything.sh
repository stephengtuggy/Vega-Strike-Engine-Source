#! /usr/bin/env bash

find . -type f -iname "*.h" -print0 | xargs -0 clang-format-8 -i
clang-format-8 -i stringsort.cpp
find launcher/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find mission/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find objconv/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find saveinterface/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find setup/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find src/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find tools/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i
find vs_cubemap_gen/ -type f -iname '*.cpp' -print0 | xargs -0 clang-format-8 -i

