#include "../include/CodeCloneInfo.hpp"

#include <set>

std::ostream& operator<<(std::ostream& os, const CodeCloneInfo& ci) {
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

		os << "snippet in file " << current_snippet << " has " << val.size()
		   << " clone(s):\n";

		for (auto& c : val) {
			os << '\t' << c << '\n';
		}
	}

	return os;
}

// g1: 294-347,
//
