#include "Student.h"

Student::Student() {}

Student::Student(string rollNumber, string name, string password)
    : rollNumber(rollNumber), name(name), password(password) {}

void Student::display() const {
    cout << "Roll Number: " << rollNumber 
         << ", Name: " << name << endl;
}
