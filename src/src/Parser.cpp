#include "../include/Parser.hpp"

std::vector<Function> parse_functions(const std::vector<std::string>& tokens) {
	std::vector<Function> functions;
	Function current;
	bool inFunction = false;
	bool seenOpenBrace = false;
	int braceCount = 0;

	for (int i = 0; i < tokens.size(); i++) {
		const std::string& t = tokens[i];

		if (!inFunction) {
			if (t.find("(") < t.size() && i > 0) {
				current = Function{};
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
			}
		}
	}

	return functions;
}
