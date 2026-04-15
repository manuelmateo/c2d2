#pragma once

#include <string>
#include <vector>

std::string remove_comments_headers(const std::string& line);

std::vector<std::string> normalize(std::istream& in);
