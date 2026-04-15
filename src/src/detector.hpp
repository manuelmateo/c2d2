#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "parser.hpp"

size_t hashFunction(const Function& f) {
    std::string combined;
    for (const std::string& token : f.tokens) {
        combined += token + " ";
    }
    return std::hash<std::string>{}(combined);
}

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
    std::unordered_map<size_t, const Function*> hashMap;

    for (const Function& f : a) {
        hashMap[hashFunction(f)] = &f;
    }

    for (const Function& f : b) {
        size_t h = hashFunction(f);
        if (hashMap.count(h)) {
            const Function* match = hashMap[h];
            if (isExactMatch(match->tokens, f.tokens)) {
                std::cout << "Type-1 Clone Found:\n";
                std::cout << "Function File1: " << match->name << "\n";
                std::cout << "Function File2: " << f.name << "\n";
                std::cout << "------------------------\n";
                count++;
            }
        }
    }

    return count;
}