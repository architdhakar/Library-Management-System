#include "Library.h"
#include <iostream>
using namespace std;

int main() {
    Library lib;
    int choice;

    do {
        cout << "\n===== Library Menu =====\n";
        cout << "1. Register Student\n";
        cout << "2. Add Book\n";
        cout << "3. Display Books\n";
        cout << "4. Search Book\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Delete Book\n";
        cout << "8. Display Issued Books\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.registerStudent(); break;
            case 2: lib.addBook(); break;
            case 3: lib.displayBooks(); break;
            case 4: lib.searchBook(); break;
            case 5: lib.issueBook(); break;
            case 6: lib.returnBook(); break;
            case 7: lib.deleteBook(); break;
            case 8: lib.displayIssuedBooks(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
