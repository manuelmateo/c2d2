#include <fstream>
#include <iostream>
#include <vector>

#include "../include/Detector.hpp"
#include "../include/Normalize.hpp"
#include "../include/Parser.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./detector <file.cpp>\n";
        return 1;
    }

    std::ifstream file1(argv[1]);

    if (!file1) {
        std::cerr << "Error opening input file\n";
        return 1;
    }

    // Step 1: normalize the file
    std::vector<std::string> tokens1 = normalize(file1);

    // Step 2: parse functions
    std::vector<Function> funcs1 = parse_functions(tokens1);

    // Step 3: detect clones within the same file
    int count = detectType1Clones(funcs1);

    std::cout << "Total amount of Type-1 Clones found: " << count << std::endl;

    return 0;
}