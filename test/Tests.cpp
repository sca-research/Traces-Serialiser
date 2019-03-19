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
 *  @file Tests.hpp
 *  @brief Contains the entry point for the tests
 *  @author Scott Egerton
 *  @date 2017-2018
 *  @copyright GNU Affero General Public License Version 3+
 */

//! Required when using Catch testing framework - Tells Catch to provide a
//! main()
//! @see https://github.com/catchorg/Catch2
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif  // CATCH_CONFIG_MAIN

#include <catch.hpp>  // for catch

#include "Traces_Serialiser.hpp"  // for Serialiser

// Instantiate all expected templates in order to include these in coverage
// information.
template class Traces_Serialiser::Serialiser<std::uint8_t>;
template class Traces_Serialiser::Serialiser<std::uint16_t>;
template class Traces_Serialiser::Serialiser<std::uint32_t>;
template class Traces_Serialiser::Serialiser<float>;

// TODO: Document
const std::string load_file(const std::string& p_file_path)
{
    std::ifstream file(p_file_path, std::ios::binary);
    std::string actual_result;

    // TRS files should only ever be one line
    std::getline(file, actual_result);

    return actual_result;
}

// The actual tests
#include "Test_Adding_Traces.hpp"
#include "Test_Constructors.hpp"
#include "Test_Different_Length_Traces.hpp"
#include "Test_Traces_Serialiser.hpp"
#include "Test_Traces_Types.hpp"
