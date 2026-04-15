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

std::vector<Snippet> get_methods_from_file(std::filesystem::path p) {
	std::regex r;
	try {
		// Source - https://stackoverflow.com/a/52996479
		// Posted by LarsH
		// Retrieved 2026-04-15, License - CC BY-SA 4.0

		r = std::regex("^[ \t]*(([A-Za-z_][A-Za-z_0-9]*[ "
					   "\t]+)+[A-Za-z_][A-Za-z_0-9]*[ \t]*\\([^;]*)$");

	} catch (const std::regex_error& e) {
		std::cout << "couldn't make regex!" << e.what() << '\n';
		assert(false);
	}

	std::ifstream file(p);

	std::vector<Snippet> result;

	if (!file.good()) {
		return result;
	}

	std::string line;
	Snippet current(p.string(), -1, -1);

	int line_num = 1;
	int brace_count = 0;

	bool in_function = false;
	bool first_brace_encountered = false;
	while (std::getline(file, line)) {
		std::smatch m;
		if (std::regex_match(line, m, r)) {
			// std::cout << "here: " << line_num << '\n';
			current.start_range = line_num;
			in_function = true;
		}
		if (in_function) {
			if (line.contains("{")) {
				brace_count++;
				first_brace_encountered = true;
			}
			if (line.contains("}")) {
				brace_count--;
			}

			if (first_brace_encountered && brace_count == 0) {
				current.end_range = line_num;
				result.push_back(current);
				current = Snippet(p.string(), -1, -1);
				first_brace_encountered = false;
				in_function = false;
			}
		}
		line_num++;
	}
	return result;
}

std::vector<Snippet> detect_method_snippets(std::string_view file_dir) {

	std::vector<Snippet> results;

	for (const auto& ent : std::filesystem::directory_iterator(file_dir)) {

		std::vector<Snippet> r2;
		if (ent.is_directory()) {
			r2 = detect_method_snippets(ent.path().string());
		} else {
			r2 = get_methods_from_file(ent.path());
		}

		results.insert(results.end(), r2.begin(), r2.end());
	}
	return results;
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

std::ostream& operator<<(std::ostream& os, const Snippet& ci) {
	os << ci.filepath << ": [" << ci.start_range << ", " << ci.end_range << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci) {
	for (const auto& [key, val] : ci.clones) {

		if (val.size() == 0) {
			continue;
		}

		auto current_snippet = ci.snippets[key];

		os << "snippet in file " << current_snippet.filepath << " ("
		   << current_snippet.start_range << "," << current_snippet.end_range
		   << ") has " << val.size() << " clone(s):\n";

		for (auto& c : val) {
			os << '\t' << c.filepath << " (" << c.start_range << ','
			   << c.end_range << ")\n";
		}
	}

	return os;
}
