/*
 *  This file is part of Traces-Serialiser.
 *
 *  Traces-Serialiser is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Affero General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Traces-Serialiser is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Traces-Serialiser.  If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 *  @file Test_Different_Length_Traces.cpp
 *  @brief Contains the tests for the Traces Serialiser that test if traces
 *  containing a different number of samples can be correctly handled.
 *  @author Scott Egerton
 *  @date 2017-2019
 *  @copyright GNU Affero General Public License Version 3+
 */

#include <cstdint>  // for uint8_t, uint16_t, uint32_t
#include <cstring>  // for memcmp

#include <catch.hpp>  // for Section, StringRef, SECTION, Sectio...

#include "Traces_Serialiser.hpp"  // for Serialiser

TEST_CASE("Different length traces"
          "[!throws][traces]")
{
    constexpr static char file_path[]{"Test_Traces.trs"};

    SECTION("Different length traces - 2 traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser{
            {{1, 2, 3}, {4, 5}}};
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result{load_file(file_path)};

        const std::vector<std::uint8_t> expected_result{
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x00};

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string{std::begin(expected_result),
                            std::end(expected_result)} == actual_result);
    }

    SECTION("Different length traces - 3 traces")
    {
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser{
            {{1, 2, 3}, {4, 5, 6}, {7}}};
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result{load_file(file_path)};

        const std::vector<std::uint8_t> expected_result{
            0x41,               // Number of traces
            0x01,               // Length
            0x03,               // Value
            0x42,               // Number of Samples per Trace
            0x01,               // Length
            0x03,               // Value
            0x43,               // Sample Coding
            0x01,               // Length
            0x01,               // Value
            0x5f,               // Trace Block Marker
            0x00,               // Length (Always 0)
            0x01, 0x02, 0x03,   // Trace 1
            0x04, 0x05, 0x06,   // Trace 2
            0x07, 0x00, 0x00};  // Trace 3

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string{std::begin(expected_result),
                            std::end(expected_result)} == actual_result);
    }

    SECTION("Different length traces - 4 traces")
    {
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser{
            {{1}, {2}, {3, 4}, {5}}};
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result{load_file(file_path)};

        const std::vector<std::uint8_t> expected_result{
            0x41,  // Number of traces
            0x01,  // Length
            0x04,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x02,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,
            0x00,  // Trace 1
            0x02,
            0x00,  // Trace 2
            0x03,
            0x04,  // Trace 3
            0x05,
            0x00};  // Trace 4

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string{std::begin(expected_result),
                            std::end(expected_result)} == actual_result);
    }

    SECTION("Different length traces - blank trace")
    {
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser{
            {{1, 2, 3}, {}, {4, 5, 6}}};
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result{load_file(file_path)};

        const std::vector<std::uint8_t> expected_result{
            0x41,               // Number of traces
            0x01,               // Length
            0x03,               // Value
            0x42,               // Number of Samples per Trace
            0x01,               // Length
            0x03,               // Value
            0x43,               // Sample Coding
            0x01,               // Length
            0x01,               // Value
            0x5f,               // Trace Block Marker
            0x00,               // Length (Always 0)
            0x01, 0x02, 0x03,   // Trace 1
            0x00, 0x00, 0x00,   // Trace 2
            0x04, 0x05, 0x06};  // Trace 3

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string{std::begin(expected_result),
                            std::end(expected_result)} == actual_result);
        ;
    }
}
