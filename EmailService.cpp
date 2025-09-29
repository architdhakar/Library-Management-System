#include "EmailService.h"
#include <iostream>

void EmailService::sendDueDateReminder(const Student& student, const Book& book, const string& dueDate) {
    cout << "\n--- SIMULATING EMAIL SENT ---" << endl;
    cout << "To: " << student.email << endl;
    cout << "Subject: Library Book Due Soon" << endl;
    cout << "-----------------------------" << endl;
    cout << "Dear " << student.name << "," << endl;
    cout << "This is a reminder that the book '" << book.title << "' is due for return tomorrow, " << dueDate << "." << endl;
    cout << "Please return it on time to avoid any fines." << endl;
    cout << "\nThank you," << endl;
    cout << "The Library" << endl;
    cout << "-----------------------------\n" << endl;
}