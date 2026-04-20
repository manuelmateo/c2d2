#pragma once

#include "Snippets.hpp"

#include <unordered_map>
#include <vector>

struct CodeCloneInfo {

	std::vector<Snippet> snippets;
	std::unordered_map<int, std::vector<Snippet>> clones;

	CodeCloneInfo(std::vector<Snippet> snippets) : snippets(snippets) {}

	friend std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci);
};
