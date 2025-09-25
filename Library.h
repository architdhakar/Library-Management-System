#ifndef LIBRARY_H
#define LIBRARY_H

#include "LibraryDB.h"
#include "Book.h"
#include "Student.h"
#include <string>
using namespace std;

class Library {
private:
    LibraryDB db;
public:
    Library();

    void registerStudent();
    void addBook();
    void displayBooks() const;
    void searchBook() const;
    void issueBook();
    void returnBook();
    void deleteBook();
    void displayIssuedBooks() const;
};

#endif
