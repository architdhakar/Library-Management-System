#include "Student.h"

Student::Student() {}

Student::Student(string rollNumber, string name, string password)
    : rollNumber(rollNumber), name(name), password(password) {}

void Student::display() const {
    cout << "Roll Number: " << rollNumber << ", Name: " << name << ", Issued Books: ";
    for (const auto& bookId : issuedBooks) {
        cout << bookId << " ";
    }
    cout << endl;
}
