#pragma once
#include <string>
#include <vector>

// Using namespace std for convenience as requested
using namespace std;

// Represents a Book in the library
struct Book {
    int id;
    string title;
    string author;
    int quantity;
    int available_quantity;
};

// Represents a Student user
struct Student {
    int id;
    string rollNo;
    string name;
    string email;
    double totalFine;
};

// Represents an Admin user
struct Admin {
    int id;
    string email;
};

// Represents a record of a book being issued to a student
struct IssueRecord {
    int id;
    string bookTitle;
    string studentName;
    string issueDate;
    string dueDate;
    string returnDate;
};