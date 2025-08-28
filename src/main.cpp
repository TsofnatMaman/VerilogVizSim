#include <iostream>
#include <fstream>
#include <sstream>

#include "mvs/Version.h"
#include "mvs/Lexer.h"
#include "mvs/Parser.h"

int main(int argc, char** argv) {
    std::cout << "MiniVerilogSim starting..." << MVS_VERSION << std::endl;
    if(argc < 2) {
        std::cerr << "Usage: mvsim <file.v>\n";
        return 0;
    }

    std::ifstream in(argv[1]);
    if(!in) {
        std::cerr << "Failed to open: " << argv[1] << "\n";
        return 1;
    }

    std::ostringstream ss; ss << in.rdbuf();
    mvs::Lexer lx(ss.str());
    auto toks = lx.Tokenize();

    mvs::Parser p(toks);
    if(p.parseModuleStub()) {
        std::cout << "Parsed (stub) OK\n";
    }

    return 0;
}
