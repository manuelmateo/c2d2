#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "parser.hpp"
 
bool isExactMatch(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    if (a.size() != b.size())
        return false;
 
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}
 
int detectType1Clones(const std::vector<Function>& a, const std::vector<Function>& b) {
    int count = 0;
 
    for (size_t i = 0; i < a.size(); i++) {
        const Function& a1 = a[i];
        for (size_t j = 0; j < b.size(); j++) {
            const Function& b1 = b[j];
            if (isExactMatch(a1.tokens, b1.tokens)) {
                std::cout << "Type-1 Clone Found:\n";
                std::cout << "Function File1: " << a1.name << "\n";
                std::cout << "Function File2: " << b1.name << "\n";
                std::cout << "------------------------\n";
                count++;
            }
        }
    }
 
    return count;
}