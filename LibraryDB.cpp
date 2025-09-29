#include "LibraryDB.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

// --- Helper Functions for Date and Hashing ---

// Simple XOR "hashing" for demonstration.
// !! WARNING: NOT FOR PRODUCTION USE. Use a real library like bcrypt or Argon2.
string LibraryDB::hashPassword(const string& password) {
    string hash = password;
    char key = 'K'; // A simple key
    for (size_t i = 0; i < hash.length(); ++i) {
        hash[i] = password[i] ^ key;
    }
    return hash;
}

bool LibraryDB::checkPassword(const string& password, const string& storedHash) {
    return hashPassword(password) == storedHash;
}

// Function to get current date as YYYY-MM-DD string
string getCurrentDate() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    stringstream ss;
    tm local_tm;
    #ifdef _WIN32
        localtime_s(&local_tm, &now_time);
    #else
        localtime_r(&now_time, &local_tm);
    #endif
    ss << put_time(&local_tm, "%Y-%m-%d");
    return ss.str();
}

// Function to add days to a date string
string addDaysToDate(const string& dateStr, int days) {
    tm t = {};
    stringstream ss(dateStr);
    ss >> get_time(&t, "%Y-%m-%d");
    t.tm_mday += days;
    mktime(&t);
    stringstream result_ss;
    result_ss << put_time(&t, "%Y-%m-%d");
    return result_ss.str();
}

// Function to calculate difference in days between two dates
long long daysBetween(const string& date1, const string& date2) {
    tm t1 = {}, t2 = {};
    stringstream ss1(date1), ss2(date2);
    ss1 >> get_time(&t1, "%Y-%m-%d");
    ss2 >> get_time(&t2, "%Y-%m-%d");
    
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);

    const double seconds_in_day = 60 * 60 * 24;
    return (long long)(difftime(time1, time2) / seconds_in_day);
}

// --- LibraryDB Class Implementation ---

LibraryDB::LibraryDB(const string& db_path) : db(nullptr) {
    if (sqlite3_open(db_path.c_str(), &db)) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
}

LibraryDB::~LibraryDB() {
    if (db) {
        sqlite3_close(db);
    }
}

bool LibraryDB::executeQuery(const string& sql) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void LibraryDB::initializeDatabase() {
    // This function remains the same, creating tables and seeding default users if they don't exist.
    const string createAdminsTable = "CREATE TABLE IF NOT EXISTS Admins (id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT NOT NULL UNIQUE, password_hash TEXT NOT NULL);";
    const string createStudentsTable = "CREATE TABLE IF NOT EXISTS Students (id INTEGER PRIMARY KEY AUTOINCREMENT, roll_no TEXT NOT NULL UNIQUE, name TEXT NOT NULL, email TEXT NOT NULL, password_hash TEXT NOT NULL, total_fine REAL DEFAULT 0.0);";
    const string createBooksTable = "CREATE TABLE IF NOT EXISTS Books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, author TEXT NOT NULL, quantity INTEGER NOT NULL, available_quantity INTEGER NOT NULL);";
    const string createIssueRecordsTable = "CREATE TABLE IF NOT EXISTS IssueRecords (id INTEGER PRIMARY KEY AUTOINCREMENT, book_id INTEGER, student_id INTEGER, issue_date TEXT NOT NULL, due_date TEXT NOT NULL, return_date TEXT, FOREIGN KEY(book_id) REFERENCES Books(id), FOREIGN KEY(student_id) REFERENCES Students(id));";
    
    executeQuery(createAdminsTable);
    executeQuery(createStudentsTable);
    executeQuery(createBooksTable);
    executeQuery(createIssueRecordsTable);
    
    // Seed database if empty
    const string insertAdmin = "INSERT INTO Admins (email, password_hash) SELECT 'admin@lib.com', '" + hashPassword("admin123") + "' WHERE NOT EXISTS (SELECT 1 FROM Admins WHERE email = 'admin@lib.com');";
    const string insertStudent = "INSERT INTO Students (roll_no, name, email, password_hash) SELECT 'S001', 'Test Student', 'student@test.com', '" + hashPassword("student123") + "' WHERE NOT EXISTS (SELECT 1 FROM Students WHERE roll_no = 'S001');";
    const string insertBook = "INSERT INTO Books (title, author, quantity, available_quantity) SELECT 'The C++ Programming Language', 'Bjarne Stroustrup', 5, 5 WHERE NOT EXISTS (SELECT 1 FROM Books WHERE title = 'The C++ Programming Language');";
    
    executeQuery(insertAdmin);
    executeQuery(insertStudent);
    executeQuery(insertBook);
}

optional<Admin> LibraryDB::authenticateAdmin(const string& email, const string& password) {
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, password_hash FROM Admins WHERE email = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            string storedHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (checkPassword(password, storedHash)) {
                Admin admin;
                admin.id = sqlite3_column_int(stmt, 0);
                admin.email = email;
                sqlite3_finalize(stmt);
                return admin;
            }
        }
    }
    sqlite3_finalize(stmt);
    return nullopt;
}

