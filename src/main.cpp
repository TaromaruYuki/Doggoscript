#include <chrono>
#include <config/DoggoscriptConfig.h>
#include <doggoscript/doggoscript.hpp>
#include <fstream>
#include <iostream>

int run_cli(SymbolTable* symbol_table) {
    std::cout << "Doggoscript CLI v" << DOGGOSCRIPT_VERSION_MAJOR << "."
              << DOGGOSCRIPT_VERSION_MINOR << "." << DOGGOSCRIPT_VERSION_PATCH
              << std::endl
              << std::endl;
    while(true) {
        std::cout << "Doggoscript > ";

        std::string input;
        if(!std::getline(std::cin, input)) { return 1; }

        if(input == ".exit") { return 0; }

        DoggoscriptResult result = run(input, symbol_table);

        if(result.error.has_value()) {
            std::cout << result.error.value().str() << std::endl;
            continue;
        }

        if(result.result.has_value())
            std::cout << result.result.value()->str() << std::endl;
    }
}

int main(int argc, char** argv) {
    auto* symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    if(argc == 1) { return run_cli(symbol_table); }

    std::string   file_name = argv[1];
    std::ifstream file(file_name);

    if(!file) {
        std::cout << "Failed to open file '" << file_name << "'" << std::endl;
        return 1;
    }

    std::string src((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();

#if DOGGOSCRIPT_DEBUG
    auto              start  = std::chrono::high_resolution_clock::now();
    DoggoscriptResult result = run(src, symbol_table);
    auto              end    = std::chrono::high_resolution_clock::now();
#else
    DoggoscriptResult result = run(src, symbol_table);
#endif

    if(result.error.has_value()) {
        std::cout << result.error.value().str() << std::endl;
        return 1;
    }

    //    if (result.result.has_value())
    //        std::cout << result.result.value()->str() << std::endl;

#if DOGGOSCRIPT_DEBUG
    std::chrono::duration<double, std::milli> ms_double = end - start;
    std::cout << "Execution time: " << ms_double.count() << "ms" << std::endl;
#endif

    return 0;
}
