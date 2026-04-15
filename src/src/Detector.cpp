#include "../include/Detector.hpp"

#include <iostream>

size_t hashFunction(const Function& f) {
    std::string combined;
    for (const std::string& token : f.tokens) {
        combined += token + " ";
    }
    return std::hash<std::string>{}(combined);
}

bool isExactMatch(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    // If they are different sizes just return false to save time
    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

int detectType1Clones(const std::vector<Function>& funcs) {
    int count = 0;

    // Pre-compute hashes for all functions
    std::vector<size_t> hashes;
    for (const Function& f : funcs) {
        hashes.push_back(hashFunction(f));
    }

    // Compare each pair, using hashes to skip non-matching pairs early
    for (size_t i = 0; i < funcs.size(); i++) {
        for (size_t j = i + 1; j < funcs.size(); j++) {
            // Skip full comparison if hashes don't match
            if (hashes[i] != hashes[j])
                continue;

            // Verify with full token comparison to rule out hash collisions
            if (isExactMatch(funcs[i].tokens, funcs[j].tokens)) {
                std::cout << "Type-1 Clone Found:\n";
                std::cout << "Function 1: " << funcs[i].name << "\n";
                std::cout << "Function 2: " << funcs[j].name << "\n";
                std::cout << "------------------------\n";
                count++;
            }
        }
    }

    return count;
}