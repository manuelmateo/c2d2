#include "../include/Parser.hpp"

#include "../include/Snippets.hpp"

#include <utility>
#include <vector>

std::pair<std::vector<Function>, std::vector<Snippet>>
parse_functions(const std::string& filename,
				const std::vector<std::string>& tokens) {
	std::vector<Function> functions;
	std::vector<Snippet> snippets;
	Snippet current_snippet(filename, -1, -1);
	Function current;
	bool inFunction = false;
	bool seenOpenBrace = false;
	int braceCount = 0;

	std::cout << "token size: " << tokens.size() << '\n';
	for (int i = 0; i < tokens.size(); i++) {
		const std::string& t = tokens[i];

		if (!inFunction) {
			if (t.find("(") < t.size() && i > 0) {
				current = Function{};
				current_snippet = Snippet(filename, i, -1);
				current_snippet.start_range = i;
				current.name =
					t.substr(0, t.find("(")); // extract name before '('
				current.tokens.clear();
				current.tokens.push_back(
					tokens[i - 1]); // include return type; was an issue for a
									// while as it would qualify as a clone with
									// different types
				inFunction = true;
				seenOpenBrace = false;
				braceCount = 0;
			}
		}

		if (inFunction) {
			current.tokens.push_back(t);
			if (t == "{") {
				braceCount++;
				seenOpenBrace = true;
			} else if (t == "}") {
				braceCount--;
			}

			if (seenOpenBrace && braceCount == 0 && !current.tokens.empty()) {
				functions.push_back(current);
				inFunction = false;
				seenOpenBrace = false;
				current_snippet.end_range = i;
				snippets.push_back(current_snippet);
			}
		}
	}

	return std::make_pair(functions, snippets);
}
