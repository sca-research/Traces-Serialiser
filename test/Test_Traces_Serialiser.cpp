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

#include <cstdint> // for uint8_t
#include <vector>  // for vector

#include <catch.hpp> // for Section, StringRef, SECTION, Sectio...

#include "Tests.cpp"
#include "Traces_Serialiser.hpp" // for Serialiser

TEST_CASE("Adding headers"
          "[!throws][headers]")
{
    SECTION("Constuctor works")
    {
        REQUIRE_NOTHROW(
            Traces_Serialiser::Serialiser(1, 5, 0x07, {0, 1, 2, 3, 4}));
    }

    SECTION("2 bytes trace size")
    {
        std::vector<uint8_t> traces = {0,  1,  2,  3,  4,  5,  6,  7,
                                       8,  9,  10, 11, 12, 13, 14, 15,
                                       16, 17, 18, 19, 20, 21, 22, 23};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(2400, 1, 0x02, traces);

        serialiser.Save("traces.trs");
    }

    SECTION("Set Cryptographic Data Length")
    {
        std::vector<uint8_t> traces = {0, 1, 2, 3, 4, 5};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(4, 1, 0x02, traces);

        REQUIRE_NOTHROW(serialiser.Set_Cryptographic_Data_Length(2));

        serialiser.Save("traces.trs");
    }

    SECTION("Set Trace Title")
    {
        std::vector<uint8_t> traces = {5};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(1, 1, 0x01, traces);

        REQUIRE_NOTHROW(serialiser.Set_Trace_Title("ABCD vwxyz."));

        serialiser.Save("traces.trs");
    }

    SECTION("Set External clock used - implicit true")
    {
        std::vector<uint8_t> traces = {5, 9};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(2, 4, 0x05, traces);

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used());

        serialiser.Save("traces.trs");
    }

    SECTION("Set External clock used - explicit true")
    {
        std::vector<uint8_t> traces = {5, 9};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(2, 4, 0x05, traces);

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(true));

        serialiser.Save("traces.trs");
    }

    SECTION("Set External clock used - explicit false")
    {
        std::vector<uint8_t> traces = {5, 9};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(2, 4, 0x05, traces);

        REQUIRE_NOTHROW(serialiser.Set_External_Clock_Used(false));

        serialiser.Save("traces.trs");
    }

    SECTION("Set Axis Scale X")
    {
        std::vector<uint8_t> traces = {5, 9};
        Traces_Serialiser::Serialiser serialiser =
            Traces_Serialiser::Serialiser(2, 4, 0x05, traces);

        REQUIRE_NOTHROW(serialiser.Set_Axis_Scale_X(0.1f));

        serialiser.Save("traces.trs");
    }
}
