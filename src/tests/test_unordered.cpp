#include "../include/Snippets.hpp"
#include <unordered_set>

int main() {

	std::unordered_set<Snippet, Snippet::Hash> snippets;

	auto copy = Snippet("somefile", 10, 15);

	snippets.emplace(copy);
	snippets.emplace(Snippet("somefile", 15, 20));
	snippets.emplace(copy);

	std::cout << snippets.size() << '\n';

	std::cout << "contains copy? "
			  << (snippets.contains(copy) ? "true" : "false") << '\n';
}
