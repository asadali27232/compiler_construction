#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

enum ColumnType
{
    qoute = 0,
    notqouteslash,
    slash,
    escapesequence,
    A,
};

ColumnType getColumnType(char ch)
{
    if (ch == '"')
        return qoute;
    if (ch != '"' && ch != '\\')
        return notqouteslash;
    if (ch == '\\')
        return slash;
    if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' || ch == 'v' || ch == '\\' || ch == '?' || ch == '\'' || ch == '"')
        return escapesequence;
}

int TT[4][5] = {
    {1, -1, -1, -1, false},
    {3, 1, 2, -1, false},
    {-1, -1, -1, 1, false},
    {-1, -1, -1, -1, true},
};

bool isStringLiteral(const string &line)
{
    int currentState = 0;
    for (char ch : line)
    {
        ColumnType type = getColumnType(ch);
        currentState = TT[currentState][type];
        if (currentState == -1)
            return false;
    }
    return currentState == 3;
}

string removeStringLiterals(const string &line)
{
    string result;
    int currentState = 0;
    for (char ch : line)
    {
        ColumnType type = getColumnType(ch);
        currentState = TT[currentState][type];
        if (currentState == 1)
            result += ch;
        if (currentState == 3)
            currentState = 0;
    }
    return result;
}

void processFile(const string &inputFilename, const string &outputFilename)
{
    ifstream inputFile(inputFilename);
    ofstream outputFile(outputFilename);
    if (!inputFile || !outputFile)
    {
        cout << "Error opening files!" << endl;
        return;
    }

    string line;
    bool inStringLiteral = false;
    while (getline(inputFile, line))
    {
        for (size_t i = 0; i < line.size(); ++i)
        {
            if (line[i] == '"')
            {
                inStringLiteral = !inStringLiteral;
                while (i < line.size() && line[i] != '"')
                    ++i;
            }
            if (!inStringLiteral && line[i] != '"')
                outputFile << line[i];
        }
        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();
}

int main()
{
    string inputFilename = "input.cpp";
    string outputFilename = "output.cpp";

    processFile(inputFilename, outputFilename);

    cout << "String literals removed and code saved to output file successfully!" << endl;

    return 0;
}
