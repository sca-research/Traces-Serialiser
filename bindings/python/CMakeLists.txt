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

INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})  #TODO: Is this needed?

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${SWIG_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)  #TODO: Repalce with var?

# Keep unneeded .cxx wrap files out of the way
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})

SET_SOURCE_FILES_PROPERTIES(${SWIG_DIR}/${SWIG_INPUT_FILE} PROPERTIES CPLUSPLUS ON)

SWIG_ADD_LIBRARY(Traces_Serialiser LANGUAGE python
                     SOURCES ${PROJECT_SOURCE_DIR}/src/Traces_Serialiser.hpp
                     ${SWIG_DIR}/${SWIG_INPUT_FILE})
