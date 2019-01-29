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

TEST_CASE("Adding traces"
          "[!throws][traces]")
{
    SECTION("Basic constructor")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {0, 1, 2, 3, 4}, 1, 5, 1));
    }

    SECTION("2 bytes trace size")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
             12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
            1,
            24,
            0x02));
    }

    SECTION("32 bit traces")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint32_t>(
            {0, 1000, 299999999, 312789, 498210113}, 1, 5, 4));
    }

    // TODO: Test if float traces are actually saved correctly.
    SECTION("Float traces")
    {
        REQUIRE_NOTHROW(
            Traces_Serialiser::Serialiser<float>({0.22548f, 0.001f}, 1, 2, 4));
    }

    // TODO: Work out how to add this test. The expected result is that
    // compilation will fail.
    /*
     *SECTION("String traces")
     *{
     *    STATIC_REQUIRE_FALSE(Traces_Serialiser::Serialiser<std::string>(
     *        1, 2, 0x07, {"Hello", "World!"}));
     *}
     */

    SECTION("Constructor without sample length")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {0, 1, 2, 3, 4, 5}, 2, 3));
    }

    SECTION("Constructor without sample length and number of samples per trace")
    {
        REQUIRE_NOTHROW(
            Traces_Serialiser::Serialiser<std::uint8_t>({0, 1, 2, 3, 4, 5}, 2));
    }

    SECTION("2D Constructor")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {{0, 1, 2}, {3, 4, 5}}));
    }

    SECTION("2D Constructor with string extra data")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {"Hello", "World"}, {{0, 1, 2}, {3, 4, 5}}));
    }

    SECTION("2D Constructor with hex extra data")
    {
        REQUIRE_NOTHROW(Traces_Serialiser::Serialiser<std::uint8_t>(
            {"678", "9ab"}, {{0, 1, 2}, {3, 4, 5}}));
    }
}
