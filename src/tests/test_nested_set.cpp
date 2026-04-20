#include "../include/Snippets.hpp"

#include <set>

int main() {
	std::set<std::set<Snippet>> snippets;

	std::set<Snippet> group1;
	group1.emplace(Snippet("testfile", 10, 15));
	group1.emplace(Snippet("testfile", 15, 20));
	group1.emplace(Snippet("testfile", 20, 25));

	std::set<Snippet> group2;
	group1.emplace(Snippet("testfile", 20, 25));
	group1.emplace(Snippet("testfile", 30, 35));
	group1.emplace(Snippet("testfile", 40, 50));

	snippets.emplace(group1);
	snippets.emplace(group2);
	snippets.emplace(group1);

	std::cout << snippets.size() << '\n';
}
