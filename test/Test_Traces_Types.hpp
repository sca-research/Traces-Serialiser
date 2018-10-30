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
 *  @file Test_Traces_Serialiser.cpp
 *  @brief Contains the tests for the Traces Serialiser.
 *  @author Scott Egerton
 *  @date 2017-2018
 *  @copyright GNU Affero General Public License Version 3+
 */

#include <cstdint>  // for uint8_t, uint16_t, uint32_t
#include <cstring>  // for memcmp
#include <memory>   // for unique_ptr, make_unique

#include <catch.hpp>  // for Section, StringRef, SECTION, Sectio...

#include "Traces_Serialiser.hpp"  // for Serialiser

TEST_CASE("Traces of different types"
          "[saving]")
{
    constexpr static char file_path[] = "Test_Traces.trs";

    SECTION("8 bit traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
            {{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        constexpr static std::uint8_t expected_result[] = {
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
            0x06};

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(0 == memcmp(actual_result.c_str(),
                            expected_result,
                            actual_result.size()));
    }

    SECTION("16 bit traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint16_t> serialiser(
            {{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        constexpr static std::uint8_t expected_result[] = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x02,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x00, 0x01,  // Start of trace 1
            0x00, 0x02,
            0x00, 0x03,
            0x00, 0x04,  // Start of trace 2
            0x00, 0x05,
            0x00, 0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(0 == memcmp(actual_result.c_str(),
                            expected_result,
                            actual_result.size()));
    }

    SECTION("32 bit traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint32_t> serialiser(
            {{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        constexpr static std::uint8_t expected_result[] = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x04,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x00, 0x00, 0x00, 0x01,  // Start of trace 1
            0x00, 0x00, 0x00, 0x02,
            0x00, 0x00, 0x00, 0x03,
            0x00, 0x00, 0x00, 0x04,  // Start of trace 2
            0x00, 0x00, 0x00, 0x05,
            0x00, 0x00, 0x00, 0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(0 == memcmp(actual_result.c_str(),
                            expected_result,
                            actual_result.size()));
    }

    SECTION("float traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<float> serialiser({{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        constexpr static std::uint8_t expected_result[] = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x14,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x00, 0x00, 0x80, 0x3f,  // Start of trace 1
            0x00, 0x00, 0x00, 0x40,
            0x00, 0x00, 0x40, 0x40,
            0x00, 0x00, 0x80, 0x40,  // Start of trace 2
            0x00, 0x00, 0xa0, 0x40,
            0x00, 0x00, 0xc0, 0x40,};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(0 == memcmp(actual_result.c_str(),
                            expected_result,
                            actual_result.size()));
    }

    SECTION("Default to float traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser serialiser({{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        constexpr static std::uint8_t expected_result[] = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x14,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x00, 0x00, 0x80, 0x3f,  // Start of trace 1
            0x00, 0x00, 0x00, 0x40,
            0x00, 0x00, 0x40, 0x40,
            0x00, 0x00, 0x80, 0x40,  // Start of trace 2
            0x00, 0x00, 0xa0, 0x40,
            0x00, 0x00, 0xc0, 0x40,};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(0 == memcmp(actual_result.c_str(),
                            expected_result,
                            actual_result.size()));
    }
}
