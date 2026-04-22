#include "../include/Snippets.hpp"

int main() {
	auto snippets = detect_method_snippets("testfiles/sample_repo");
	std::cout << "num snippets:" << snippets.size() << '\n';

	for (const auto& s : snippets) {
		std::cout << s << '\n';
	}
}
