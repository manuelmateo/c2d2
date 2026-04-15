#include <print>
#include <string_view>

#include "include/Args.hpp"
#include "include/Snippets.hpp"

int main(int argc, char** argv) {

	YAGP::Args args(argc, argv);

	auto scan_dir = args.get<std::string_view>("-scan-dir");
	auto out_report_dir = args.get<std::string_view>("-out");
	auto embedding_model = args.get<std::string_view>("-model");
	auto dimensionality = args.get<int>("-dim");

	if (!scan_dir) {
		std::print("no scan directory!");
		return 1;
	}

	auto snippets = detect_method_snippets(*scan_dir);
}
