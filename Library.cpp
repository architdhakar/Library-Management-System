#include "Library.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

Library::Library() {
    loadBooks();
    loadStudents();
}

void Library::loadBooks() {
    ifstream file("books.txt");
    string line, id, title, category;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, category, ',');
        books.push_back(Book(id, title, category));
    }
    file.close();
}

void Library::loadStudents() {
    ifstream file("students.txt");
    string line, rollNumber, name, password;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, rollNumber, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        students.push_back(Student(rollNumber, name, password));
    }
    file.close();
}

void Library::saveStudent(const Student& student) {
    ofstream file("students.txt", ios::app);
    file << student.rollNumber << "," << student.name << "," << student.password << endl;
    file.close();
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
    Student student(rollNumber, name, password);
    students.push_back(student);
    saveStudent(student);
    cout << "Student registered successfully!" << endl;
}

void Library::displayBooks() const {
    for (const auto& book : books) {
        book.display();
    }
}

void Library::issueBook() {
    string rollNumber, bookId;
    cout << "Enter Roll Number: ";
    cin >> rollNumber;
    cout << "Enter Book ID: ";
    cin >> bookId;

    auto student = find_if(students.begin(), students.end(), [&rollNumber](const Student& s) { return s.rollNumber == rollNumber; });
    if (student != students.end()) {
        auto book = find_if(books.begin(), books.end(), [&bookId](const Book& b) { return b.id == bookId; });
        if (book != books.end()) {
            if (!book->isIssued) {
                book->isIssued = true;
                student->issuedBooks.push_back(bookId);
                cout << "Book issued successfully to " << student->name << endl;
            } else {
                cout << "Book is already issued!" << endl;
            }
        } else {
            cout << "Book not found!" << endl;
        }
    } else {
        cout << "Student not found!" << endl;
    }
}

void Library::returnBook() {
    string rollNumber, bookId;
    cout << "Enter Roll Number: ";
    cin >> rollNumber;
    cout << "Enter Book ID: ";
    cin >> bookId;

    auto student = find_if(students.begin(), students.end(), [&rollNumber](const Student& s) { return s.rollNumber == rollNumber; });
    if (student != students.end()) {
        auto it = find(student->issuedBooks.begin(), student->issuedBooks.end(), bookId);
        if (it != student->issuedBooks.end()) {
            student->issuedBooks.erase(it);
            auto book = find_if(books.begin(), books.end(), [&bookId](Book& b) { return b.id == bookId; });
            if (book != books.end()) {
                book->isIssued = false;
                cout << "Book returned successfully!" << endl;
            }
        } else {
            cout << "Book not issued to this student!" << endl;
        }
    } else {
        cout << "Student not found!" << endl;
    }
}

void Library::searchBook() const {
    string title;
    cout << "Enter Book Title: ";
    cin.ignore();
    getline(cin, title);

    auto book = find_if(books.begin(), books.end(), [&title](const Book& b) { return b.title == title; });
    if (book != books.end()) {
        book->display();
    } else {
        cout << "Book not found!" << endl;
    }
}

void Library::deleteBook() {
    string bookId;
    cout << "Enter Book ID to delete: ";
    cin >> bookId;

    auto it = remove_if(books.begin(), books.end(), [&bookId](const Book& b) { return b.id == bookId; });
    if (it != books.end()) {
        books.erase(it, books.end());
        cout << "Book deleted successfully!" << endl;
    } else {
        cout << "Book not found!" << endl;
    }
}

void Library::displayIssuedBooks() const {
    for (const auto& book : books) {
        if (book.isIssued) {
            book.display();
        }
    }
}
