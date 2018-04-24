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

#ifndef TRACES_SERIALISER_HPP
#define TRACES_SERIALISER_HPP

#include <cstdint>       // for uint8_t
#include <iomanip>       // for setw, setfill
#include <iostream>      // for cout
#include <string>        // for string
#include <unordered_map> // for map
#include <utility>       // for pair
#include <vector>        // for vector

namespace Traces_Serialiser
{
class Serialiser
{
private:
    std::unordered_map<uint8_t, std::pair<uint8_t, std::vector<uint8_t>>>
        m_headers;

    std::vector<uint8_t> m_traces;

    template <typename T>
    void convert_to_bytes(const T& p_data, std::vector<uint8_t>& out_bytes)
    {
        // Strings need to be handled seperately
        if constexpr (std::is_same<T, std::string>::value)
        {
            out_bytes = {p_data.begin(), p_data.end()};
        }
        else
        {
            // Cast to a byte aray
            const unsigned char* bytes =
                reinterpret_cast<const unsigned char*>(&p_data);

            // Convert byte array to byte vector
            out_bytes = {bytes, bytes + sizeof(T)};

            // Needed to remove trailing 0s
            out_bytes.erase(
                std::remove_if(out_bytes.begin(), out_bytes.end(),
                               [](uint8_t byte) { return byte == 0; }),
                out_bytes.end());
        }
    }

    void validate(const size_t& p_tag)
    {
        // Only allow external clock related values to be set if the external
        // clock has been explicitly enabled.

        // If this is an external clock related header...
        if (Tag_External_Clock_Threshold <= p_tag &&
            Tag_External_Clock_Time_Base >= p_tag)
        {
            // ...and external clock used has not been set...
            if (m_headers.find(Tag_External_Clock_Used) != m_headers.end() ||
                // ...or it has been set to false
                !m_headers[Tag_External_Clock_Used].second.front())
            {
                throw("Enable external clock explicitly with "
                      "Set_External_Clock_Used()");
            }

            // Only allow external clock resampler mask to be set if the
            // external clock has been explicitly enabled.

            // If this is the external clock resampler mask...
            if (Tag_External_Clock_Resampler_Mask == p_tag &&
                // ...and the resampler used has not been set...
                (m_headers.find(Tag_External_Clock_Resampler_Enabled) !=
                     m_headers.end() ||
                 // ...or it has been set to false
                 !m_headers[Tag_External_Clock_Resampler_Enabled]
                      .second.front()))
            {
                throw("Enable external clock resampler explicitly with "
                      "Set_External_Clock_Resampler_Enabled()");
            }
        }
    }

public:
    // Public so user can write code like this: Add_Header(Number_Of_Traces, 4);
    const uint8_t Tag_Number_Of_Traces             = 0x41;
    const uint8_t Tag_Number_Of_Samples_Per_Trace  = 0x42;
    const uint8_t Tag_Sample_Coding                = 0x43;
    const uint8_t Tag_Length_Of_Cryptographic_Data = 0x44;
    const uint8_t Tag_Title_Space_Per_Trace        = 0x45;
    const uint8_t Tag_Trace_Title                  = 0x46;
    const uint8_t Tag_Description                  = 0x47;
    const uint8_t Tag_Axis_Offset_X                = 0x48;
    const uint8_t Tag_Axis_Label_X                 = 0x49;
    const uint8_t Tag_Axis_Label_Y                 = 0x4A;
    const uint8_t Tag_Axis_Scale_X                 = 0x4B;
    const uint8_t Tag_Axis_Scale_Y                 = 0x4C;
    const uint8_t Tag_Trace_Offset                 = 0x4D;
    const uint8_t Tag_Logarithmic_Scale            = 0x4E;
    // 0x4F - 0x54 Reserved for future use.
    const uint8_t Tag_Scope_Range           = 0x55;
    const uint8_t Tag_Scope_Coupling        = 0x56;
    const uint8_t Tag_Scope_Offset          = 0x57;
    const uint8_t Tag_Scope_Input_Impedance = 0x58;
    const uint8_t Tag_Scope_ID              = 0x59;
    const uint8_t Tag_Filter_Type           = 0x5A;
    const uint8_t Tag_Filter_Frequency      = 0x5B;
    const uint8_t Tag_Filter_Range          = 0x5C;
    // 0x5D - 0x5E Undocumented.
    const uint8_t Tag_Trace_Block_Marker               = 0x5F;
    const uint8_t Tag_External_Clock_Used              = 0x60;
    const uint8_t Tag_External_Clock_Threshold         = 0x61;
    const uint8_t Tag_External_Clock_Multiplier        = 0x62;
    const uint8_t Tag_External_Clock_Phase_Shift       = 0x63;
    const uint8_t Tag_External_Clock_Resampler_Mask    = 0x64;
    const uint8_t Tag_External_Clock_Resampler_Enabled = 0x65;
    const uint8_t Tag_External_Clock_Frequency         = 0x66;
    const uint8_t Tag_External_Clock_Time_Base         = 0x67;

    Serialiser(const uint32_t p_number_of_traces,
               const uint32_t p_samples_per_trace,
               const uint8_t p_sample_coding, /* TODO: Maybe change sample
                                                 coding to something easy to
                                                 understand? */
               const std::vector<uint8_t> p_traces)
        : m_headers(), m_traces(p_traces)
    {
        Add_Header(Tag_Number_Of_Traces, p_number_of_traces);
        Add_Header(Tag_Number_Of_Samples_Per_Trace, p_samples_per_trace);
        Add_Header(Tag_Sample_Coding, p_sample_coding);
    }

