#ifndef LIBRARYDB_H
#define LIBRARYDB_H

#include <sqlite3.h>
#include <string>
#include <iostream>
using namespace std;

class LibraryDB {
private:
    sqlite3* db;
public:
    LibraryDB(const string& dbName);
    ~LibraryDB();
    bool execute(const string& sql);
    bool exists(const string& sql);
    int getInt(const string& sql);
    void createTables();
};

#endif
