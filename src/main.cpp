#include <filesystem>
#include <fstream>
#include <optional>
#include <print>
#include <string_view>

#include "include/Args.hpp"
#include "include/Detector.hpp"
#include "include/Normalize.hpp"
#include "include/SimilarityDetect.hpp"
#include "include/Snippets.hpp"

int main(int argc, char** argv) {

	YAGP::Args args(argc, argv, 12);

	auto scan_dir = args.get<std::string_view>("-scan-dir");
	auto out_report_dir = args.get<std::string_view>("-out");
	auto embedding_model = args.get<std::string_view>("-model");
	auto dimensionality = args.get<int>("-dim");
	auto threshold = args.get<float>("-threshold");
	auto format_opt = args.get<std::string_view>("-format");
	auto only_snippets_opt = args.get<bool>("-snippets");

	std::string format = "default";

	if (!scan_dir) {
		std::println("no scan directory!");
		return 1;
	}
	auto snippets = detect_method_snippets(*scan_dir);

	if (only_snippets_opt && *only_snippets_opt) {
		for (const auto& s : snippets) {
			std::cout << s << ' ' << s.function_name << '\n';
		}
		return 0;
	}

	if (format_opt) {
		format = *format_opt;
	}

	if (!embedding_model) {
		std::println("no embedding model specified!");
		return 1;
	}

	if (!dimensionality) {
		std::println("no dimensionality specified!");
		return 1;
	}

	std::optional<SimilarityDetect> sd;

	if (!threshold) {
		sd = SimilarityDetect(*dimensionality, *embedding_model);
	} else {
		sd = SimilarityDetect(*dimensionality, *embedding_model, *threshold);
	}

	auto ci_similarity_detect = (*sd)(snippets);
	ci_similarity_detect.set_format(format);

	auto funcs = normalize(snippets);
	auto ci_hash_based = detectType1Clones(funcs, snippets);
	ci_hash_based.set_format(format);

	std::cout << "similarity based:" << '\n';
	std::cout << ci_similarity_detect << '\n';
	std::cout << "hash based:" << '\n';
	std::cout << ci_hash_based << '\n';

	auto default_out_dir = std::filesystem::path("c2d2-output");

	auto specific_out_dir =
		default_out_dir / std::filesystem::path(*scan_dir).filename();

	auto similarity_output_file_path = specific_out_dir / "sim_results";
	auto hash_output_file_path = specific_out_dir / "hash_results";

	auto res = std::filesystem::create_directories(
		similarity_output_file_path.parent_path());

	if (!res) {
		std::cerr << "couldn't make paths!: "
				  << similarity_output_file_path.parent_path() << '\n';
		return 1;
	}

	std::ofstream similarity_output_file(similarity_output_file_path);

	similarity_output_file << ci_similarity_detect;

	std::ofstream hash_output_file(hash_output_file_path);

	hash_output_file << ci_hash_based;
}
