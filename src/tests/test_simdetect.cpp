#include "../include/SimilarityDetect.hpp"
#include "../include/Snippets.hpp"
#include <vector>

int main() {
	SimilarityDetect sd(768, "embeddinggemma");

	std::vector<Snippet> snippets = {{"testfiles/type1_1.txt", 1, 3},
									 {"testfiles/type1_2.txt", 1, 3}};

	auto clones = sd(snippets);

	std::cout << clones;
}
