#pragma once

#include "Snippets.hpp"
#include <string>
#include <vector>

struct Function {
	std::string name;
	std::vector<std::string> tokens;
};

std::pair<std::vector<Function>, std::vector<Snippet>>
parse_functions(const std::string& filename,
				const std::vector<std::string>& tokens);
