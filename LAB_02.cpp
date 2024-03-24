#include<iostream>
#include <cstring>
#include <cstdio>

using namespace std;

struct Token {
    string name;
    string type;
};

const int MAX_TOKENS = 500;
int currentTokenPosition = 0;
Token tokenArray[MAX_TOKENS];

#define BUFFER_SIZE 1024

char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];

char* currentBuffer = nullptr;

int state = 0;

FILE* file = nullptr;

void openFile() {
    char filename[] = "dummy.cpp";

    file = fopen(filename, "rb");
    if (file == nullptr) {
        cout << "Error opening file!" << endl;
        return;
    } else {
        cout << "File opened successfully!" << endl;
        return;
    }
}

void readFile() {
    if (file != nullptr) {
        if (currentBuffer == nullptr || currentBuffer == buffer2) {
            size_t bytesRead = fread(buffer1, 1, BUFFER_SIZE, file);
            if (bytesRead == 0) {
                currentBuffer = nullptr; // Reached end of file
            } else {
                currentBuffer = buffer1;
                cout << "Current Buffer is buffer1" << endl;
            }
        } else {
            size_t bytesRead = fread(buffer2, 1, BUFFER_SIZE, file);
            if (bytesRead == 0) {
                currentBuffer = nullptr; // Reached end of file
            } else {
                currentBuffer = buffer2;
                cout << "Current Buffer is buffer2" << endl;
            }
        }
    }
}

void tokenGen(string tokenName, string tokenType) {
    Token newToken;
    newToken.name = tokenName;
    newToken.type = tokenType;

    tokenArray[currentTokenPosition] = newToken;
    currentTokenPosition++;
}

bool isLetter(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

void identifierFinder() {
    if (currentBuffer == nullptr) {
        cout << "Exited as file ended" << endl;
        return; // Exit if end of file reached
    }
    int bufferIndex = 0;
    int lexemeIndex = 0;

    char ch = currentBuffer[bufferIndex];
    char lexeme[50];

    for (bufferIndex = 0; bufferIndex < BUFFER_SIZE; bufferIndex++) {
        switch (state) {
            case 0:
                if (ch == '_' || isLetter(ch)) {
                    state = 1;
                    lexeme[lexemeIndex] = ch;
                    lexemeIndex++;
                }
                break;

            case 1:
                if (ch == '_' || isLetter(ch) || isDigit(ch)) {
                    state = 1;
                    lexeme[lexemeIndex] = ch;
                    lexemeIndex++;
                } else {
                    state = 2;
                }
                break;

            case 2:
                lexeme[lexemeIndex] = '\0';
                string lexemeString = lexeme;
                state = 0;
                bufferIndex--;
                tokenGen(lexemeString, "id");
                lexemeIndex = 0; // Reset lexemeIndex for the next identifier
                break;
        }
        ch = currentBuffer[bufferIndex];
    }
    return;
}

void printTokens() {
    for (int i = 0; i < currentTokenPosition; ++i) {
        cout << "Token " << i+1 << ":" << endl;
        cout << "  Name: " << tokenArray[i].name << endl;
        cout << "  Type: " << tokenArray[i].type << endl;
    }
}

int main() {
    openFile();
    do {
        readFile();
        identifierFinder();
    } while (currentBuffer != nullptr);
    printTokens();
    

    return 0;
}
