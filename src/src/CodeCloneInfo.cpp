#include "../include/CodeCloneInfo.hpp"

#include <filesystem>
#include <ostream>
#include <set>

std::ostream& default_format(std::ostream& os, const CodeCloneInfo& ci) {
	int total_clones = 0;
	std::set<std::set<Snippet>> seen_snippets;

	for (const auto& [key, val] : ci.clones) {

		if (val.size() == 0) {
			continue;
		}

		auto current_snippet = ci.snippets[key];

		std::set<Snippet> current_snippets(val.begin(), val.end());

		current_snippets.insert(current_snippet);

		if (seen_snippets.contains(current_snippets)) {
			continue;
		} else {
			seen_snippets.insert(current_snippets);
		}

		total_clones += current_snippets.size();

		os << "snippet in file " << current_snippet << " has " << val.size()
		   << " clone(s):\n";

		for (auto& c : val) {
			os << '\t' << c << '\n';
		}
	}
	std::cout << "# of clones found: " << total_clones << '\n';

	return os;
}

std::ostream& pair_format(std::ostream& os, const CodeCloneInfo& ci) {
	std::set<std::set<Snippet>> seen_pairs;

	for (const auto& [key, val] : ci.clones) {

		if (val.size() == 0) {
			continue;
		}

		auto current_snippet = ci.snippets[key];

		std::set<Snippet> current_snippets(val.begin(), val.end());

		current_snippets.insert(current_snippet);

		for (const auto& c1 : current_snippets) {
			for (const auto& c2 : current_snippets) {
				if (c1 == c2)
					continue;
				auto pair = std::set{c1, c2};

				if (seen_pairs.contains(pair))
					continue;

				seen_pairs.insert(pair);

				std::filesystem::path p1(c1.filepath);
				std::filesystem::path p2(c2.filepath);

				os << p1.stem().string() << "." << c1.function_name << "()"
				   << "-" << p2.stem().string() << "." << c2.function_name
				   << "()" << '\n';
			}
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci) {
	if (ci.format == "pairs") {
		return pair_format(os, ci);
	}
	return default_format(os, ci);
}