optional<Student> LibraryDB::authenticateStudent(const string& rollNo, const string& password) {
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, name, email, total_fine, password_hash FROM Students WHERE roll_no = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, rollNo.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            string storedHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            if (checkPassword(password, storedHash)) {
                Student student;
                student.id = sqlite3_column_int(stmt, 0);
                student.rollNo = rollNo;
                student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                student.totalFine = sqlite3_column_double(stmt, 3);
                sqlite3_finalize(stmt);
                return student;
            }
        }
    }
    sqlite3_finalize(stmt);
    return nullopt;
}

bool LibraryDB::addStudent(const string& rollNo, const string& name, const string& email, const string& password) {
    sqlite3_stmt* stmt;
    const string sql = "INSERT INTO Students (roll_no, name, email, password_hash) VALUES (?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        string hashedPassword = hashPassword(password);
        
        sqlite3_bind_text(stmt, 1, rollNo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, hashedPassword.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            if (sqlite3_errcode(db) == SQLITE_CONSTRAINT_UNIQUE) {
                cerr << "Error: A student with this Roll Number already exists." << endl;
            } else {
                cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
            }
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }
    return false;
}

bool LibraryDB::addAdmin(const string& email, const string& password) {
    sqlite3_stmt* stmt;
    const string sql = "INSERT INTO Admins (email, password_hash) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        string hashedPassword = hashPassword(password);

        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            if (sqlite3_errcode(db) == SQLITE_CONSTRAINT_UNIQUE) {
                cerr << "Error: An admin with this email already exists." << endl;
            } else {
                cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
            }
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }
    return false;
}

bool LibraryDB::addBook(const Book& book) {
    sqlite3_stmt* stmt;
    const string sql = "INSERT INTO Books (title, author, quantity, available_quantity) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, book.title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, book.author.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, book.quantity);
        sqlite3_bind_int(stmt, 4, book.quantity);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }
    return false;
}

bool LibraryDB::removeBook(int bookId) {
    const string sql = "DELETE FROM Books WHERE id = " + to_string(bookId) + ";";
    return executeQuery(sql);
}

bool LibraryDB::updateBookQuantity(int bookId, int newQuantity) {
    optional<Book> currentBook = getBookById(bookId);
    if (!currentBook) return false;

    int issuedCount = currentBook->quantity - currentBook->available_quantity;
    int newAvailable = newQuantity - issuedCount;

    if (newAvailable < 0) {
        cerr << "Error: New quantity cannot be less than the number of books currently issued." << endl;
        return false;
    }

    sqlite3_stmt* stmt;
    const string sql = "UPDATE Books SET quantity = ?, available_quantity = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, newQuantity);
        sqlite3_bind_int(stmt, 2, newAvailable);
        sqlite3_bind_int(stmt, 3, bookId);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }
    return false;
}

vector<Student> LibraryDB::getAllStudents() {
    vector<Student> students;
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, roll_no, name, email, total_fine FROM Students;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student s;
            s.id = sqlite3_column_int(stmt, 0);
            s.rollNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            s.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            s.totalFine = sqlite3_column_double(stmt, 4);
            students.push_back(s);
        }
    }
    sqlite3_finalize(stmt);
    return students;
}

bool LibraryDB::resetStudentPassword(const string& rollNo, const string& newPassword) {
    string newHash = hashPassword(newPassword);
    sqlite3_stmt* stmt;
    const string sql = "UPDATE Students SET password_hash = ? WHERE roll_no = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, newHash.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, rollNo.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return sqlite3_changes(db) > 0;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}

vector<Book> LibraryDB::searchBooks(const string& query) {
    vector<Book> books;
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, title, author, quantity, available_quantity FROM Books WHERE title LIKE ? OR author LIKE ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        string searchQuery = "%" + query + "%";
        sqlite3_bind_text(stmt, 1, searchQuery.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, searchQuery.c_str(), -1, SQLITE_TRANSIENT);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Book b;
            b.id = sqlite3_column_int(stmt, 0);
            b.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            b.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            b.quantity = sqlite3_column_int(stmt, 3);
            b.available_quantity = sqlite3_column_int(stmt, 4);
            books.push_back(b);
        }
    }
    sqlite3_finalize(stmt);
    return books;
}

bool LibraryDB::issueBook(int studentId, int bookId) {
    optional<Book> book = getBookById(bookId);
    if (!book || book->available_quantity <= 0) {
        cerr << "Book not available for issue." << endl;
        return false;
    }
    executeQuery("BEGIN TRANSACTION;");
    string updateBookSql = "UPDATE Books SET available_quantity = available_quantity - 1 WHERE id = " + to_string(bookId) + ";";
    if (!executeQuery(updateBookSql)) {
        executeQuery("ROLLBACK;");
        return false;
    }
    sqlite3_stmt* stmt;
    string issueSql = "INSERT INTO IssueRecords (book_id, student_id, issue_date, due_date) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, issueSql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        string issueDate = getCurrentDate();
        string dueDate = addDaysToDate(issueDate, 1);
        sqlite3_bind_int(stmt, 1, bookId);
        sqlite3_bind_int(stmt, 2, studentId);
        sqlite3_bind_text(stmt, 3, issueDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, dueDate.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Failed to create issue record: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            executeQuery("ROLLBACK;");
            return false;
        }
        sqlite3_finalize(stmt);
    } else {
        executeQuery("ROLLBACK;");
        return false;
    }
    return executeQuery("COMMIT;");
}

