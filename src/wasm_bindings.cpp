#include <emscripten/bind.h>
#include <string>
#include <stdexcept>

// 💡 נניח שהנתיך הזה עובד (אם הורדת את json.hpp)
#include "json.hpp" 

#include "mvs/lexer.hpp"
#include "mvs/parser.hpp"
#include "mvs/netlist_extractor.hpp"
#include "mvs/netlist_types.hpp"
#include "mvs/netlist_to_dot.hpp" // נדרש לשימוש ב-gateTypeToString

using namespace emscripten;
using json = nlohmann::json;

// פונקציית עזר להמרת NetlistComponent ל-JSON
json to_json(const mvs::NetlistComponent& comp)
{
    return json{
        {"output", comp.output_wire},
        {"type", mvs::NetlistToDotConverter::gateTypeToString(comp.type)},
        {"inputs", comp.input_wires}
        // אפשר להוסיף גם את constant_value
    };
}

// 💡 הפונקציה המרכזית ש-JavaScript יקרא
std::string generate_netlist_json(const std::string& verilog_source)
{
    try 
    {
        if (verilog_source.empty()) {
            return json{{"error", "Empty Verilog source"}}.dump();
        }
        
        // 1. Tokenization (Lexing)
        mvs::Lexer lexer(verilog_source);
        auto tokens = lexer.Tokenize();
        
        if (tokens.empty()) {
            return json{{"error", "Tokenization resulted in no tokens - possibly empty or whitespace only"}}.dump();
        }
        
        // 2. Parsing
        mvs::Parser parser(tokens);
        std::optional<mvs::Module> module_opt = parser.parseModule();

        if (!module_opt.has_value())
        {
            std::string error_msg = "Parsing failed";
            if (parser.hasError()) {
                error_msg = parser.getErrorMessage();
            } else {
                error_msg = "Unknown parsing error - parseModule() returned empty optional";
            }
            return json{{"error", error_msg}}.dump();
        }
        
        mvs::Module module = std::move(module_opt.value());

        // 3. Extract netlist
        mvs::Netlist netlist = mvs::NetlistExtractor::extract(module);

        // 4. Convert to JSON
        json netlist_json = json::array();
        for (const auto& comp : netlist) {
            netlist_json.push_back(to_json(comp));
        }

        return json{{"success", true}, {"netlist", netlist_json}}.dump();
    }
    catch (const std::exception& e)
    {
        // Return JSON error with full exception details
        std::string error_msg = std::string(e.what());
        return json{{"error", error_msg}}.dump();
    }
    catch (...)
    {
        // Catch all other exceptions
        return json{{"error", "Unknown C++ exception occurred"}}.dump();
    }
}

// חשיפת הפונקציה ל-JavaScript
EMSCRIPTEN_BINDINGS(mvs_bindings) {
    function("generateNetlistJson", &generate_netlist_json);
}