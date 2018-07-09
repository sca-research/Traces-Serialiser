%module Traces_Serialiser
%{
#include "Traces_Serialiser.hpp"
%}

%include "stdint.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"

namespace std {
    %template(byte_vector) vector<uint8_t>;
}

%include "Traces_Serialiser.hpp"
