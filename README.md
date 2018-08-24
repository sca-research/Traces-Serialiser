# Traces Serialiser

Traces Serialiser is a tool that helps to generate
[side channel analysis](https://en.wikipedia.org/wiki/Side-channel_attack)
traces in a format compatible with
[Riscure's Inspector tool.](https://www.riscure.com/security-tools/inspector-sca/)

Traces Serialiser is a C++, single header library that depends on nothing other
than the standard library.

- [Traces Serialiser](#traces-serialiser)
  * [Usage](#usage)
    + [Usage (C++)](#usage-c)
      - [Example Usage (C++)](#example-usage-c)
    + [Usage (Python)](#usage-python)
      - [Example Usage (Python)](#example-usage-python)
    + [Other Languages](#other-languages)
  * [API Documentation](#api-documentation)
  * [Getting started for development](#getting-started-for-development)
    + [Prerequisites](#prerequisites)
      - [Linux](#linux)
      - [MacOS](#macos)
      - [Windows](#windows)
    + [Building](#building)
    + [Language Bindings](#language-bindings)
    + [Running tests](#running-tests)
      - [Coverage information](#coverage-information)
  * [Configuration Options](#configuration-options)
  * [Built with](#built-with)
  * [License](#license)

## Usage

### Usage (C++)

1) The only file that needs to be included in your program is
`Traces_Serialiser.hpp` located in the directory `src`.

2) To make use of it, add this line to your program
```cpp
#include "Traces_Serialiser.hpp"
```
This requires **C++17** or later.

#### Example Usage (C++)

This is the most basic way to use the library at the moment.
```cpp
Traces_Serialiser::Serialiser<float> serialiser(number_of_traces,
                                                samples_per_trace,
                                                sample_coding,
                                                traces);

serialiser.Save("/file/path/to/save/to");
```
The template parameter refers to the type of the traces. In the above example, `<float>` is used, indicating that the `traces` object is a vector of float values. Any arithmetic type can be used here.

Adding additional headers is simple as all headers have a custom function
(at the time of writing). Here is an example of a few of them. A full list is
available in the [API Documentation.](#api-documentation)

```cpp
Traces_Serialiser::Serialiser<uint8_t> serialiser(number_of_traces,
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
Traces_Serialiser::Serialiser<uint32_t> serialiser(number_of_traces,
                                                   samples_per_trace,
                                                   sample_coding, traces);

serialiser.Add_Header(Traces_Serialiser::Serialiser<uint16_t>::Tag_Trace_Title,
                      "My traces");

serialiser.Add_Header(0x48, 7);

serialiser.Save("/file/path/to/save/to");
```

### Usage (Python)

1) Follow the instructions in the
[Language Bindings section](#language-bindings) in order to generate
python bindings.

2) Two files are required: `Traces_Serialiser.py` and `_Traces_Serialiser.so`
located in the directory 
```
/path/to/build/directory/output/bindings/python
```
3) To make use of it, add this line to your program
```python
import Traces_Serialiser
```

#### Example Usage (Python)

Using the python bindings is slightly more complicated than in C++ but it is
mostly the same.
```python
serialiser = Traces_Serialiser.Serialiser_float(number_of_traces,
	                                        samples_per_trace,
	                                        sample_coding,
                                                traces)

serialiser.Save("/file/path/to/save/to")
```
Instead of templates, there are four separate python classes. Serialiser_8, Serialiser_16 and Serialiser_32 should be used to store unsigned 8, 16 and 32 bit values. Serialiser_float should be used to store floating point values.

vector_8, vector_16, vector_32 and vector_float objects are provided as an alternative format for the traces to be in. These are exactly the same as a [C++ vector.](https://en.cppreference.com/w/cpp/container/vector)
```python
traces = Traces_Serialiser.vector_8()

# Add the traces to this object however you like.
traces.push_back(trace)
...

serialiser = Traces_Serialiser.Serialiser_8(number_of_traces,
                                            samples_per_trace,
                                            sample_coding,
                                            traces)

serialiser.Save("/file/path/to/save/to")
```

Adding additional headers is simple as all headers have a custom function
(at the time of writing). Here is an example of a few of them. A full list is
available in the [API Documentation.](#api-documentation)

```python
serialiser = Traces_Serialiser.Serialiser_32(number_of_traces,
                                             samples_per_trace,
                                             sample_coding,
                                             traces)

serialiser.Set_Trace_Title("My traces")
serialiser.Set_Axis_Scale_Y(0.5f)
serialiser.Set_External_Clock_Used()

serialiser.Save("/file/path/to/save/to")
```

Custom headers can also be added through the `Add_Header` method, however this
is not recommended as it is more error prone.
```cpp
serialiser = Traces_Serialiser.Serialiser_16(number_of_traces,
                                             samples_per_trace,
                                             sample_coding, traces)

serialiser.Add_Header(Traces_Serialiser.Serialiser_8.Tag_Trace_Title,
                      "My traces")

serialiser.Add_Header(0x48, 7)

serialiser.Save("/file/path/to/save/to")
```

### Other Languages

Support for other languages can be added upon request providing they are
[supported by SWIG.](http://www.swig.org/compat.html#SupportedLanguages)

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

**The library does not need to be built to be used.** This section is only
needed for building tests, language bindings and documentation.

1) **Create an empty build directory.**

2) **From the build directory run this command.** This will generate native
build files for your platform. A specific generator can be specified using the
`-G *generator*` flag. A [list of generators is available here.]
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

### Language Bindings

In order to support languages other than C++, wrapper code needs to be
generated. This can be done using [SWIG.](http://www.swig.org/)

1) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

2) **Ensure that [SWIG](www.swig.org/download.html) is installed.**

3) **Generate the bindings with this command.** This will tell the native build
system to build the target called bindings which will run SWIG for you.
```
cmake --build . --target bindings
```

### Running tests

1) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

2) **Build the tests with this command.** This will tell the native build system
to build the target called tests.
```
cmake --build . --target tests
```
3) **Run the tests**
```
/path/to/build/directory/output/tests
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
- [SWIG](http://www.swig.org/)
- [markdown-toc](https://github.com/Ecotrust-Canada/markdown-toc)
- [Text to ASCII Art Generator](http://patorjk.com/software/taag/#f=ANSI Shadow)

## License
This program is released under license AGPLv3+.
