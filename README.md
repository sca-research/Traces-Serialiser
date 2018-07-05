# Traces_Serialiser

Traces Serialiser is a tool that helps to generate
[side channel analysis](https://en.wikipedia.org/wiki/Side-channel_attack)
traces in a format compatible with
[Riscure's Inspector tool.](https://www.riscure.com/security-tools/inspector-sca/)

Traces Serialiser is a C++, single header library that depends on nothing other
than the standard library.

- [Traces_Serialiser](#traces-serialiser)
  * [Usage](#usage)
    + [Example Usage](#example-usage)
  * [API Documentation](#api-documentation)
  * [Getting started for development](#getting-started-for-development)
    + [Prerequisites](#prerequisites)
      - [Linux](#linux)
      - [MacOS](#macos)
      - [Windows/Linux](#windows-linux)
    + [Building](#building)
    + [Running tests](#running-tests)
      - [Coverage information](#coverage-information)
  * [Configuration Options](#configuration-options)
  * [Built with](#built-with)
  * [License](#license)


## Usage

1) The only file that needs to be included in your program is
`Traces_Serialiser.hpp` located in the directory `src`.

2) To make use of it, add this line to your program
```cpp
#include "Traces_Serialiser.hpp"
```

This requires **C++17** or later.

### Example Usage

This is the most basic way to use the library at the moment.
```cp
Traces_Serialiser::Serialiser serialiser(number_of_traces,
                                         samples_per_trace,
                                         sample_coding,
                                         traces);p

serialiser.Save("/file/path/to/save/to");
```

Adding additional headers is simple as all headers have a custom function
(at the time of writing). Here is an example of a few of them. A full list is
available in the [API Documentation.](#api-documentation)

```cpp
Traces_Serialiser::Serialiser serialiser(number_of_traces,
                                         samples_per_trace,
                                         sample_coding,
                                         traces);

serialiser.Set_Trace_Title("My traces");
serialiser.Set_Axis_Scale_Y(0.5f);
serialiser.Set_External_Clock_Used();

serialiser.Save("/file/path/to/save/to");
```

Custom headers can also be added through the `Add_Header` method, however this
is not recommended as it is more error prone.
```cpp
Traces_Serialiser::Serialiser serialiser(number_of_traces,
                                         samples_per_trace,
                                         sample_coding, traces);

serialiser.Add_Header(Traces_Serialiser::Serialiser::Tag_Trace_Title,
                      "My traces");

serialiser.Add_Header(0x48, 7);

serialiser.Save("/file/path/to/save/to");
```

## API Documentation

Documentation is generated using
[Doxygen.](https://www.stack.nl/~dimitri/doxygen) In order to generate this
follow these instructions:

1) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

2) Documentation can be built with CMake using the "doc" target by **running the
command as shown.**
```
cmake --build . --target doc
```

3) **Open this file**
```
/path/to/build/directory/doc/html/index.html
```

Additionally, the cmake configuration option
`Traces-Serialiser_BUILD_DOCUMENTATION` will generate the documentation every
time you compile.

## Getting started for development

### Prerequisites

This is compiling using [CMake.](https://cmake.org/) As such, CMake needs to be
installed first.

#### Linux

The recommended way of installing CMake is through your package manager.

Other distributions may have packages available. If a CMake package isn't
available on your Linux distribution follow the
[official CMake install instructions.](https://cmake.org/install/)

##### APT (Ubuntu/Linux Mint/Debian)

```
sudo apt install cmake
```

##### YUM (Fedora/CentOS/RHEL)

```
sudo yum install cmake
```

##### Pacman (Arch)

```
sudo pacman -S cmake
```

#### MacOS

CMake can be installed using [Homebrew](https://brew.sh/) with the following
command.
```
brew install cmake
```
Alternatively, follow the
[official CMake install instructions.](https://cmake.org/install/)

#### Windows

Follow the [official CMake install instructions.](https://cmake.org/install/)

### Building

**The library does not need to be built to be used.** This section is only needed
for building tests and documentation.

1) **Create an empty build directory.**

2) **From the build directory run this command.** This will generate native
build files for your platform. A specific generator can be specified using the
"-G *generator*" flag. A [list of generators is available here.]
(https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
```
cmake /path/to/source/directory/
```

3) **Run this command to compile the code.** This will compile the program using
your chosen generator e.g. make. Alternatively the native build system can be
used manually from this point on.
```
cmake --build .
```

### Running tests

1) **Build the tests with this command.** This will tell the native build system
to build the target called tests.
```
cmake --build . --target tests
```
2) **Run the tests**
```
/path/to/build/directory/bin/tests
```

#### Coverage information

In order to generate code coverage information, [Gcovr](https://gcovr.com/) is
required.

1) **Regenerate the build files** using the option shown below from the build
directory. This is needed as specific compile flags need to be added in order to
generate coverage information.
```
cmake /path/to/source/directory/traces_serialiser -DTraces-Serialiser_CALCULATE_COVERAGE=ON
```

2) **Run this command.** This will tell the native build system to build the
target called coverage.
```
cmake --build . --target coverage
```
Coverage information can be found by opening the file `coverage.html` in the
folder `coverage` which should have been created in your *`build directory`*.
```
/path/to/build/directory/coverage/coverage.html
```

## Configuration Options

These are all CMake options and can be appended to the CMake generate command.
For example, from the build directory:
```
cmake /path/to/source/directory/ -DTraces-Serialiser_BUILD_DOCUMENTATION
```

### Traces-Serialiser_BUILD_DOCUMENTATION

This option will always built the "doc" target when the target "all" is built.
See [API Documentation section.](#api-documentation)

### Traces-Serialiser_UPDATE_EXTERNAL_HEADERS

This will force all external header files to be redownloaded from their
respective sources when building. Currently this is only used for the
[Catch2](https://github.com/catchorg/Catch2) testing library.

### Traces-Serialiser_CALCULATE_COVERAGE

## Built with

- C++
- [CMake](https://cmake.org/)
- [Catch2](https://github.com/catchorg/Catch2)
- [Gcovr](https://gcovr.com/)
- [Doxygen](https://www.stack.nl/~dimitri/doxygen)

## License
This program is released under license AGPLv3+.
