#include <iostream>
#include <limits>
#include "LibraryDB.h"
#include "EmailService.h"

// --- Function Prototypes for UI ---
void showAdminMenu(LibraryDB& db, EmailService& emailer);
void showStudentMenu(LibraryDB& db, Student currentStudent);
void clearScreen();
void pauseScreen();

// Registration Handlers
void handleStudentRegistration(LibraryDB& db);
void handleAdminRegistration(LibraryDB& db);

// Admin Menu Handlers
void handleAddBook(LibraryDB& db);
void handleRemoveBook(LibraryDB& db);
void handleUpdateBook(LibraryDB& db);
void handleViewStudents(LibraryDB& db);
void handleResetStudentPassword(LibraryDB& db);
void handleSendReminders(LibraryDB& db, EmailService& emailer);

// Student Menu Handlers
void handleSearchBooks(LibraryDB& db);
void handleIssueBook(LibraryDB& db, int studentId);
void handleReturnBook(LibraryDB& db, int studentId);
void handleViewHistory(LibraryDB& db, int studentId);
void handleViewFines(LibraryDB& db, int studentId);


// --- Main Application ---
int main() {
    LibraryDB db("library.db");
    db.initializeDatabase();
    EmailService emailer;

    int choice;
    while (true) {
        clearScreen();
        cout << R"(
 _     _     _     _     _     _     _     _  
/ \   / \   / \   / \   / \   / \   / \   / \ 
( L ) ( i ) ( b ) ( r ) ( a ) ( r ) ( y ) ( ! )
 \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/ 
)" << endl;
        cout << "========================================" << endl;
        cout << "      LIBRARY MANAGEMENT SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Student Login" << endl;
        cout << "3. Register as New Student" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: {
                clearScreen();
                cout << "--- Admin Login ---" << endl;
                string email, password;
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> password;
                if (auto admin = db.authenticateAdmin(email, password)) {
                    cout << "\nAdmin login successful!" << endl;
                    pauseScreen();
                    showAdminMenu(db, emailer);
                } else {
                    cout << "\nInvalid credentials!" << endl;
                    pauseScreen();
                }
                break;
            }
            case 2: {
                clearScreen();
                cout << "--- Student Login ---" << endl;
                string rollNo, password;
                cout << "Roll Number: ";
                cin >> rollNo;
                cout << "Password: ";
                cin >> password;
                if (auto student = db.authenticateStudent(rollNo, password)) {
                    cout << "\nLogin successful! Welcome, " << student->name << "." << endl;
                    pauseScreen();
                    showStudentMenu(db, *student);
                } else {
                    cout << "\nInvalid credentials!" << endl;
                    pauseScreen();
                }
                break;
            }
            case 3: {
                handleStudentRegistration(db);
                pauseScreen();
                break;
            }
            case 4:
                cout << "Exiting application. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
                break;
        }
    }

    return 0;
}

// --- UI Implementations ---

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showAdminMenu(LibraryDB& db, EmailService& emailer) {
    int choice;
    while (true) {
        clearScreen();
        cout << "--- Admin Dashboard ---" << endl;
        cout << "1. Add New Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Update Book Quantity" << endl;
        cout << "4. View All Students & Fines" << endl;
        cout << "5. Reset Student Password" << endl;
        cout << "6. Send Due Date Reminders" << endl;
        cout << "7. Register New Admin" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: handleAddBook(db); break;
            case 2: handleRemoveBook(db); break;
            case 3: handleUpdateBook(db); break;
            case 4: handleViewStudents(db); break;
            case 5: handleResetStudentPassword(db); break;
            case 6: handleSendReminders(db, emailer); break;
            case 7: handleAdminRegistration(db); break;
            case 8: return;
            default: cout << "Invalid choice." << endl;
        }
        pauseScreen();
    }
}

void showStudentMenu(LibraryDB& db, Student currentStudent) {
    int choice;
    while (true) {
        clearScreen();
        cout << "--- Student Dashboard ---" << endl;
        cout << "Welcome, " << currentStudent.name << " (" << currentStudent.rollNo << ")" << endl;
        cout << "--------------------------" << endl;
        cout << "1. Search for Books" << endl;
        cout << "2. Issue a Book" << endl;
        cout << "3. Return a Book" << endl;
        cout << "4. View My Issued History" << endl;
        cout << "5. View My Fines" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: handleSearchBooks(db); break;
            case 2: handleIssueBook(db, currentStudent.id); break;
            case 3: handleReturnBook(db, currentStudent.id); break;
            case 4: handleViewHistory(db, currentStudent.id); break;
            case 5: handleViewFines(db, currentStudent.id); break;
            case 6: return;
            default: cout << "Invalid choice." << endl;
        }
        pauseScreen();
    }
}

// --- Registration Handler Implementations ---

void handleStudentRegistration(LibraryDB& db) {
    string rollNo, name, email, password;
    clearScreen();
    cout << "--- New Student Registration ---" << endl;
    cout << "Enter Roll Number: ";
    cin >> rollNo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Full Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Password: ";
    cin >> password;

    if (db.addStudent(rollNo, name, email, password)) {
        cout << "\nRegistration successful! You can now log in." << endl;
    } else {
        cout << "\nRegistration failed. Please try again." << endl;
    }
}

