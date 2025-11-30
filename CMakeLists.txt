cmake_minimum_required(VERSION 3.16)
project(MyVerilogSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing() 

# --- Core library ---
add_library(core
    src/parser.cpp
    src/lexer.cpp
    src/expression_evaluator.cpp
    src/circuit_simulator.cpp
    
    # 💡 הוספת קבצי הנטליסט החדשים
    src/netlist_extractor.cpp
    src/netlist_to_dot.cpp
)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_include_directories(core PUBLIC /path/to/nlohmann/json) # 💡 אם ספריית JSON אינה ב-'include'

# --- Wasm Module ---
if(EMSCRIPTEN)
    # Build Wasm module for Emscripten
    add_executable(VerilogSimWasm 
        src/wasm_bindings.cpp
    )
    
    target_link_libraries(VerilogSimWasm PRIVATE core embind)
    
    set_target_properties(VerilogSimWasm PROPERTIES SUFFIX ".js")
    
    # Emscripten linker options (quoted properly)
    target_link_options(VerilogSimWasm PUBLIC
        -sEXPORT_NAME=VerilogSimModule
        -sEMULATE_FUNCTION_POINTER_CASTS
        -sEXPORT_ES6=1
        -sNO_DISABLE_EXCEPTION_CATCHING=1
        -sWASM=1
        -sENVIRONMENT=web
        -sALLOW_MEMORY_GROWTH=1
    )
endif()

# --- Main executable (Legacy CLI) ---
add_executable(MyVerilogSimMain src/main.cpp)
target_link_libraries(MyVerilogSimMain PRIVATE core)
target_include_directories(MyVerilogSimMain PRIVATE ${CMAKE_SOURCE_DIR}/include)

# --- Tests ---
add_subdirectory(tests)