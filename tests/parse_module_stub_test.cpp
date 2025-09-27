#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "mvs/lexer.hpp"
#include "mvs/parser.hpp"

TEST_CASE("Parser parses minimal module stub", "[parser]") {
    using namespace mvs;

    std::string code = "module MyModule(input a, output b); endmodule";

    Lexer lexer(code);
    auto tokens = lexer.Tokenize();

    Parser parser(tokens);

    REQUIRE(parser.parseModuleStub() == true);
}

TEST_CASE("Parser fails on missing endmodule", "[parser]") {
    using namespace mvs;

    std::string code = "module MyModule(input a, output b);";

    Lexer lexer(code);
    auto tokens = lexer.Tokenize();

    Parser parser(tokens);

    REQUIRE(parser.parseModuleStub() == false);
}

TEST_CASE("Parser parses empty port list", "[parser]") {
    using namespace mvs;

    std::string code = "module MyModule(); endmodule";

    Lexer lexer(code);
    auto tokens = lexer.Tokenize();

    Parser parser(tokens);

    REQUIRE(parser.parseModuleStub() == true);
}
