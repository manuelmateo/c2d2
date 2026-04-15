#pragma once

#include "Parser.hpp"

#include <string>
#include <vector>

size_t hashFunction(const Function& f);

bool isExactMatch(const std::vector<std::string>& a,
				  const std::vector<std::string>& b);

int detectType1Clones(const std::vector<Function>& a);
