#pragma once

#include "../include/Parser.hpp"
#include "../include/Snippets.hpp"
#include <string>
#include <vector>

std::string remove_comments_headers(const std::string& line);

std::vector<Function> normalize(const std::vector<Snippet>& snippets);
