#include <iostream>
#include <cstring>

using namespace std;

class LinkedList {
private:
    struct Node {
        string data;
        Node* next;
        Node(const string& d) : data(d), next(nullptr) {}
    };
    Node* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(const string& data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

   bool printList() const {
    if (head == nullptr) {
        return false;
    }
    Node* temp = head;
    while (temp != nullptr) {
        cout << " --> " << temp->data;
        temp = temp->next;
    }
    return true;
}
};

class HashTable {
private:
    LinkedList hashTable[7];

    int hashFunction(const string& word) {
        int sum = 0;
        for (char letter : word) {
            sum += letter;
        }
        return sum % 10;
    }

public:
    void insert(const string& word) {
        int index = hashFunction(word);
        hashTable[index].insert(word);
    }

    void printHashTable() {
    for (int i = 0; i < 7; ++i) {
        cout << "Index " << i + 1 << ": " << endl;
        hashTable[i].printList();
        cout << endl;
    }
}

};

int main() {
    const char paragraph[] = "Abc Efg Hij. Klm Opq Rst.Uvw Xyz.";
    cout << endl << paragraph << endl << endl;

    HashTable hashTable;

    string word = "";
    int counter = 0;
    do {
        char letter = paragraph[counter];

        if (letter == ' ' || letter == '\0' || letter == '.') {
            if (!word.empty()) {
                hashTable.insert(word);
                word = "";
            }
        } else {
            word += letter;
        }

        counter++;

    } while (paragraph[counter] != '\0');

    cout << "Printing the hash table:" << endl << endl;
    hashTable.printHashTable();

    return 0;
}
