#include <print>
#include <string_view>

#include "include/Args.hpp"
#include "include/Detector.hpp"
#include "include/Normalize.hpp"
#include "include/SimilarityDetect.hpp"
#include "include/Snippets.hpp"

int main(int argc, char** argv) {

	YAGP::Args args(argc, argv);

	auto scan_dir = args.get<std::string_view>("-scan-dir");
	auto out_report_dir = args.get<std::string_view>("-out");
	auto embedding_model = args.get<std::string_view>("-model");
	auto dimensionality = args.get<int>("-dim");

	if (!scan_dir) {
		std::println("no scan directory!");
		return 1;
	}

	if (!embedding_model) {
		std::println("no embedding model specified!");
		return 1;
	}

	if (!dimensionality) {
		std::println("no dimensionality specified!");
		return 1;
	}

	auto snippets = detect_method_snippets(*scan_dir);

	SimilarityDetect sd(*dimensionality, *embedding_model);
	auto ci_similarity_detect = sd(snippets);

	auto funcs = normalize(snippets);
	auto ci_hash_based = detectType1Clones(funcs, snippets);

	std::cout << "similarity based:" << '\n';
	std::cout << ci_similarity_detect << '\n';

	std::cout << "hash based:" << '\n';
	std::cout << ci_hash_based << '\n';
}
