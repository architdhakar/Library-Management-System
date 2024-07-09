#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Student {
public:
    string rollNumber;
    string name;
    string password;
    vector<string> issuedBooks;

    Student();
    Student(string rollNumber, string name, string password);
    void display() const;
};

#endif
