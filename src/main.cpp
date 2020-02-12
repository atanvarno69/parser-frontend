#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "ebnfAstGenerator.h"
#include "ebnfParserFactory.h"
#include "nodeReader.h"
#include "scanner.h"

using namespace enjoy;

int main(int argc, char** argv) {
    // Populate arguments vector
    std::vector<std::string> arguments;
    for (int i = 0; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }
    // Check for filename
    if (arguments.size() < 2) {
        std::cout << "No filename given." << std::endl;
        return EXIT_FAILURE;
    }
    std::string filename = arguments[1];
    // Build input stream
    std::ifstream input {filename};
    if (!input) {
        std::cout << "File " << filename << " can't be read." << std::endl;
        return EXIT_FAILURE;
    }
    // Build file scanner
    scanner fileScanner {&input};
    // Get EBNF parser and dictionary
    parser ebnfParser = *ebnfParserFactory::getParser();
    std::map<unsigned, std::string> dictionary = *ebnfParserFactory::getDictionary();
    // Get parse tree
    node output;
    try {
        output = *ebnfParser.parse(fileScanner);
    } catch (std::runtime_error& e) {
        std::cout << "Fail!" << std::endl;
        return EXIT_FAILURE;
    }
    // Walking
    ebnfAstGenerator astGenerator;
    output = *astGenerator.visit(&output);
    nodeReader reader {dictionary};
    output.accept(reader);
    std::cout << "Winning!" << std::endl;
    return EXIT_SUCCESS;
}