#include<iostream>
#include <cstring>
#include <cstdio>

using namespace std;

#define BUFFER_SIZE 4096

char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];

char* currentBuffer = nullptr;

FILE* file = nullptr;

void openFile() {
    char filename[] = "LAB_1.cpp";
    // cout << "Enter the file name: ";
    // cin >> filename;

    file = fopen(filename, "rb");
    if (file == nullptr) {
        cout << "Error opening file!" << endl;
        return;
    } else {
        cout << "File openned sucessfulluy!" << endl;
        return;
    }
}

void readFile() {
    if (file != nullptr) {
        size_t bytesRead = fread(buffer1, 1, BUFFER_SIZE, file);

        if (bytesRead < BUFFER_SIZE) {
            if (feof(file)) {
                cout << "End of file reached." << endl;
            } else {
                cout << "Error reading from file." << endl;
            }
        }
    }
    currentBuffer = buffer1;
    return;
}

int main() {
    openFile();
    readFile();
    
    cout << currentBuffer << endl;

    return 0;
}