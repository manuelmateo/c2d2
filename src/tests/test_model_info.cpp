#include <iostream>

#include "../include/ollama.hpp"

int main() {
	nlohmann::json model_info = ollama::show_model_info("embeddinggemma");
	std::cout << model_info["shape"] << '\n';
}
