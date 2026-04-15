#include "../include/Snippets.hpp"

#include <iostream>

int main() {
	Snippet s("testfiles/testfile.txt", 5, 10);

	auto text = s.get_snippet();
	for (auto& s : text) {
		std::cout << s << '\n';
	}
}