bool LibraryDB::returnBook(int studentId, int bookId) {
    sqlite3_stmt* stmt;
    const string findSql = "SELECT id, due_date FROM IssueRecords WHERE student_id = ? AND book_id = ? AND return_date IS NULL ORDER BY issue_date DESC LIMIT 1;";
    int recordId = -1;
    string dueDate;
    if (sqlite3_prepare_v2(db, findSql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, studentId);
        sqlite3_bind_int(stmt, 2, bookId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            recordId = sqlite3_column_int(stmt, 0);
            dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        } else {
            cerr << "No active issue record found for this book by the student." << endl;
            sqlite3_finalize(stmt);
            return false;
        }
    }
    sqlite3_finalize(stmt);
    executeQuery("BEGIN TRANSACTION;");
    string returnDate = getCurrentDate();
    string updateRecordSql = "UPDATE IssueRecords SET return_date = '" + returnDate + "' WHERE id = " + to_string(recordId) + ";";
    if (!executeQuery(updateRecordSql)) {
        executeQuery("ROLLBACK;");
        return false;
    }
    string updateBookSql = "UPDATE Books SET available_quantity = available_quantity + 1 WHERE id = " + to_string(bookId) + ";";
    if (!executeQuery(updateBookSql)) {
        executeQuery("ROLLBACK;");
        return false;
    }
    long long daysLate = daysBetween(returnDate, dueDate);
    if (daysLate > 0) {
        double fineAmount = daysLate * 2.0;
        cout << "Book returned " << daysLate << " day(s) late. Fine of Rs. " << fineAmount << " applied." << endl;
        string updateFineSql = "UPDATE Students SET total_fine = total_fine + " + to_string(fineAmount) + " WHERE id = " + to_string(studentId) + ";";
        if (!executeQuery(updateFineSql)) {
            executeQuery("ROLLBACK;");
            return false;
        }
    }
    return executeQuery("COMMIT;");
}

vector<IssueRecord> LibraryDB::getStudentIssueHistory(int studentId) {
    vector<IssueRecord> history;
    sqlite3_stmt* stmt;
    const string sql = "SELECT IR.id, B.title, S.name, IR.issue_date, IR.due_date, IR.return_date FROM IssueRecords AS IR JOIN Books AS B ON IR.book_id = B.id JOIN Students AS S ON IR.student_id = S.id WHERE IR.student_id = ? ORDER BY IR.issue_date DESC;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, studentId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            IssueRecord rec;
            rec.id = sqlite3_column_int(stmt, 0);
            rec.bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            rec.studentName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            rec.issueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            rec.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* returnDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            rec.returnDate = returnDate ? returnDate : "Not Returned";
            history.push_back(rec);
        }
    }
    sqlite3_finalize(stmt);
    return history;
}

optional<Student> LibraryDB::getStudentById(int studentId) {
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, roll_no, name, email, total_fine FROM Students WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, studentId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            Student s;
            s.id = sqlite3_column_int(stmt, 0);
            s.rollNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            s.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            s.totalFine = sqlite3_column_double(stmt, 4);
            sqlite3_finalize(stmt);
            return s;
        }
    }
    sqlite3_finalize(stmt);
    return nullopt;
}


optional<Book> LibraryDB::getBookById(int bookId) {
    sqlite3_stmt* stmt;
    const string sql = "SELECT id, title, author, quantity, available_quantity FROM Books WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) { // FIXED typo from c_s() to c_str()
        sqlite3_bind_int(stmt, 1, bookId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            Book b;
            b.id = sqlite3_column_int(stmt, 0);
            b.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            b.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            b.quantity = sqlite3_column_int(stmt, 3);
            b.available_quantity = sqlite3_column_int(stmt, 4);
            sqlite3_finalize(stmt);
            return b;
        }
    }
    sqlite3_finalize(stmt);
    return nullopt;
}

vector<pair<Student, Book>> LibraryDB::getBooksDueTomorrow() {
    vector<pair<Student, Book>> results;
    string tomorrow = addDaysToDate(getCurrentDate(), 1);

    sqlite3_stmt* stmt;
    const string sql = "SELECT S.id, S.roll_no, S.name, S.email, B.id, B.title, B.author FROM IssueRecords AS IR JOIN Students AS S ON IR.student_id = S.id JOIN Books AS B ON IR.book_id = B.id WHERE IR.due_date = ? AND IR.return_date IS NULL;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, tomorrow.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student s;
            s.id = sqlite3_column_int(stmt, 0);
            s.rollNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            s.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            Book b;
            b.id = sqlite3_column_int(stmt, 4);
            b.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            b.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            results.emplace_back(s, b);
        }
    }
    sqlite3_finalize(stmt);
    return results;
}