#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Ast.hpp"
#include "Parser.hpp"
#include "Value.hpp"

#include <string>

class Interpreter {
    std::string source;
    Parser parser;

  public:
    Interpreter(char *source);
    void read_eval_print();
};

#endif