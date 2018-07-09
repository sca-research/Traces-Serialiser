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

# TODO: Create subfolders for each language

FIND_PACKAGE(SWIG)
INCLUDE(${SWIG_USE_FILE}) #TODO: is this needed?

set(SWIG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SWIG_INPUT_FILE Traces_Serialiser.i)

# These warnings come from swig itself... As -Werror is used this fails
# to compile. As these are not related to this project they are disabled.
# TODO: Ensure these are still enabled in src folder
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-all")

# Disable extra clang warnings, if clang is being used.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything")
endif()

if (SWIG_FOUND)
    option(${PROJECT_NAME}_BINDINGS_PYTHON "Generate bindings for use from Python. Requires Swig."
        OFF)

    if(${PROJECT_NAME}_BINDINGS_PYTHON)
        # Recurse into the "python" subdirectory.
        add_subdirectory(python)
    endif()
endif()
