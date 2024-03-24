#include <iostream>
#include <cstring>

using namespace std;

class LinkedList
{
private:
    struct Node
    {
        string data;
        Node *next;
        Node(const string &d) : data(d), next(nullptr) {}
    };
    Node *head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(const string &data)
    {
        Node *newNode = new Node(data);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node *temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    bool search(const string &target, int &occurrences) const
    {
        Node *current = head;
        int count = 0;
        int index = -1;

        while (current != nullptr)
        {
            if (current->data == target)
            {
                if (index == -1)
                {
                    index = count; // Save the first index where the word is found
                }
                occurrences++; // Increment count for each occurrence
            }
            count++;
            current = current->next;
        }

        return (index != -1);
    }

    bool printList() const
    {
        if (head == nullptr)
        {
            return false;
        }
        Node *temp = head;
        while (temp != nullptr)
        {
            cout << " --> " << temp->data;
            temp = temp->next;
        }
        return true;
    }
};

class HashTable
{
private:
    LinkedList hashTable[7];

    int hashFunction(const string &word)
    {
        int sum = 0;
        for (char letter : word)
        {
            sum += letter;
        }
        return sum % 10;
    }

public:
    void insert(const string &word)
    {
        int index = hashFunction(word);
        hashTable[index].insert(word);
    }

    int search(const string &word, int &occurrences)
    {
        int index = hashFunction(word);
        occurrences = 0;
        return (hashTable[index].search(word, occurrences)) ? index : -1;
    }

    void printHashTable()
    {
        for (int i = 0; i < 7; ++i)
        {
            cout << "Index " << i + 1 << ": " << endl;
            hashTable[i].printList();
            cout << endl;
        }
    }
};

int main()
{
    string paragraph;
    cout << "Enter a paragraph: ";
    getline(cin, paragraph);

    HashTable hashTable;

    string searchWord;
    cout << "Enter a word to search in the hash table: ";
    cin >> searchWord;

    int occurrences = 0;
    int index = hashTable.search(searchWord, occurrences);

    if (index != -1)
    {
        cout << searchWord << " is found in the hash table at index " << index + 1 << " with " << occurrences << " occurrences." << endl;
    }
    else
    {
        cout << searchWord << " is not found in the hash table." << endl;
    }

    return 0;
}
