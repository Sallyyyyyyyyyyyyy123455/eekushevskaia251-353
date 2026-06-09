#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

bool initDatabase();
bool registerUser(const std::string& username, const std::string& password);
int loginUser(const std::string& username, const std::string& password);
std::string hashPassword(const std::string& password);

#endif
