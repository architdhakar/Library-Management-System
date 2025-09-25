#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
using namespace std;

class Student {
public:
    string rollNumber;
    string name;
    string password;

    Student();
    Student(string rollNumber, string name, string password);
    void display() const;
};

#endif
