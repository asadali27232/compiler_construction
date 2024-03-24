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
    L = 0,
    U,
    D,
    NLDU,
    A,
    R,
    T
};

ColumnType getColumnType(char ch)
{
    if (isLetter(ch))
        return L;
    if (isDigit(ch))
        return D;
    if (ch == '_')
        return U;
    else
        return NLDU;
}

int TT[3][7] = {
    {1, 1, -1, -1, false, false, N},
    {1, 1, 1, 2, false, false, N},
    {0, 0, 0, 0, true, true, ID},
};

struct Token
{
    string name;
    string type;
};

const int MAX_TOKENS = 500;
int currentTokenPosition = 0;
Token tokenArray[MAX_TOKENS];

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

char *currentBuffer = nullptr;

int state = 0;

ifstream file;

bool openFile(const string &filename)
{
    file.open(filename);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return false;
    }
    cout << "File opened successfully!" << endl;
    return true;
}

void readFile()
{
    if (file.is_open())
    {
        file.read(buffer, BUFFER_SIZE);
        streamsize bytesRead = file.gcount();
        if (bytesRead < BUFFER_SIZE)
        {
            if (file.eof())
            {
                currentBuffer = nullptr;
                file.close();
            }
            else
            {
                cout << "Error: Could not read the complete buffer. Potential file error." << endl;
            }
        }
        else
        {
            currentBuffer = buffer;
        }
    }
    else
    {
        cout << "File not opened!" << endl;
        currentBuffer = nullptr;
    }
}

void tokenGen(const string &tokenName, const string &tokenType)
{
    if (currentTokenPosition < MAX_TOKENS)
    {
        tokenArray[currentTokenPosition].name = tokenName;
        tokenArray[currentTokenPosition].type = tokenType;
        currentTokenPosition++;
    }
    else
    {
        cout << "Maximum token limit reached!" << endl;
    }
}

void identifierFinder()
{
    if (currentBuffer == nullptr)
    {
        cout << "Exited as file ended" << endl;
        return;
    }

    int lexemeIndex = 0;

    for (int bufferIndex = 0; bufferIndex < BUFFER_SIZE; bufferIndex++)
    {
        char ch = currentBuffer[bufferIndex];
        state = TT[state][getColumnType(ch)];
        if (!TT[state][A])
        {
            if (lexemeIndex < 50)
            { // Ensure not to exceed lexeme array size
                tokenArray[currentTokenPosition].name += ch;
                lexemeIndex++;
            }
            else
            {
                cout << "Lexeme length exceeded limit!" << endl;
                return;
            }
        }
        else
        {
            tokenGen(tokenArray[currentTokenPosition].name, "ID");
            tokenArray[currentTokenPosition].name.clear(); // Clear the token name for the next token
            lexemeIndex = 0;
        }
    }
}

void printTokens()
{
    for (int i = 0; i < currentTokenPosition; ++i)
    {
        cout << "Token " << i + 1 << ":" << endl;
        cout << "  Name: " << tokenArray[i].name << endl;
        cout << "  Type: " << tokenArray[i].type << endl;
    }
}

int main()
{
    string filename = "dummy.cpp";

    if (!openFile(filename))
    {
        return 1;
    }

    do
    {
        readFile();
        identifierFinder();
        printTokens();
    } while (currentBuffer != nullptr);

    return 0;
}
