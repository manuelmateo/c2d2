#include "Snippets.hpp"
#include "ollama.hpp"

#include <vector>

class SimilarityDetect {

	int dimensionality;
	std::string model_name;
	float threshold;

	static float cosine_similarity(const std::vector<float>& a,
								   const std::vector<float>& b);

  public:
	SimilarityDetect(int dimensionality, const std::string& model_name,
					 float threshold = 0.8)
		: dimensionality(dimensionality), model_name(model_name),
		  threshold(threshold) {}

	CodeCloneInfo operator()(const std::vector<Snippet>& snippets);
};
