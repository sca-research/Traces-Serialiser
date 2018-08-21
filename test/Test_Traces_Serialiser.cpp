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

#define CATCH_CONFIG_MAIN

#include <catch.hpp>  // for Section, StringRef, SECTION, Sectio...

#include "Tests.cpp"
#include "Traces_Serialiser.hpp"  // for Serialiser

TEST_CASE("Adding traces"
          "[!throws][traces]")
{
    SECTION("Constructor works")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<uint8_t>(
            1, 5, 0x01, {0, 1, 2, 3, 4}));
    }

    SECTION("2 bytes trace size")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<uint8_t>(
            2400, 1, 0x02, {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                            12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
    }

    SECTION("32 bit traces")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<uint32_t>(
            1, 5, 0x04, {0, 1000, 299999999, 312789, 498210113}));
    }

    // TODO: Test if float traces are actually saved correctly.
    SECTION("Float traces")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<float>(
            1, 2, 0x02, {0.22548f, 0.001f}));
    }
}

TEST_CASE("Adding headers"
          "[!throws][headers]")
{
    std::vector<uint8_t> traces = {0, 1, 2, 3, 4, 5};
    Traces_Serialiser::Serialiser<uint8_t> serialiser(4, 1, 0x02, traces);

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

    SECTION("Set External clock used - implicit true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used());
    }

    SECTION("Set External clock used - explicit true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));
    }

    SECTION("Set External clock used - explicit false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));
    }

    SECTION("Set External clock threashold after setting external clock used "
            "to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Threshold(9.0f));
    }

    SECTION("Set External clock threashold after setting external clock used "
            "to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Threshold(90.080f),
            Catch::Matchers::Contains("Enable external clock explicitly with "
                                      "Set_External_Clock_Used()"));
    }

    SECTION("Set External clock threashold without setting external clock used")
    {
        REQUIRE_THROWS_WITH(
            serialiser.Set_External_Clock_Threshold(9.81f),
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

    SECTION("Set External clock resampler mask after setting external clock "
            "resampler enabled to true")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Resampler_Enabled());

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Resampler_Mask(1));
    }

    SECTION("Set External clock resampler mask after setting external clock "
            "resampler enabled to false")
    {
        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));
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
}
