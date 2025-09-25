#include "Library.h"
#include <iostream>

Library::Library() : db("library.db") {
    db.createTables();
}

void Library::registerStudent() {
    string rollNumber, name, password;
    cout << "Enter Roll Number: ";
    cin >> rollNumber;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Password: ";
    cin >> password;

    string sql = "INSERT INTO Students VALUES('" + rollNumber + "', '" + name + "', '" + password + "');";
    if (db.execute(sql))
        cout << "Student registered successfully!" << endl;
}

void Library::addBook() {
    string id, title, category;
    cout << "Enter Book ID: ";
    cin >> id;
    cout << "Enter Title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter Category: ";
    getline(cin, category);

    string sql = "INSERT INTO Books VALUES('" + id + "', '" + title + "', '" + category + "', 0);";
    if (db.execute(sql))
        cout << "Book added successfully!" << endl;
}

void Library::displayBooks() const {
    string sql = "SELECT id, title, category, isIssued FROM Books;";
    sqlite3* dbHandle;
    sqlite3_open("library.db", &dbHandle);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dbHandle, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Book book(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3)
            );
            book.display();
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(dbHandle);
}

void Library::searchBook() const {
    string title;
    cout << "Enter Book Title: ";
    cin.ignore();
    getline(cin, title);

    string sql = "SELECT id, title, category, isIssued FROM Books WHERE title='" + title + "';";
    sqlite3* dbHandle;
    sqlite3_open("library.db", &dbHandle);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dbHandle, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            Book book(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3)
            );
            book.display();
        } else {
            cout << "Book not found!" << endl;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(dbHandle);
}

void Library::issueBook() {
    string rollNumber, bookId;
    cout << "Enter Roll Number: ";
    cin >> rollNumber;
    cout << "Enter Book ID: ";
    cin >> bookId;

    string sqlBook = "SELECT isIssued FROM Books WHERE id='" + bookId + "';";
    int issued = db.getInt(sqlBook);

    if (issued == -1) {
        cout << "Book not found!" << endl;
        return;
    }
    if (issued == 1) {
        cout << "Book already issued!" << endl;
        return;
    }

    string sqlInsert = "INSERT INTO IssuedBooks VALUES('" + rollNumber + "', '" + bookId + "');";
    string sqlUpdate = "UPDATE Books SET isIssued=1 WHERE id='" + bookId + "';";

    if (db.execute(sqlInsert) && db.execute(sqlUpdate))
        cout << "Book issued successfully!" << endl;
}

void Library::returnBook() {
    string rollNumber, bookId;
    cout << "Enter Roll Number: ";
    cin >> rollNumber;
    cout << "Enter Book ID: ";
    cin >> bookId;

    string sqlDelete = "DELETE FROM IssuedBooks WHERE rollNumber='" + rollNumber + "' AND bookId='" + bookId + "';";
    string sqlUpdate = "UPDATE Books SET isIssued=0 WHERE id='" + bookId + "';";

    if (db.execute(sqlDelete) && db.execute(sqlUpdate))
        cout << "Book returned successfully!" << endl;
    else
        cout << "Failed to return book!" << endl;
}

void Library::deleteBook() {
    string bookId;
    cout << "Enter Book ID to delete: ";
    cin >> bookId;

    string sql = "DELETE FROM Books WHERE id='" + bookId + "';";
    if (db.execute(sql))
        cout << "Book deleted successfully!" << endl;
    else
        cout << "Book not found!" << endl;
}

void Library::displayIssuedBooks() const {
    string sql = "SELECT Books.id, Books.title, Books.category, Books.isIssued "
                 "FROM Books JOIN IssuedBooks ON Books.id = IssuedBooks.bookId;";

    sqlite3* dbHandle;
    sqlite3_open("library.db", &dbHandle);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dbHandle, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Book book(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3)
            );
            book.display();
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(dbHandle);
}
