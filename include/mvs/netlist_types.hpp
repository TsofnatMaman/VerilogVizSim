#pragma once
#include <string>
#include <vector>
#include <optional>
#include <map>
#include "module.hpp"

namespace mvs
{
    enum class GateType
    {
        AND, OR, XOR, NOT, CONSTANT, IDENTITY 
    };

    struct NetlistComponent
    {
        std::string output_wire;
        GateType type;
        std::vector<std::string> input_wires;
        std::optional<int> constant_value; 
    };

    using Netlist = std::vector<NetlistComponent>;
}