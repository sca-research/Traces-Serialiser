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

#include <catch.hpp>  // for Section, StringRef, SECTION, Sectio...

#include "Traces_Serialiser.hpp"  // for Serialiser

TEST_CASE("Adding headers"
          "[!throws][headers]")
{
    Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
        {{1, 2, 3}, {4, 5, 6}});

    SECTION("Set Cryptographic Data Length")
    {
        REQUIRE_NOTHROW(serialiser.Set_Cryptographic_Data_Length(2));
    }

    SECTION("Set Trace Title")
    {
        REQUIRE_NOTHROW(serialiser.Set_Trace_Title("ABCD vwxyz."));
    }

    SECTION("Set Axis Scale X")
    {
        REQUIRE_NOTHROW(serialiser.Set_Axis_Scale_X(0.1f));
    }
}

TEST_CASE("Saving traces"
          "[saving]")
{
    constexpr static char file_path[] = "Test_Traces.trs";

    // TODO: Many of the sections in this file can be merged.
    SECTION("8 bit traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
            {{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        const std::vector<std::uint8_t> expected_result = {
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
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
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
        const std::vector<std::uint8_t> expected_result = {
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
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
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
        const std::vector<std::uint8_t> expected_result = {
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
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }
    SECTION("float traces")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<float> serialiser({{1, 2, 3}, {4, 5, 6}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
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
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }
}

TEST_CASE("Saving headers"
          "[saving][headers]")
{
    constexpr static char file_path[] = "Test_Traces.trs";

    // Create some traces and serialise them to a trs file.
    Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
        {{1, 2, 3}, {4, 5, 6}});

    SECTION("Set trace title")
    {
        serialiser.Set_Trace_Title("Hello World! 123");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x46,  // Trace title
            0x10,  // Length
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f,  // Value
            0x72, 0x6c, 0x64, 0x21, 0x20, 0x31, 0x32, 0x33,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set description")
    {
        serialiser.Set_Trace_Description("Hello World! 123");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x47,  // Description
            0x10,  // Length
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f,  // Value
            0x72, 0x6c, 0x64, 0x21, 0x20, 0x31, 0x32, 0x33,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set offset in X axis")
    {
        serialiser.Set_Axis_Offset_X(4081);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x48,  // Offset in X axis
            0x02,  // Length
            0xf1, 0x0f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set label of X axis")
    {
        serialiser.Set_Axis_Label_X("Hello World! 123");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x49,  // X axis label
            0x10,  // Length
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f,  // Value
            0x72, 0x6c, 0x64, 0x21, 0x20, 0x31, 0x32, 0x33,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set label of Y axis")
    {
        serialiser.Set_Axis_Label_Y("Hello World! 123");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x4A,  // Y axis label
            0x10,  // Length
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f,  // Value
            0x72, 0x6c, 0x64, 0x21, 0x20, 0x31, 0x32, 0x33,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scale value for X axis")
    {
        serialiser.Set_Axis_Scale_X(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x4B,  // Scale value for X axis
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scale value for Y axis")
    {
        serialiser.Set_Axis_Scale_Y(0.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x4C,  // Scale value for Y axis
            0x04,  // Length
            0xcd, 0xcc, 0xcc, 0x3d,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set trace offset for displaying numbers")
    {
        serialiser.Set_Trace_Offset(4081);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x4d,  // Trace offset
            0x02,  // Length
            0xf1, 0x0f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set logarithmic scale")
    {
        serialiser.Set_Logarithmic_Scale(0);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x4E,  // Logarithmic scale
            0x01,  // Length
            0x00,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scope range")
    {
        serialiser.Set_Scope_Range(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x55,  // Scope Range
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scope coupling")
    {
        serialiser.Set_Scope_Coupling(4);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x56,  // Scope Coupling
            0x01,  // Length
            0x04,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scope offset")
    {
        serialiser.Set_Scope_Offset(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x57,  // Scope Offset
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scope input impedance")
    {
        serialiser.Set_Scope_Input_Impedance(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x58,  // Scope input impedance
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set scope ID")
    {
        serialiser.Set_Scope_ID("Hello World! 123");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x59,  // Scope input impedance
            0x10,  // Length
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f,  // Value
            0x72, 0x6c, 0x64, 0x21, 0x20, 0x31, 0x32, 0x33,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set filter type")
    {
        serialiser.Set_Filter_Type(1);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x5A,  // Filter type
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
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set filter frequency")
    {
        serialiser.Set_Filter_Frequency(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x5B,  // Filter frequency
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set filter range")
    {
        serialiser.Set_Filter_Range(1.1f);

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x5C,  // Filter range
            0x04,  // Length
            0xcd, 0xcc, 0x8c, 0x3f,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock used - implicit true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used());

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
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
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock used - explicit true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
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
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock used - explicit false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x00,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock threshold after setting external clock used "
            "to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Threshold(90.080f),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock threshold without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Threshold(9.81f),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock multiplier after setting external clock used "
            "to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Multiplier(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock multiplier without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Multiplier(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock phase shift after setting external clock used "
            "to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Phase_Shift(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION(
        "Set External clock phase shift without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Phase_Shift(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock resampler mask after setting external clock "
            "used to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Mask(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock resampler mask without setting external clock "
            "used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Mask(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock resampler enabled after setting external clock "
            "used to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Enabled(true),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock resampler enabled without setting external "
            "clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Enabled(true),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock frequency after setting external clock used to "
            "false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Frequency(3.4f),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock frequency without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Frequency(3.4f),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock time base after setting external clock used to "
            "false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Time_Base(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock time base without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Time_Base(9),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock threshold after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Threshold(9.0f));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x61,  // External clock threshold
            0x04,  // Length
            0x00, 0x00, 0x10, 0x41,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock multiplier after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Multiplier(7));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x62,  // External clock multiplier
            0x01,  // Length
            0x07,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock phase shift after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Phase_Shift(7));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x63,  // External clock phase shift
            0x01,  // Length
            0x07,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock frequency after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Frequency(7));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x66,  // External clock frequency
            0x04,  // Length
            0x00, 0x00, 0xe0, 0x40,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock time base after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Time_Base(7));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x67,  // External clock time base
            0x01,  // Length
            0x07,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock resampler mask after setting external clock "
            "resampler enabled and external clock used to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Resampler_Enabled());

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Resampler_Mask(7));

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x60,  // External clock used
            0x01,  // Length
            0x01,  // Value
            0x64,  // External clock resampler mask
            0x01,  // Length
            0x07,  // Value
            0x65,  // External clock resampler enabled
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
        // clang-format on

        // Ensure that the actual result is the same as the expected
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Set External clock resampler mask after setting external clock "
            "resampler enabled to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used());
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Resampler_Enabled(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Mask(10),
            Catch::Matchers::Contains(
                "Enable external clock resampler explicitly with "
                "Set_External_Clock_Resampler_Enabled()"));
    }

    SECTION("Set External clock resampler mask without setting external clock "
            "resampler enabled")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Resampler_Mask(0),
            Catch::Matchers::Contains(
                "Enable external clock resampler explicitly with "
                "Set_External_Clock_Resampler_Enabled()"));
    }

    SECTION("Set extra long header (> 7 bits length)")
    {
        serialiser.Set_Trace_Description(
            "This is extra long. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x47,  // Description
            0x81,  // Number of bytes length is stored as
            0x82,  // Actual Length
            0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x65,  // Value
            0x78, 0x74, 0x72, 0x61, 0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x2e, 0x20,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x01,  // Start of trace 1
            0x02,
            0x03,
            0x04,  // Start of trace 2
            0x05,
            0x06};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Saving with string extra data")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
            {"Hello", "World"}, {{0, 1, 2}, {3, 4, 5}});
        serialiser.Save(file_path);

        // Load the trs file into a string
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x44,  // Cryptographic data Length
            0x01,  // Length
            0x05,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x48,  // Start of trace 1 extra data
            0x65,
            0x6c,
            0x6c,
            0x6f,
            0x00,  // Start of trace 1
            0x01,
            0x02,
            0x57,  // Start of trace 2 extra data
            0x6f,
            0x72,
            0x6c,
            0x64,
            0x03,  // Start of trace 2
            0x04,
            0x05};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    SECTION("Saving with hex extra data")
    {
        // Create some traces and serialise them to a trs file.
        Traces_Serialiser::Serialiser<std::uint8_t> serialiser(
            {"6789", "abcd"}, {{0, 1, 2}, {3, 4, 5}});
        // serialiser.Save(file_path);
        serialiser.Save(file_path);

        // Load the trs file into a string
        // const std::string actual_result = load_file(file_path);
        const std::string actual_result = load_file(file_path);

        // clang-format off
        const std::vector<std::uint8_t> expected_result = {
            0x41,  // Number of traces
            0x01,  // Length
            0x02,  // Value
            0x42,  // Number of Samples per Trace
            0x01,  // Length
            0x03,  // Value
            0x43,  // Sample Coding
            0x01,  // Length
            0x01,  // Value
            0x44,  // Cryptographic data Length
            0x01,  // Length
            0x02,  // Value
            0x5f,  // Trace Block Marker
            0x00,  // Length (Always 0)
            0x67,  // Start of trace 1 extra data
            0x89,
            0x00,  // Start of trace 1
            0x01,
            0x02,
            0xab,  // Start of trace 2 extra data
            0xcd,
            0x03,  // Start of trace 2
            0x04,
            0x05};
        // clang-format on

        // Ensure that the actual result is the same as the expected result.
        REQUIRE(std::string(std::begin(expected_result),
                            std::end(expected_result)) == actual_result);
    }

    // TODO: Check the sample length is correctly detected for every
    // trace
}
