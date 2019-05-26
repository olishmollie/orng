#include "Lexer.h"

#include <iostream>

using namespace std;

int main() {
    cout << "ORNG" << endl;

    while (true) {
        cout << "=> ";

        string buffer;
        getline(cin, buffer);

        if (buffer.length() == 0) {
            break;
        } // eof

        Lexer lexer(buffer);
        while (!lexer.eof()) {
            cout << lexer.lex() << endl;
        }
    }

    cout << endl;

    return 0;
}