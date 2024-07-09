#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Student.h"
#include <vector>

using namespace std;

class Library {
private:
    vector<Book> books;
    vector<Student> students;

    void loadBooks();
    void loadStudents();
    void saveStudent(const Student& student);

public:
    Library();
    void registerStudent();
    void displayBooks() const;
    void issueBook();
    void returnBook();
    void searchBook() const;
    void deleteBook();
    void displayIssuedBooks() const;
};

#endif
