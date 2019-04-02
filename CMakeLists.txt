#[=[
    This file is part of Traces-Serialiser.

    Traces-Serialiser is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Traces-Serialiser is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Traces-Serialiser.  If not, see <http://www.gnu.org/licenses/>.
#]=]

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)

# Project's name
project(Traces-Serialiser)

# C++11 is required for constexpr if
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Stricter warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++")

# Even stricter warnings when using clang only
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")

    # Disable unwanted warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat \
                                            -Wno-binary-literal")
endif()

# Coloured compilation output
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")

# Warnings are errors
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# Place executables into a 'bin' folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)

# Recurse into the "test" subdirectory.
# The "test" directory is added before "src" so that compile flags can be added
# if coverage is enabled.
add_subdirectory(test)

# Recurse into the "doc" subdirectory.
add_subdirectory(doc)

# Recurse into the "src" subdirectory.
add_subdirectory(src)

# Recurse into the "bindings" subdirectory.
add_subdirectory(bindings)
