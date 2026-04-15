#include "../include/ollama.hpp"
#include <vector>

int main() {
	ollama::response r =
		ollama::generate_embeddings("embeddinggemma", "some sample text");
	ollama::response r2 =
		ollama::generate_embeddings("embeddinggemma", "some sample text");

	auto embedding1 = r.as_json()["embeddings"][0].get<std::vector<float>>();
	auto embedding2 = r.as_json()["embeddings"][0].get<std::vector<float>>();

	std::cout << embedding1.size() << '\n';
	std::cout << (embedding1 == embedding2 ? "same" : "different") << '\n';
}
