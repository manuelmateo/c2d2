#pragma once

#include "Snippets.hpp"

#include <unordered_map>
#include <vector>

struct CodeCloneInfo {

	std::vector<Snippet> snippets;
	std::unordered_map<int, std::vector<Snippet>> clones;
	std::string format;

	CodeCloneInfo(std::vector<Snippet> snippets, std::string format = "default")
		: snippets(snippets), format(format) {}

	void set_format(const std::string& f) { this->format = f; }

	friend std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci);
};
