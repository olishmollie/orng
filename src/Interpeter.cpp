#include "Interpreter.hpp"

Interpreter::Interpreter(char *source_)
    : source{(std::string)source_}, parser{Parser(source_)} {}

void Interpreter::read_eval_print() {
    Ast ast = parser.parse();

    std::unique_ptr<Matrix> result = ast.eval();
    std::cout << *result;
}
