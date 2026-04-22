#include "../include/Snippets.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::vector<Snippet> get_partial_funcs_from_c_file(std::filesystem::path p) {
	auto res = std::vector<Snippet>{};

	int status = std::system(
		(std::string("ctags -xT ") + p.string() + " > out_tags").c_str());
	if (status != 0) {
		return res;
	}

	std::ifstream f("out_tags");
	std::string line;

	while (std::getline(f, line)) {
		// skip any macros
		if (line.contains("#define")) {
			continue;
		}
		std::stringstream ss(line);

		std::string function_name, file_path;
		int line_num;
		ss >> function_name >> line_num >> file_path;
		res.push_back(Snippet(file_path, line_num, -1, function_name));
	}

	std::filesystem::remove("out_tags");

	return res;
}

std::vector<Snippet> get_partial_funcs_from_java_file(std::filesystem::path p) {
	std::vector<Snippet> result;

	std::function<bool(const std::string&)> predicate;

	try {

		// Source - https://stackoverflow.com/a/52996479
		// Posted by LarsH
		// Retrieved 2026-04-15, License - CC BY-SA 4.0

		predicate = [](const std::string& line) {
			std::smatch m;
			std::regex r =
				std::regex("^[ \t]*(([A-Za-z_][A-Za-z_0-9]*[ "
						   "\t]+)+[A-Za-z_][A-Za-z_0-9]*[ \t]*\\([^;]*)$");

			return std::regex_match(line, m, r);
		};

	} catch (const std::regex_error& e) {
		std::cerr << "couldn't make java regex!:" << e.what() << '\n';
		assert(false);
	}

	std::ifstream file(p);

	if (!file.good()) {
		return result;
	}

	std::string line;

	int line_num = 1;

	while (std::getline(file, line)) {

		if (predicate(line)) {

			result.push_back(Snippet(p.string(), line_num, -1));
		}
		line_num++;
	}
	return result;
}

std::vector<Snippet>
resolve_partial_snippets_from_file(const std::vector<Snippet>& partials) {
	std::vector<Snippet> result;

	for (const auto& snippet : partials) {
		std::ifstream file(snippet.filepath);

		if (!file.good()) {
			continue;
		}

		for (int i = 0; i < snippet.start_range - 1; i++) {
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		std::string line;
		Snippet current = snippet;

		int line_num = snippet.start_range;
		int brace_count = 0;

		bool first_brace_encountered = false;

		while (std::getline(file, line)) {
			if (line.contains("{")) {
				brace_count++;
				first_brace_encountered = true;
			}
			if (line.contains("}")) {
				brace_count--;
			}

			if (first_brace_encountered && brace_count == 0) {
				current.end_range = line_num;
				if (current.end_range - current.start_range > 0) {
					// make sure it's not a declaration
					result.push_back(current);
					break;
				}
			}

			line_num++;
		}
	}
	return result;
}

std::vector<Snippet> get_methods_from_file(std::filesystem::path p) {
	std::vector<Snippet> partials;

	if (p.string().ends_with(".java")) {
		partials = get_partial_funcs_from_java_file(p);
	} else if (p.string().ends_with(".c")) {
		partials = get_partial_funcs_from_c_file(p);
	} else {
		return partials; // non-whitelisted file
	}
	return resolve_partial_snippets_from_file(partials);
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
		std::cerr << "couldn't open file " << this->filepath << '\n';
		return lines; // empty
	}

	// skip to start range; from:
	// 		https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
	for (int i = 0; i < start_range - 1; i++) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	bool in_multiline_comment = false;

	for (int i = 0; i < num_lines; i++) {
		std::string _s;
		std::string s;
		std::getline(file, _s);

		// https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
		const auto str_begin = _s.find_first_not_of("\t \n");
		if (str_begin == std::string::npos) {
			continue; // empty line
		}
		const auto str_end = _s.find_last_not_of("\t \n");

		_s = _s.substr(str_begin, (str_end - str_begin) + 1);

		// normalize spaces (eg. "a			b" -> "a b")
		// beautiful oneliner from:
		// https://stackoverflow.com/questions/35301432/remove-extra-white-spaces-in-c
		std::unique_copy(
			_s.begin(), _s.end(), std::back_insert_iterator<std::string>(s),
			[](char a, char b) { return ::isspace(a) && ::isspace(b); });

		auto single_line_comment_pos = s.find("//");
		if (single_line_comment_pos != std::string::npos) {
			// erase comment to end
			s.erase(single_line_comment_pos, std::string::npos);
		}

		auto multi_line_start_pos = s.find("/*");
		auto multi_line_end_pos = s.find("*/");

		if (multi_line_start_pos != std::string::npos &&
			multi_line_end_pos != std::string::npos) {
			s.erase(multi_line_start_pos, multi_line_end_pos);
		} else {
			if (multi_line_start_pos != std::string::npos) {
				s.erase(multi_line_start_pos, std::string::npos);
				in_multiline_comment = true;
			}

			if (in_multiline_comment) {

				if (multi_line_end_pos == std::string::npos) {
					continue;
				}
				s.erase(0, multi_line_end_pos + 2);
				in_multiline_comment = false;
			}
		}

		// removing anything empty
		if (s.empty()) {
			continue;
		}

		lines.push_back(s);
	}
	return lines;
}

std::ostream& operator<<(std::ostream& os, const Snippet& ci) {
	os << ci.filepath << ": [" << ci.start_range << ", " << ci.end_range << "]";
	return os;
}

bool Snippet::operator<(const Snippet& rhs) const {

	Snippet::Hash hasher;

	return hasher(*this) < hasher(rhs);
}

bool Snippet::operator==(const Snippet& other) const {
	if (this->filepath == other.filepath &&
		this->start_range == other.start_range &&
		this->end_range == other.end_range) {
		return true;
	}
	return false;
}

std::size_t Snippet::Hash::operator()(const Snippet& s) const {
	std::stringstream ss;
	ss << s;

	return std::hash<std::string>()(ss.str());
}
