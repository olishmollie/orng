#include "Lexer.h"

#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>

using namespace std;

int main() {
    cout << "ORNG" << endl;

    char *buf;
    while ((buf = readline(">> ")) != nullptr) {

        if (strlen(buf) > 0) {
            add_history(buf);
        }

        Lexer lexer(buf);
        while (!lexer.eof()) {
            cout << lexer.lex() << endl;
        }

        free(buf);
    }

    cout << endl;

    return 0;
}