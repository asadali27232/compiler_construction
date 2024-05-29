#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Parser {
public:
    Parser(const pair<string, string> *tokens, size_t numTokens) 
        : tokens(tokens), numTokens(numTokens), pos(0) {}

    void parse() {
        stack.push_back("Sentence");
        if (sentence()) {
            if (pos == numTokens) {
                cout << "The sentence is syntactically correct." << endl;
            } else {
                error();
            }
        } else {
            error();
        }
    }

private:
    const pair<string, string> *tokens;
    size_t numTokens;
    size_t pos;
    vector<string> stack;

    bool sentence() {
        printStack();
        size_t start_pos = pos;
        if (nounPhrase() && verbPhrase() && pos == numTokens) {
            stack.pop_back();
            printStack();
            return true;
        }
        pos = start_pos;
        return false;
    }

    bool nounPhrase() {
        stack.push_back("NounPhrase");
        printStack();
        size_t start_pos = pos;
        if (determiner() && noun()) {
            stack.pop_back();
            printStack();
            return true;
        }
        pos = start_pos;
        stack.pop_back();
        printStack();
        return false;
    }

    bool verbPhrase() {
        stack.push_back("VerbPhrase");
        printStack();
        size_t start_pos = pos;
        if (verb() && nounPhrase()) {
            stack.pop_back();
            printStack();
            return true;
        }
        pos = start_pos;
        stack.pop_back();
        printStack();
        return false;
    }

    bool prepositionalPhrase() {
        stack.push_back("PrepositionalPhrase");
        printStack();
        size_t start_pos = pos;
        if (preposition() && nounPhrase()) {
            stack.pop_back();
            printStack();
            return true;
        }
        pos = start_pos;
        stack.pop_back();
        printStack();
        return false;
    }

    bool determiner() {
        printStack();
        return match("the", "determiner") || match("a", "determiner");
    }

    bool noun() {
        printStack();
        return match("cat", "noun") || match("dog", "noun") || match("man", "noun") || match("woman", "noun");
    }

    bool verb() {
        printStack();
        return match("chased", "verb") || match("caught", "verb");
    }

    bool preposition() {
        printStack();
        return match("in", "preposition") || match("on", "preposition") || match("with", "preposition");
    }

    bool match(const string &token, const string &type) {
        if (pos < numTokens && tokens[pos].first == token && tokens[pos].second == type) {
            ++pos;
            return true;
        }
        return false;
    }

    void printStack() {
        cout << "Current stack: ";
        for (const auto &s : stack) {
            cout << s << " ";
        }
        cout << endl;
    }

    void error() {
        cout << "Error: The sentence is not syntactically correct." << endl;
        cout << "Stack trace: ";
        for (const auto &s : stack) {
            cout << s << " ";
        }
        cout << endl;
        cout << "Remaining tokens: ";
        for (size_t i = pos; i < numTokens; ++i) {
            cout << "<" << tokens[i].first << "," << tokens[i].second << "> ";
        }
        cout << endl;
    }
};

pair<string, string> *tokenize(const string &input, size_t &numTokens) {
    string token;
    istringstream tokenStream(input);
    numTokens = 0;
    while (getline(tokenStream, token, '<')) {
        if (!token.empty()) {
            ++numTokens;
        }
    }
    pair<string, string> *tokens = new pair<string, string>[numTokens];
    tokenStream.clear();
    tokenStream.seekg(0);
    size_t i = 0;
    while (getline(tokenStream, token, '<')) {
        if (!token.empty()) {
            size_t comma_pos = token.find(',');
            string word = token.substr(0, comma_pos);
            string type = token.substr(comma_pos + 1, token.find('>') - comma_pos - 1);
            tokens[i++] = make_pair(word, type);
        }
    }
    return tokens;
}

int main() {
    string input = "<the,determiner><dog,noun><caught,verb><a,determiner><cat,noun>";
    size_t numTokens;
    pair<string, string> *tokens = tokenize(input, numTokens);

    Parser parser(tokens, numTokens);
    parser.parse();

    delete[] tokens;
    return 0;
}
