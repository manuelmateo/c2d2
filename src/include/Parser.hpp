#pragma once

#include <string>
#include <vector>

struct Function {
	std::string name;
	std::vector<std::string> tokens;
};

std::vector<Function> parse_functions(const std::vector<std::string>& tokens);
