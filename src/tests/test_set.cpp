#include "../include/Snippets.hpp"

#include <set>

int main() {

	std::set<Snippet> snippets;

	auto copy = Snippet("somefile", 10, 15);

	snippets.emplace(copy);
	snippets.emplace(Snippet("somefile", 15, 20));
	snippets.emplace(copy);

	std::cout << snippets.size() << '\n';

	std::cout << "contains copy? "
			  << (snippets.contains(copy) ? "true" : "false") << '\n';
}