    // Public to allow user to add new headers that may not have functions
    template <typename T> void Add_Header(const size_t& p_tag, const T& p_data)
    {
        // TODO: Handle case where bit 8 (msb) is set to '0' in object length.
        // See inspector manual for details.

        validate(p_tag);

        // A temporary variable to convert p_data to bytes.
        std::vector<uint8_t> value;

        convert_to_bytes(p_data, value);

        // Don't save empty values
        if (0 == value.size())
        {
            return;
        }

        // Add it to the map of headers.
        m_headers[p_tag] = std::make_pair(value.size(), value);
    }

    void Save(std::string p_file_path)
    {
        // TODO: Add 0x5F - header end in front of Traces
        std::cout << "Headers : " << std::endl;
        for (const auto& elem : m_headers)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << +elem.first << " : " << std::hex << std::setw(2)
                      << std::setfill('0') << +elem.second.first << " : ";
            for (const auto& i : elem.second.second)
            {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << +i
                          << ' ';
            }
            std::cout << "End Header" << std::endl;
        }
        std::cout << "Traces: ";
        std::cout << std::hex << Tag_Trace_Block_Marker << std::endl;
        for (const auto& i : m_traces)
        {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << +i
                      << ' ';
        }
        std::cout << " End Traces" << std::endl << std::endl;
    }

    // TODO: Rename
    void Set_Cryptographic_Data_Length(const uint16_t p_length = 0)
    {
        Add_Header(Tag_Length_Of_Cryptographic_Data, p_length);
    }

    // TODO: Title_Space_Per_Trace

    void Set_Trace_Title(const std::string& p_title = "trace")
    {
        Add_Header(Tag_Trace_Title, p_title);
    }

    void Set_Trace_Description(const std::string& p_description)
    {
        Add_Header(Tag_Description, p_description);
    }

    void Set_Axis_Offset_X(const std::string& p_label)
    {
        Add_Header(Tag_Axis_Offset_X, p_label);
    }

    void Set_Axis_Label_X(const std::string& p_label)
    {
        Add_Header(Tag_Axis_Label_X, p_label);
    }

    void Set_Axis_Label_Y(const std::string& p_label)
    {
        Add_Header(Tag_Axis_Label_Y, p_label);
    }

    void Set_Axis_Scale_X(const float p_scale = 1)
    {
        Add_Header(Tag_Axis_Scale_X, p_scale);
    }

    void Set_Axis_Scale_Y(const float p_scale = 1)
    {
        Add_Header(Tag_Axis_Scale_Y, p_scale);
    }

    void Set_Trace_Offset(const uint32_t p_offset = 0)
    {
        Add_Header(Tag_Trace_Offset, p_offset);
    }

    void Set_Logarithmic_Scale(const uint8_t p_scale = 0)
    {
        Add_Header(Tag_Logarithmic_Scale, p_scale);
    }

    // 0x4F - 0x54 Reserved for future use.

    void Set_Scope_Range(const float p_range = 0)
    {
        Add_Header(Tag_Scope_Range, p_range);
    }

    void Set_Scope_Coupling(const uint32_t p_coupling = 0)
    {
        Add_Header(Tag_Scope_Coupling, p_coupling);
    }

    void Set_Scope_Offset(const float p_offset = 0)
    {
        Add_Header(Tag_Scope_Offset, p_offset);
    }

    void Set_Scope_Input_Impedance(const float p_impedance = 0)
    {
        Add_Header(Tag_Scope_Input_Impedance, p_impedance);
    }

    // TODO: Should this be a string?
    void Set_Scope_ID(const std::string& p_id = 0)
    {
        Add_Header(Tag_Scope_ID, p_id);
    }

    void Set_Filter_Type(const uint32_t p_type = 0)
    {
        Add_Header(Tag_Filter_Type, p_type);
    }

    void Set_Filter_Frequency(const float p_frequency = 0)
    {
        Add_Header(Tag_Filter_Frequency, p_frequency);
    }

    void Set_Filter_Range(const float p_range = 0)
    {
        Add_Header(Tag_Filter_Range, p_range);
    }

    // 0x5D - 0x5E Undocumented
    // 0x5F Marks header end

    void Set_External_Clock_Used(const bool p_used = true)
    {
        Add_Header(Tag_External_Clock_Used, p_used);
    }

    void Set_External_Clock_Threshold(const float p_threshold = 0)
    {
        Add_Header(Tag_External_Clock_Threshold, p_threshold);
    }

    void Set_External_Clock_Multiplier(const uint32_t p_multiplier = 0)
    {
        Add_Header(Tag_External_Clock_Multiplier, p_multiplier);
    }

    void Set_External_Clock_Phase_Shift(const uint32_t p_phase_shift = 0)
    {
        Add_Header(Tag_External_Clock_Phase_Shift, p_phase_shift);
    }

    void Set_External_Clock_Resampler_Mask(const uint32_t p_resampler_mask = 0)
    {
        Add_Header(Tag_External_Clock_Resampler_Mask, p_resampler_mask);
    }

    void
    Set_External_Clock_Resampler_Enabled(const bool p_resampler_enabled = true)
    {
        Add_Header(Tag_External_Clock_Resampler_Enabled, p_resampler_enabled);
    }

    void Set_External_Clock_Frequency(const float p_frequency = 0)
    {
        Add_Header(Tag_External_Clock_Frequency, p_frequency);
    }

    void Set_External_Clock_Time_Base(const float p_time_base = 0)
    {
        Add_Header(Tag_External_Clock_Time_Base, p_time_base);
    }
};
} // namespace Traces_Serialiser
#endif
