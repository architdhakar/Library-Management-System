#pragma once
#include "Models.h"
#include <sqlite3.h>
#include <vector>
#include <optional>

class LibraryDB {
public:
    // Constructor: Opens the database connection
    LibraryDB(const string& db_path);
    // Destructor: Closes the database connection
    ~LibraryDB();

    // Initialization
    void initializeDatabase();

    // Authentication & Registration
    optional<Admin> authenticateAdmin(const string& email, const string& password);
    optional<Student> authenticateStudent(const string& rollNo, const string& password);
    bool addStudent(const string& rollNo, const string& name, const string& email, const string& password);
    bool addAdmin(const string& email, const string& password);

    // Admin Functions
    bool addBook(const Book& book);
    bool removeBook(int bookId);
    bool updateBookQuantity(int bookId, int newQuantity);
    vector<Student> getAllStudents();
    bool resetStudentPassword(const string& rollNo, const string& newPassword);
    
    // Student Functions
    vector<Book> searchBooks(const string& query);
    bool issueBook(int studentId, int bookId);
    bool returnBook(int studentId, int bookId);
    vector<IssueRecord> getStudentIssueHistory(int studentId);
    optional<Student> getStudentById(int studentId);
    optional<Book> getBookById(int bookId);

    // Fine & Notification System
    vector<pair<Student, Book>> getBooksDueTomorrow();

private:
    sqlite3* db;
    
    // Private helper methods
    string hashPassword(const string& password);
    bool checkPassword(const string& password, const string& hash);
    bool executeQuery(const string& sql);
};