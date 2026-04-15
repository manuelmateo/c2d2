#include "../include/Normalize.hpp"

#include <istream>
#include <sstream>

std::string remove_comments_headers(const std::string& line) {
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

std::vector<std::string> normalize(std::istream& in) {
	std::vector<std::string> words;
	std::string line;
	bool blockComment = false;

	while (std::getline(in, line)) {
		std::string cleaned;
		int i = 0;

		while (i < line.size()) {
			// Checks if the current line starts with /* or ends with */
			if (blockComment == false && i + 1 < line.size() &&
				line[i] == '/' && line[i + 1] == '*') {
				blockComment = true;
				i += 2; // + 2 to skip past the '/' and '*'
			} else if (blockComment && i + 1 < line.size() && line[i] == '*' &&
					   line[i + 1] == '/') {
				blockComment = false;
				i += 2;
			} else {
				if (!blockComment) {
					cleaned += line[i];
				}
				i++;
			}
		}

		cleaned = remove_comments_headers(
			cleaned); // Remove the single line comments/headers

		std::istringstream stream(cleaned);
		std::string word;

		while (stream >> word) {
			words.push_back(word);
		}
	}

	return words;
}
