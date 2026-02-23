#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

/* ================= BOOK CLASS ================= */

class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;

    Book() {}
    Book(int i, string t, string a, bool issued = false)
        : id(i), title(t), author(a), isIssued(issued) {}
};

/* ================= STUDENT CLASS ================= */

class Student {
public:
    int id;
    string name;

    Student() {}
    Student(int i, string n) : id(i), name(n) {}
};

/* ================= ISSUE CLASS ================= */

class Issue {
public:
    int bookId;
    int studentId;
    int issueDate;   // store as timestamp
    int dueDate;

    Issue() {}
    Issue(int b, int s, int issue, int due)
        : bookId(b), studentId(s), issueDate(issue), dueDate(due) {}
};

/* ================= LIBRARY CLASS ================= */

class Library {
private:
    vector<Book> books;
    vector<Student> students;
    vector<Issue> issues;

    /* --------- FILE LOAD & SAVE --------- */

    void loadBooks() {
        ifstream file("books.txt");
        Book b;
        while (file >> b.id >> ws) {
            getline(file, b.title);
            getline(file, b.author);
            file >> b.isIssued;
            books.push_back(b);
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        for (auto &b : books) {
            file << b.id << endl;
            file << b.title << endl;
            file << b.author << endl;
            file << b.isIssued << endl;
        }
        file.close();
    }

    void loadStudents() {
        ifstream file("students.txt");
        Student s;
        while (file >> s.id >> ws) {
            getline(file, s.name);
            students.push_back(s);
        }
        file.close();
    }

    void saveStudents() {
        ofstream file("students.txt");
        for (auto &s : students) {
            file << s.id << endl;
            file << s.name << endl;
        }
        file.close();
    }

    void loadIssues() {
        ifstream file("issues.txt");
        Issue i;
        while (file >> i.bookId >> i.studentId >> i.issueDate >> i.dueDate) {
            issues.push_back(i);
        }
        file.close();
    }

    void saveIssues() {
        ofstream file("issues.txt");
        for (auto &i : issues) {
            file << i.bookId << " "
                 << i.studentId << " "
                 << i.issueDate << " "
                 << i.dueDate << endl;
        }
        file.close();
    }

public:
    Library() {
        loadBooks();
        loadStudents();
        loadIssues();
    }

    /* --------- BOOK FUNCTIONS --------- */

    void addBook() {
        int id;
        string title, author;

        cout << "Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Title: ";
        getline(cin, title);

        cout << "Author: ";
        getline(cin, author);

        books.push_back(Book(id, title, author));
        saveBooks();
        cout << "Book added successfully.\n";
    }

    void viewBooks() {
        cout << left << setw(8) << "ID"
             << setw(25) << "Title"
             << setw(20) << "Author"
             << setw(10) << "Status\n";

        for (auto &b : books) {
            cout << left << setw(8) << b.id
                 << setw(25) << b.title
                 << setw(20) << b.author
                 << setw(10) << (b.isIssued ? "Issued" : "Available")
                 << endl;
        }
    }

    void searchBook() {
        string keyword;
        cin.ignore();
        cout << "Enter title keyword: ";
        getline(cin, keyword);

        for (auto &b : books) {
            if (b.title.find(keyword) != string::npos) {
                cout << "Found: " << b.title << " by " << b.author << endl;
            }
        }
    }

    /* --------- STUDENT FUNCTIONS --------- */

    void addStudent() {
        int id;
        string name;

        cout << "Student ID: ";
        cin >> id;
        cin.ignore();

        cout << "Student Name: ";
        getline(cin, name);

        students.push_back(Student(id, name));
        saveStudents();
        cout << "Student registered.\n";
    }

    /* --------- ISSUE & RETURN --------- */

    void issueBook() {
        int bookId, studentId;

        cout << "Book ID: ";
        cin >> bookId;

        cout << "Student ID: ";
        cin >> studentId;

        for (auto &b : books) {
            if (b.id == bookId && !b.isIssued) {
                time_t now = time(0);
                int due = now + (7 * 24 * 60 * 60); // 7 days

                issues.push_back(Issue(bookId, studentId, now, due));
                b.isIssued = true;

                saveBooks();
                saveIssues();

                cout << "Book issued. Due in 7 days.\n";
                return;
            }
        }
        cout << "Book unavailable.\n";
    }

    void returnBook() {
        int bookId;
        cout << "Book ID: ";
        cin >> bookId;

        for (auto it = issues.begin(); it != issues.end(); ++it) {
            if (it->bookId == bookId) {
                time_t now = time(0);
                int fine = 0;

                if (now > it->dueDate) {
                    int daysLate = (now - it->dueDate) / (24 * 60 * 60);
                    fine = daysLate * 5; // 5 per day
                }

                cout << "Fine: " << fine << endl;

                for (auto &b : books) {
                    if (b.id == bookId)
                        b.isIssued = false;
                }

                issues.erase(it);
                saveBooks();
                saveIssues();
                return;
            }
        }
        cout << "No issue record found.\n";
    }
};

/* ================= MAIN ================= */

int main() {
    Library lib;
    int choice;

    do {
        cout << "\n===== LIBRARY SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Search Book\n";
        cout << "4. Add Student\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.viewBooks(); break;
            case 3: lib.searchBook(); break;
            case 4: lib.addStudent(); break;
            case 5: lib.issueBook(); break;
            case 6: lib.returnBook(); break;
        }

    } while (choice != 7);

    return 0;
}