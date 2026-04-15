#include <iostream>
#include <fstream>

#include "normalize.hpp"
#include "parser.hpp"
#include "detector.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: ./detector <file1.cpp> <file2.cpp>\n";
        return 1;
    }

    std::ifstream file1(argv[1]);
    std::ifstream file2(argv[2]);

    if (!file1 || !file2) {
        std::cerr << "Error opening input files\n";
        return 1;
    }

    // Step 1: normalize both files
    std::vector<std::string> tokens1 = normalize(file1);
    std::vector<std::string> tokens2 = normalize(file2);

    // Step 2: parse functions
    std::vector<Function> funcs1 = parseFunctions(tokens1);
    std::vector<Function> funcs2 = parseFunctions(tokens2);

    // Step 3: detect clones
    int count;
    count = detectType1Clones(funcs1, funcs2);
    std::cout << "Total amount of Type-1 Clones found: " << count << std::endl;
    return 0;
}