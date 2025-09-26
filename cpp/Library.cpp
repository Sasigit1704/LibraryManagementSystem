#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;
struct Book {
    int id;
    string title;
    string author;
    bool available;
};
// ---------------- JSON Saving ----------------
void saveBooksJSON(const vector<Book>& books) {
    ofstream out("books.json");
    out << "[\n";
    for(size_t i = 0; i < books.size(); ++i) {
        out << "  {"
            << "\"id\": " << books[i].id << ", "
            << "\"title\": \"" << books[i].title << "\", "
            << "\"author\": \"" << books[i].author << "\", "
            << "\"status\": \"" << (books[i].available ? "Available" : "Issued") << "\"}";
        if(i != books.size() - 1) out << ",";
        out << "\n";
    }
    out << "]";
    out.close();
}
// ---------------- JSON Loading ----------------
void loadBooksJSON(vector<Book>& books) {
    ifstream in("books.json");
    if(!in) {
        cout << "No existing books.json found. Preloading 50 books.\n";
        for(int i = 1; i <= 50; i++) {
            books.push_back({100 + i, "Book " + to_string(i),
                             "Author " + string(1, char(64 + (i%26 ? i%26 : 26))),
                             rand() % 2 == 0});
        }
        saveBooksJSON(books);
        return;
    }
    books.clear();
    string line, key, value;
    Book temp;
    while(getline(in, line)) {
        // Simple parsing: look for id, title, author, status
        size_t pos;
        if((pos = line.find("\"id\":")) != string::npos) {
            temp.id = stoi(line.substr(pos + 5));
        } else if((pos = line.find("\"title\":")) != string::npos) {
            size_t start = line.find("\"", pos + 8) + 1;
            size_t end = line.find("\"", start);
            temp.title = line.substr(start, end - start);
        } else if((pos = line.find("\"author\":")) != string::npos) {
            size_t start = line.find("\"", pos + 9) + 1;
            size_t end = line.find("\"", start);
            temp.author = line.substr(start, end - start);
        } else if((pos = line.find("\"status\":")) != string::npos) {
            size_t start = line.find("\"", pos + 9) + 1;
            size_t end = line.find("\"", start);
            string stat = line.substr(start, end - start);
            temp.available = (stat == "Available");
            books.push_back(temp);
        }
    }
    in.close();
}
// ---------------- Utility Functions ----------------
bool stringContains(string a, string b) {
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    return a.find(b) != string::npos;
}
// ---------------- Search Book ----------------
void searchBook(vector<Book>& books, const string& keyword) {
    bool found = false;
    for(auto& b : books) {
        if(stringContains(b.title, keyword) || stringContains(b.author, keyword)) {
            cout << "ID: " << b.id << ", Title: " << b.title
                 << ", Author: " << b.author
                 << ", Status: " << (b.available ? "Available" : "Issued") << endl;
            found = true;
        }
    }
    if(!found) cout << "Book not found!" << endl;
}
// ---------------- Display Books ----------------
void displayBooks(vector<Book>& books) {
    cout << "ID\tTitle\tAuthor\tStatus\n";
    for(auto& b : books) {
        cout << b.id << "\t" << b.title << "\t" << b.author << "\t"
             << (b.available ? "Available" : "Issued") << endl;
    }
}
// ---------------- Add Book ----------------
void addBook(vector<Book>& books) {
    Book b;
    cout << "Enter Book ID: ";
    cin >> b.id;
    cin.ignore();
    cout << "Enter Book Title: ";
    getline(cin, b.title);
    cout << "Enter Author: ";
    getline(cin, b.author);
    b.available = true;
    books.push_back(b);
    saveBooksJSON(books);
    cout << "Book added successfully!\n";
}
// ---------------- Issue Book ----------------
void issueBook(vector<Book>& books) {
    int id;
    cout << "Enter Book ID to issue: ";
    cin >> id;
    for(auto& b : books) {
        if(b.id == id) {
            if(b.available) {
                b.available = false;
                cout << "Book issued successfully!\n";
            } else {
                cout << "Book is already issued!\n";
            }
            saveBooksJSON(books);
            return;
        }
    }
    cout << "Book not found!\n";
}
// ---------------- Return Book ----------------
void returnBook(vector<Book>& books) {
    int id;
    cout << "Enter Book ID to return: ";
    cin >> id;
    for(auto& b : books) {
        if(b.id == id) {
            if(!b.available) {
                b.available = true;
                cout << "Book returned successfully!\n";
            } else {
                cout << "Book was not issued!\n";
            }
            saveBooksJSON(books);
            return;
        }
    }
    cout << "Book not found!\n";
}
// ---------------- Delete Book ----------------
void deleteBook(vector<Book>& books) {
    int id;
    cout << "Enter Book ID to delete: ";
    cin >> id;
    auto it = remove_if(books.begin(), books.end(), [id](Book &b) { return b.id == id; });
    if(it != books.end()) {
        books.erase(it, books.end());
        saveBooksJSON(books);
        cout << "Book deleted successfully!\n";
    } else {
        cout << "Book not found!\n";
    }
}
// ---------------- Main Menu ----------------
int main() {
    vector<Book> books;
    loadBooksJSON(books);
    int choice;
    string keyword;
    do {
        cout << "\nLibrary Menu:\n";
        cout << "1. Display Books\n";
        cout << "2. Add Book\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book\n";
        cout << "6. Delete Book\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        switch(choice) {
            case 1: displayBooks(books); break;
            case 2: addBook(books); break;
            case 3: issueBook(books); break;
            case 4: returnBook(books); break;
            case 5: 
                cout << "Enter keyword to search (title/author): ";
                getline(cin, keyword);
                searchBook(books, keyword); 
                break;
            case 6: deleteBook(books); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while(choice != 0);
    return 0;
}