#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

std::string encryptVigenere(const std::string& text, const std::string& key);
std::string decryptVigenere(const std::string& text, const std::string& key);

#endif
