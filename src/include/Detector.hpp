#pragma once

#include "CodeCloneInfo.hpp"
#include "Parser.hpp"
#include "Snippets.hpp"

#include <string>
#include <vector>

size_t hashFunction(const Function& f);

bool isExactMatch(const std::vector<std::string>& a,
				  const std::vector<std::string>& b);

CodeCloneInfo detectType1Clones(const std::vector<Function>& a,
								const std::vector<Snippet>& snippets);