void handleAdminRegistration(LibraryDB& db) {
    string email, password;
    clearScreen();
    cout << "--- New Admin Registration ---" << endl;
    cout << "Enter new admin's Email: ";
    cin >> email;
    cout << "Enter a temporary Password: ";
    cin >> password;

    if (db.addAdmin(email, password)) {
        cout << "\nNew admin account created successfully." << endl;
    } else {
        cout << "\nFailed to create new admin account." << endl;
    }
}


// --- Admin Handler Implementations ---

void handleAddBook(LibraryDB& db) {
    Book newBook;
    clearScreen();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Title: ";
    getline(cin, newBook.title);
    cout << "Enter Author: ";
    getline(cin, newBook.author);
    cout << "Enter Quantity: ";
    cin >> newBook.quantity;
    if (db.addBook(newBook)) {
        cout << "Book added successfully!" << endl;
    } else {
        cout << "Failed to add book." << endl;
    }
}

void handleRemoveBook(LibraryDB& db) {
    int id;
    clearScreen();
    cout << "Enter ID of the book to remove: ";
    cin >> id;
    if (db.removeBook(id)) {
        cout << "Book removed successfully!" << endl;
    } else {
        cout << "Failed to remove book. It might be referenced in issue records or does not exist." << endl;
    }
}

void handleUpdateBook(LibraryDB& db) {
    int id, quantity;
    clearScreen();
    cout << "Enter ID of the book to update: ";
    cin >> id;
    cout << "Enter new total quantity: ";
    cin >> quantity;
    if(db.updateBookQuantity(id, quantity)) {
        cout << "Book quantity updated successfully!" << endl;
    } else {
        cout << "Failed to update quantity." << endl;
    }
}

void handleViewStudents(LibraryDB& db) {
    clearScreen();
    auto students = db.getAllStudents();
    cout << "--- All Students ---" << endl;
    cout << "ID\tRoll No\t\tName\t\t\tEmail\t\t\tFine (Rs)" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    for (const auto& s : students) {
        cout << s.id << "\t" << s.rollNo << "\t\t" << s.name << "\t\t" << s.email << "\t\t" << s.totalFine << endl;
    }
}

void handleResetStudentPassword(LibraryDB& db) {
    string rollNo, newPass;
    clearScreen();
    cout << "Enter student's Roll Number: ";
    cin >> rollNo;
    cout << "Enter new password: ";
    cin >> newPass;
    if (db.resetStudentPassword(rollNo, newPass)) {
        cout << "Password reset successfully for " << rollNo << "." << endl;
    } else {
        cout << "Failed to reset password. Student not found." << endl;
    }
}

void handleSendReminders(LibraryDB& db, EmailService& emailer) {
    clearScreen();
    cout << "Checking for books due tomorrow..." << endl;
    auto reminders = db.getBooksDueTomorrow();
    if (reminders.empty()) {
        cout << "No books are due tomorrow." << endl;
    } else {
        for (const auto& pair : reminders) {
            emailer.sendDueDateReminder(pair.first, pair.second, "Tomorrow");
        }
        cout << reminders.size() << " reminder(s) have been processed." << endl;
    }
}

// --- Student Handler Implementations ---

void handleSearchBooks(LibraryDB& db) {
    string query;
    clearScreen();
    cout << "Enter search term (title or author): ";
    cin.ignore();
    getline(cin, query);
    auto books = db.searchBooks(query);
    cout << "\n--- Search Results ---" << endl;
    cout << "ID\tTitle\t\t\tAuthor\t\t\tAvailable" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    if (books.empty()) {
        cout << "No books found." << endl;
    } else {
        for (const auto& b : books) {
            cout << b.id << "\t" << b.title << "\t\t" << b.author << "\t\t" << b.available_quantity << "/" << b.quantity << endl;
        }
    }
}

void handleIssueBook(LibraryDB& db, int studentId) {
    int bookId;
    clearScreen();
    cout << "Enter the ID of the book you want to issue: ";
    cin >> bookId;
    if (db.issueBook(studentId, bookId)) {
        cout << "Book issued successfully! Due in 14 days." << endl;
    } else {
        cout << "Failed to issue book. It may be unavailable or the ID is incorrect." << endl;
    }
}

void handleReturnBook(LibraryDB& db, int studentId) {
    int bookId;
    clearScreen();
    cout << "Enter the ID of the book you want to return: ";
    cin >> bookId;
    if (db.returnBook(studentId, bookId)) {
        cout << "Book returned successfully." << endl;
    } else {
        cout << "Failed to return book. Please check the book ID." << endl;
    }
}

void handleViewHistory(LibraryDB& db, int studentId) {
    clearScreen();
    auto history = db.getStudentIssueHistory(studentId);
    cout << "\n--- Your Issue History ---" << endl;
    cout << "Book Title\t\tIssue Date\tDue Date\tReturn Date" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    if (history.empty()) {
        cout << "No history found." << endl;
    } else {
        for (const auto& rec : history) {
            cout << rec.bookTitle << "\t\t" << rec.issueDate << "\t" << rec.dueDate << "\t" << rec.returnDate << endl;
        }
    }
}

void handleViewFines(LibraryDB& db, int studentId) {
    clearScreen();
    // Re-fetch student data to show the most up-to-date fine amount.
    auto studentOpt = db.getStudentById(studentId);
    if (studentOpt) {
        cout << "\n--- Your Fine Details ---" << endl;
        cout << "Total outstanding fine for " << studentOpt->name << " (" << studentOpt->rollNo << "): Rs. " << studentOpt->totalFine << endl;
        cout << "Please clear your dues at the library counter." << endl;
    } else {
        cout << "Could not retrieve your details." << endl;
    }
}