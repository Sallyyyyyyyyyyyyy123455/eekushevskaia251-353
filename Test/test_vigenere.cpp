#include <iostream>
#include <string>
#include <cassert>

std::string encrypt(const std::string& text, const std::string& key) {
    std::string res;
    for (size_t i = 0; i < text.size(); i++)
        res += text[i] + key[i % key.size()];
    return res;
}

std::string decrypt(const std::string& text, const std::string& key) {
    std::string res;
    for (size_t i = 0; i < text.size(); i++)
        res += text[i] - key[i % key.size()];
    return res;
}

int main() {
    std::string plain = "HELLO";
    std::string key = "KEY";
    std::string enc = encrypt(plain, key);
    std::string dec = decrypt(enc, key);
    assert(dec == plain);
    std::cout << "Test passed!\n";
    return 0;
}
