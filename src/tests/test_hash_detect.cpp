#include <fstream>
#include <iostream>

#include "../include/Detector.hpp"
#include "../include/Normalize.hpp"
#include "../include/Snippets.hpp"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: ./detector <file.cpp>\n";
		return 1;
	}

	std::ifstream file1(argv[1]);

	if (!file1) {
		std::cerr << "Error opening input file\n";
		return 1;
	}

	// step 0: get snippets from file
	auto snippets = detect_method_snippets(argv[1]);

	// Step 1: normalize the snippets + convert to function objects
	auto tokens1 = normalize(snippets);

	std::cout << "num funcs: " << tokens1.size() << '\n';

	// Step 3: detect clones within the same file
	auto ci = detectType1Clones(tokens1, snippets);

	std::cout << "Total amount of Type-1 Clones found: " << ci.clones.size()
			  << std::endl;
	std::cout << ci << '\n';

	return 0;
}
