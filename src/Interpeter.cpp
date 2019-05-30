#include "Interpreter.hpp"

Interpreter::Interpreter(char *source_)
    : source{static_cast<std::string>(source_)}, parser{Parser(source_)} {}

void Interpreter::read_eval_print() {
    Ast ast = parser.parse();

    Value result = ast.eval();
    std::cout << *result;
}
