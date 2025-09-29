#pragma once
#include "Models.h"

class EmailService {
public:
    // Sends a due date reminder.
    // In a real app, this would use an SMTP library to send an actual email.
    void sendDueDateReminder(const Student& student, const Book& book, const string& dueDate);
};