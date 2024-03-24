#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isLetter(char ch)
{
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

bool isDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

enum TokenType
{
    N = 0,
    ID,
    OP,
    KEY,
    LIT,
    PUN
};

enum ColumnType
{
    slash = 0,
    star,
    other,
    A,
    R,
    T
};

ColumnType getColumnType(char ch)
{
    if (ch == '/')
        return slash;
    if (ch == '*')
        return star;
    else
        return other;
}

int TT[5][6] = {
    {1, 0, 0, false, false, N},
    {0, 2, 0, false, false, N},
    {0, 3, 2, false, false, N},
    {4, 3, 2, false, false, N},
    {0, 0, 0, true, false, N},
};

struct Token
{
    string name;
    string type;
};

#define BUFFER_SIZE 1024

char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];

char *currentBuffer = nullptr;

int state = 0;

FILE *file = nullptr;

void openFile(char *filename)
{
    file = fopen(filename, "rb");
    if (file == nullptr)
    {
        cout << "Error opening file!" << endl;
        return;
    }
    else
    {
        cout << "File opened successfully!" << endl;
        return;
    }
}

void readFile()
{
    if (file != nullptr)
    {
        if (currentBuffer == nullptr || currentBuffer == buffer2)
        {
            size_t bytesRead = fread(buffer1, 1, BUFFER_SIZE, file);
            if (bytesRead < BUFFER_SIZE)
            {
                if (feof(file))
                {
                    currentBuffer = nullptr;
                    fclose(file);
                    file = nullptr;
                }
                else
                {
                    cout << "Error: Could not read the complete buffer. Potential file error." << endl;
                }
            }
            else
            {
                currentBuffer = buffer1;
            }
        }
        else
        {
            size_t bytesRead = fread(buffer2, 1, BUFFER_SIZE, file);
            if (bytesRead < BUFFER_SIZE)
            {
                if (feof(file))
                {
                    currentBuffer = nullptr;
                    fclose(file);
                    file = nullptr; // Reset the file pointer
                }
                else
                {
                    // Handle potential read error
                    cout << "Error: Could not read the complete buffer. Potential file error." << endl;
                }
            }
            else
            {
                currentBuffer = buffer2;
            }
        }
    }
    else
    {
        cout << "File not opened!" << endl;
        currentBuffer = nullptr;
    }
}

void printMultiLineComment()
{
    int bufferIndex = 0;
    while (bufferIndex < BUFFER_SIZE && currentBuffer[bufferIndex] != '\0')
    {
        char ch = currentBuffer[bufferIndex];
        ColumnType col = getColumnType(ch);
        state = TT[state][col];
        if (state == 1 || state == 2 || state == 3)
        {
            cout << ch;
        }
        else if (state == 4)
        {
            state = 0; // Reset state to initial state
        }
        bufferIndex++;
    }
}

int main()
{
    char filename[] = "comment.cpp";

    openFile(filename);

    do
    {
        cout << "Reading file..." << endl;
        readFile();
        printMultiLineComment();
    } while (currentBuffer != nullptr);
    return 0;
}
