#pragma once
#include "mvs/netlist_types.hpp"
#include "mvs/module.hpp" // נדרש כדי לקבל את מבנה ה-Module
#include <string>

namespace mvs
{
    class NetlistToDotConverter
    {
    public:
        // נשאיר את הפונקציה הזו פשוטה כרגע, היא נדרשת רק ל-gateTypeToString
        static std::string convert(const Netlist& netlist, const Module& module);
        
        // 💡 פונקציה חיונית ל-JSON Bindings
        static std::string gateTypeToString(GateType type);
    };
}