#include "Lexer.h"
#include "Parser.h"

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

        Parser parser(buf);
        try {
            Ast *ast = parser.parse();
            cout << ast->to_string() << endl;
            delete ast;
        } catch (ParseError err) {
            cout << err.caused_by() << endl;
        }

        free(buf);
    }

    cout << endl;

    return 0;
}