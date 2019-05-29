#include "Interpreter.h"
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

        // Lexer lexer(buf);
        // while (!lexer.eof()) {
        //     try {
        //         cout << lexer.lex() << endl;
        //     } catch (OrngError &e) {
        //         cout << e.caused_by() << endl;
        //         break;
        //     }
        // }

        try {
            Parser parser(buf);
            Ast ast = parser.parse();
            cout << ast.to_string() << endl;
            // delete ast;
        } catch (OrngError &e) {
            cout << e.caused_by() << endl;
        }

        // try {
        //     Interpreter interpreter(buf);
        //     interpreter.read_eval_print();
        // } catch (OrngError &e) {
        //     cerr << e.caused_by() << endl;
        // }

        free(buf);
    }

    cout << endl;

    return 0;
}