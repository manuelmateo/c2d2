#include "../include/Snippets.hpp"
#include <filesystem>
#include <vector>

std::vector<Snippet> get_methods_from_file(std::filesystem::path p);

int main() {
	auto result = get_methods_from_file("testfiles/sample_repo/testfile.c");

	for (auto& s : result) {
		std::cout << s << '\n';
	}
}
