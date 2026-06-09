#include "vigenere.h"

std::string encryptVigenere(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    std::string result;
    int keyLen = key.length();
    for (size_t i = 0; i < text.length(); i++) {
        result.push_back(text[i] + key[i % keyLen]);
    }
    return result;
}

std::string decryptVigenere(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    std::string result;
    int keyLen = key.length();
    for (size_t i = 0; i < text.length(); i++) {
        result.push_back(text[i] - key[i % keyLen]);
    }
    return result;
}
