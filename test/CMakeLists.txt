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

cmake_minimum_required(VERSION 2.6)

include("${PROJECT_SOURCE_DIR}/cmake/Third_Party.cmake")

get_external_header("catch.hpp"
    "https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp")

option(${PROJECT_NAME}_CALCULATE_COVERAGE "Setup ready to calculate the code \
coverage of the unit tests. This adds profiling compiler flags, disables \
optimisations and adds the 'coverage' target. Requires Gcovr."
    OFF)

if(${PROJECT_NAME}_CALCULATE_COVERAGE)
    # Enable profiling for code coverage
    # PARENT_SCOPE is needed for these flags to persist outside this directory
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -fPIC -fno-inline -fno-inline-small-functions -fno-default-inline" PARENT_SCOPE)
    # Second call is needed to set these in the current directory
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -fPIC -fno-inline -fno-inline-small-functions -fno-default-inline")

    # A custom command to generate a html code coverage report using gcovr. 
    # (https://gcovr.com/)
    # This will create a directory, "coverage", compile and run the tests and
    # then generate the coverage information for those tests.
    add_custom_target(coverage
        # Make a directory to store the coverage report in.
        COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${PROJECT_BINARY_DIR}/coverage
        # Run the tests
        COMMAND ${EXECUTABLE_OUTPUT_PATH}/tests
        # Run Gcovr
        COMMAND gcovr ${PROJECT_BINARY_DIR}
                    -r ${PROJECT_SOURCE_DIR}
                    --exclude-unreachable-branches
                    --filter ${PROJECT_SOURCE_DIR}/src
                    --html
                    --html-details
                    -o ${PROJECT_BINARY_DIR}/coverage/coverage.html
            DEPENDS tests
            COMMENT "Generating HTML coverage report"
    )
endif(${PROJECT_NAME}_CALCULATE_COVERAGE)

# Make test executable
set(TEST_SOURCES Test_Traces_Serialiser.cpp)
add_executable(tests ${TEST_SOURCES})

# Include the src being tested
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/src
    ${EXECUTABLE_OUTPUT_PATH})

target_link_libraries(tests ${PROJECT_NAME})
target_include_directories(tests SYSTEM PRIVATE ${THIRD_PARTY_DIR})

enable_testing()
add_test(NAME Run_Tests COMMAND tests)
