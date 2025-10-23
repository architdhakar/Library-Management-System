# Library Management System

A complete Command Line Interface (CLI) application for managing a library, written in C++ with an SQLite backend. This project demonstrates core Object-Oriented Programming (OOP) principles, database management, and a modular, user-friendly design.

---

##  Features

###  Admin System
- **Secure Login**: Admins log in with a unique email and password.
- **Admin Registration**: A logged-in admin can create new admin accounts.
- **Book Management**:
  - Add new books to the library catalogue.
  - Remove books from the catalogue.
  - Update the total quantity of existing books.
- **Student Oversight**:
  - View a list of all registered students and their outstanding fines.
  - Reset passwords for student accounts.


###  Student System
- **Secure Login**: Students log in with their roll number and password.
- **Student Registration**: New users can register for a student account from the main menu.
- **Book Operations**:
  - Search the library catalogue by book title or author.
  - Issue available books (loan period is 14 days).
  - Return issued books.
- **Personal Tracking**:
  - View personal history of all issued and returned books.
  - Check current outstanding fine amount.

###  Fine Calculation
- **Automatic Fines**: A fine of **₹2 per day** is automatically calculated and added to a student's account for each overdue book upon its return.

---

##  Technology Stack

* **Backend**: C++ (17)
* **Database**: SQLite3
* **Build System**: CMake
* **Core Concepts**: Object-Oriented Programming (OOP), SQL, Data Structures

---

##  How to Build and Run

### Prerequisites
1.  **C++ Compiler**: A modern C++ compiler (like GCC, Clang, or MSVC) that supports C++17.
2.  **CMake**: Version 3.10 or higher.
3.  **SQLite3 Development Library**:
    * **On Debian/Ubuntu**: `sudo apt-get install build-essential cmake libsqlite3-dev`
    * **On Fedora/CentOS**: `sudo dnf install cmake make gcc-c++ sqlite-devel`
    * **On macOS (with Homebrew)**: `brew install cmake sqlite3`

### Build Steps
1.  Place all the source files (`.cpp`, `.h`, `CMakeLists.txt`) in your project directory.

2.  Open a terminal in your project directory and run the following commands:
    ```bash
    # Create a separate directory for build files
    mkdir build
    cd build

    # Configure the project with CMake
    cmake ..

    # Compile the source code
    make
    ```

### Run the Application
After a successful build, an executable named `library_backend` (or `library_backend.exe` on Windows) will be created in the `build` directory. Run it from your main project directory:

```bash
# Go back to the main project directory from 'build'
cd ..

# Run the executable
./build/library_backend
```

---

## How to Use

### Default Credentials
* **Admin**:
    * Email: `admin@lib.com`
    * Password: `admin123`
* **Student**:
    * Roll Number: `S001`
    * Password: `student123`

### Registration
* **New Students**: Choose option `3` from the main menu to register.
* **New Admins**: Log in as an admin and choose option `7` from the admin dashboard to create a new admin account.

---

