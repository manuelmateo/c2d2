#include "../include/Snippets.hpp"

int main() {
	Snippet s("./testfiles/snippet_parse.txt", 1, 12);

	auto lines = s.get_snippet();

	for (const auto& l : lines) {
		std::cout << l << '\n';
	}
}
