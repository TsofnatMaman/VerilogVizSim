#include "catch.hpp"
#include "mvs/lexer.hpp"
#include "mvs/parser.hpp"
#include "mvs/simulator.hpp"
#include <string>
#include <sstream>

using namespace mvs;

// 🔑 התיקון הקריטי מתמקד בהצגת הודעת השגיאה של ה-Parser 
TEST_CASE("Full Pipeline Integration Test: Masking and Stabilization", "[integration][simulator]")
{
    const std::string VERILOG_CODE = R"(
module full_pipeline_test(input [31:0] IN, output [7:0] Y);
    wire [31:0] W;
    assign W = IN + 256; 
    assign Y = W; 
endmodule
)";

    Lexer lexer(VERILOG_CODE);
    Parser parser(lexer.Tokenize()); 
    
    // 1. קבלת התוצאה כאופציונלית
    std::optional<Module> optional_module = parser.parseModule();

    // 2. בדוק שה-Parser הצליח. אם לא, הדפס את הודעת השגיאה של ה-Parser!
    if (!optional_module.has_value()) {
        FAIL("Parser failed to parse module: " << parser.getErrorMessage());
    }
    
    // אם הגענו לכאן, ה-Module קיים וניתן להשתמש בו בבטחה
    Module module = optional_module.value();

    // 3. Setup and Run Simulator (המשך הטסט)
    CircuitSimulator sim(std::move(module));
    sim.symbols_.set_value("IN", 257); 
    sim.simulate();

    // 4. בדיקת התוצאה 
    REQUIRE(sim.get_symbols().get_value("Y") == 1);
}