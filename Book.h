#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

using namespace std;

class Book {
public:
    string id;
    string title;
    string category;
    bool isIssued;

    Book();
    Book(string id, string title, string category);
    void display() const;
};

#endif
