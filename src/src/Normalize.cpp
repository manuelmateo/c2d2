#include "../include/Normalize.hpp"
#include "../include/Parser.hpp"
#include "../include/Snippets.hpp"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

std::string removeCommentsHeaders(const std::string& line) {
	std::string result;
	int i = 0;

	while (i < line.size()) {
		// For single-line comments
		if (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/')
			break;
		else if (i + 1 < line.size() && line[i] == '#')
			break;
		result += line[i];
		i++;
	}

	return result;
}

std::vector<Function> normalize(const std::vector<Snippet>& snippets) {
	std::vector<Function> funcs;
	bool blockComment = false;

	for (const auto& s : snippets) {
		std::vector<std::string> words;
		for (auto line : s.get_snippet()) {
			std::string cleaned;
			int i = 0;

			while (i < line.size()) {
				// Checks if the current line starts with /* or ends with */
				if (blockComment == false && i + 1 < line.size() &&
					line[i] == '/' && line[i + 1] == '*') {
					blockComment = true;
					i += 2; // + 2 to skip past the '/' and '*'
				} else if (blockComment && i + 1 < line.size() &&
						   line[i] == '*' && line[i + 1] == '/') {
					blockComment = false;
					i += 2;
				} else {
					if (!blockComment) {
						cleaned += line[i];
					}
					i++;
				}
			}

			// Remove the single line comments
			cleaned = removeCommentsHeaders(cleaned);

			std::istringstream stream(cleaned);
			std::string word;

			// Returns a vector with removed whitespace
			while (stream >> word) {
				words.push_back(word);
			}
		}
		funcs.push_back({.name = s.filepath + "[" +
								 std::to_string(s.start_range) + "," +
								 std::to_string(s.end_range) + "]",
						 .tokens = words});
	}

	return funcs;
}
