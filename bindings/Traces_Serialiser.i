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
    %template(vector_8) vector<uint8_t>;
    %template(vector_16) vector<uint16_t>;
    %template(vector_32) vector<uint32_t>;
    %template(vector_float) vector<float>;
}

%include "Traces_Serialiser.hpp"

namespace Traces_Serialiser {
    %template(Serialiser_8) Serialiser<uint8_t>;
    %template(Serialiser_16) Serialiser<uint16_t>;
    %template(Serialiser_32) Serialiser<uint32_t>;
    %template(Serialiser_float) Serialiser<float>;
    %template(Serialiser_double) Serialiser<double>;
}
