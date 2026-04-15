#include <fstream>
#include <iostream>
#include <vector>

#include "../include/Detector.hpp"
#include "../include/Normalize.hpp"
#include "../include/Parser.hpp"

template <typename T> void print_vec(const std::vector<T> v) {
	for (auto& item : v) {
		std::cout << item << '\n';
	}
}

int main(int argc, char* argv[]) {
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

	print_vec(tokens1);

	// Step 2: parse functions
	auto funcs1 = parse_functions(argv[1], tokens1);
	auto funcs2 = parse_functions(argv[2], tokens2);

	std::cout << funcs1.second.size() << '\n';
	std::cout << funcs1.second[1].start_range << ":"
			  << funcs1.second[1].end_range << '\n';

	std::cout << funcs1.first[0].name << '\n';
	std::cout << funcs1.first[1].name << '\n';

	// Step 3: detect clones
	int count;
	count = detectType1Clones(funcs1.first, funcs2.first);
	std::cout << "Total amount of Type-1 Clones found: " << count << std::endl;
	return 0;
}
