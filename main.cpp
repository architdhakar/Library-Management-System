#include "Library.h"
#include <iostream>

using namespace std;

int main() {
    Library library;
    int choice;

    do {
        cout << "Library Management System" << endl;
        cout << "1. Register Student" << endl;
        cout << "2. View Books" << endl;
        cout << "3. Issue Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Search Book" << endl;
        cout << "6. Delete Book" << endl;
        cout << "7. Display Issued Books" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            library.registerStudent();
            break;
        case 2:
            library.displayBooks();
            break;
        case 3:
            library.issueBook();
            break;
        case 4:
            library.returnBook();
            break;
        case 5:
            library.searchBook();
            break;
        case 6:
            library.deleteBook();
            break;
        case 7:
            library.displayIssuedBooks();
            break;
        case 8:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
