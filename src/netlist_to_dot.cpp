#include "mvs/netlist_to_dot.hpp"
#include <map>
#include <stdexcept>

namespace mvs
{
    // 💡 פונקציה זו משמשת לייצוא JSON
    std::string NetlistToDotConverter::gateTypeToString(GateType type)
    {
        static const std::map<GateType, std::string> gate_map = {
            {GateType::AND, "AND"},
            {GateType::OR, "OR"},
            {GateType::XOR, "XOR"},
            {GateType::NOT, "NOT"},
            {GateType::CONSTANT, "CONST"},
            {GateType::IDENTITY, "ID"}
        };
        try {
            return gate_map.at(type);
        } catch (const std::out_of_range&) {
            return "UNKNOWN";
        }
    }

    // הפונקציה המלאה אינה נדרשת ל-Wasm, אך נכללת לשם השלמות
    std::string NetlistToDotConverter::convert(const Netlist& netlist, const Module& module) {
        return ""; 
    }
}