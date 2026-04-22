#include "../include/SimilarityDetect.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <exception>
#include <numeric>
#include <vector>

float SimilarityDetect::cosine_similarity(const std::vector<float>& a,
										  const std::vector<float>& b) {
	assert(a.size() == b.size());

	float dot_product = 0.0, norm_a = 0.0, norm_b = 0.0;
	for (size_t i = 0; i < a.size(); ++i) {
		dot_product += a[i] * b[i];
		norm_a += a[i] * a[i];
		norm_b += b[i] * b[i];
	}
	return dot_product / (std::sqrt(norm_a) * std::sqrt(norm_b));
}

CodeCloneInfo
SimilarityDetect::operator()(const std::vector<Snippet>& snippets) {

	std::vector<std::vector<float>> embeddings;
	embeddings.reserve(snippets.size() * this->dimensionality);

	// add all snippets to index
	for (auto& s : snippets) {
		try {
			auto text_as_lines = s.get_snippet();
			auto text = std::accumulate(text_as_lines.begin(),
										text_as_lines.end(), std::string{});

			auto r =
				ollama::generate_embeddings(this->model_name, text).as_json();
			auto embedding = r["embeddings"][0].get<std::vector<float>>();
			embeddings.push_back(embedding);

		} catch (std::exception e) {
			std::cerr << s << '\n';
			auto lines = s.get_snippet();
			for (const auto& l : lines) {
				std::cerr << l << '\n';
			}
			std::cerr << e.what() << '\n';
		}
	}

	CodeCloneInfo clones(snippets);

	for (size_t i = 0; i < embeddings.size(); i++) {
		std::vector<Snippet> current_clones;
		for (size_t j = 0; j < embeddings.size(); j++) {
			if (i == j)
				continue;
			if (SimilarityDetect::cosine_similarity(
					embeddings[i], embeddings[j]) > this->threshold) {
				current_clones.push_back(snippets[j]);
			}
		}
		if (current_clones.size() > 0) {
			clones.clones[i] = current_clones;
		}
	}
	return clones;
}
