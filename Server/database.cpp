#include "database.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <openssl/sha.h>  // или простой хеш

static sqlite3* db = nullptr;

std::string hashPassword(const std::string& password) {
    // Простой SHA256 через OpenSSL, можно заменить на свой
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool initDatabase() {
    int rc = sqlite3_open("users.db", &db);
    if (rc) return false;
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "username TEXT UNIQUE, "
                      "password TEXT);";
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    return rc == SQLITE_OK;
}

bool registerUser(const std::string& username, const std::string& password) {
    std::string hashed = hashPassword(password);
    std::string sql = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + hashed + "');";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    return rc == SQLITE_OK;
}

int loginUser(const std::string& username, const std::string& password) {
    std::string hashed = hashPassword(password);
    std::string sql = "SELECT id FROM users WHERE username = '" + username + "' AND password = '" + hashed + "';";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    int userId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return userId;
}
