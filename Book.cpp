#include "Book.h"

Book::Book() : isIssued(false) {}

Book::Book(string id, string title, string category) 
    : id(id), title(title), category(category), isIssued(false) {}

void Book::display() const {
    cout << "ID: " << id << ", Title: " << title << ", Category: " << category << ", Issued: " << (isIssued ? "Yes" : "No") << endl;
}
