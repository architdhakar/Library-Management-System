#include "LibraryDB.h"

LibraryDB::LibraryDB(const string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
}

LibraryDB::~LibraryDB() {
    if (db) sqlite3_close(db);
}

bool LibraryDB::execute(const string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool LibraryDB::exists(const string& sql) {
    sqlite3_stmt* stmt;
    bool found = false;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) found = true;
    }
    sqlite3_finalize(stmt);
    return found;
}

int LibraryDB::getInt(const string& sql) {
    sqlite3_stmt* stmt;
    int value = -1;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            value = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return value;
}

void LibraryDB::createTables() {
    string sqlStudents = 
        "CREATE TABLE IF NOT EXISTS Students ("
        "rollNumber TEXT PRIMARY KEY, "
        "name TEXT, "
        "password TEXT);";

    string sqlBooks = 
        "CREATE TABLE IF NOT EXISTS Books ("
        "id TEXT PRIMARY KEY, "
        "title TEXT, "
        "category TEXT, "
        "isIssued INTEGER DEFAULT 0);";

    string sqlIssued = 
        "CREATE TABLE IF NOT EXISTS IssuedBooks ("
        "rollNumber TEXT, "
        "bookId TEXT, "
        "PRIMARY KEY (rollNumber, bookId), "
        "FOREIGN KEY (rollNumber) REFERENCES Students(rollNumber), "
        "FOREIGN KEY (bookId) REFERENCES Books(id));";

    execute(sqlStudents);
    execute(sqlBooks);
    execute(sqlIssued);
}
