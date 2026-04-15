#include "../include/Snippets.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

std::vector<Snippet> detect_method_snippets(std::string_view file_dir) {
	try {
		// Source - https://stackoverflow.com/a/847507
		// Posted by Georgios Gousios
		// Retrieved 2026-04-14, License - CC BY-SA 2.5
		const auto r = std::regex(
			"(public|protected|private|static|\\s) "
			"+[\\w\\<\\>\[\\]]+\\s+(\\w+) *\\([^\\)]*\\) *(\\{?|[^;])");

	} catch (const std::regex_error& e) {
		std::cout << "couldn't make regex!" << e.what() << '\n';
		assert(false);
	}

	for (const auto& ent : std::filesystem::directory_iterator(file_dir)) {
		ent.is_directory();
		std::cout << ent.path() << '\n';
	}

	return std::vector<Snippet>{};
}

std::vector<std::string> Snippet::get_snippet() const {

	int num_lines = this->end_range - this->start_range + 1;

	std::vector<std::string> lines;
	lines.reserve(num_lines);

	std::ifstream file(this->filepath);

	if (!file.good()) {
		return lines; // empty
	}

	// skip to start range; from:
	// 		https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
	for (int i = 0; i < start_range - 1; i++) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	for (int i = 0; i < num_lines; i++) {
		std::string s;
		std::getline(file, s);
		lines.push_back(s);
	}
	return lines;
}

std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci) {
	for (const auto& [key, val] : ci.clones) {
		auto current_snippet = ci.snippets[key];
		os << "snippet in file " << current_snippet.filepath << " ("
		   << current_snippet.start_range << "," << current_snippet.end_range
		   << ") has " << val.size() << " clones:\n";

		for (auto& c : val) {
			os << '\t' << c.filepath << " (" << c.start_range << ','
			   << c.end_range << ")\n";
		}
	}

	return os;
}
