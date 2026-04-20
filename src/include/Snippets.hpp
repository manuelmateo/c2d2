#pragma once

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

/*
 * information of a single code clone
 * a snippet is any block of code located in file `filepath` at the lines
 * [start_range, end_range]
 *
 * filepath: the path to the file the code clone is contained in
 * start_range: the start line number within the file the code clone is
 * contained in end_range: the end line within the file the code clone is
 */
struct Snippet {
	std::string filepath;
	int start_range;
	int end_range;

	Snippet(const std::string& filepath, int start_range, int end_range)
		: filepath(filepath), start_range(start_range), end_range(end_range) {}

	/*
	 * gets the actual snippet text
	 * returns: the snippet (specified by filepath, start_range, end_range)
	 */
	std::vector<std::string> get_snippet() const;

	friend std::ostream& operator<<(std::ostream& os, const Snippet& ci);

	bool operator==(const Snippet& other) const;

	bool operator<(const Snippet& rhs) const;

	struct Hash {

		std::size_t operator()(const Snippet& s) const;
	};
};

/*
 * detects methods from the files
 */
std::vector<Snippet> detect_method_snippets(std::string_view file_dir);
