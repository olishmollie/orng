#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Ast.h"
#include "Parser.h"
#include "Value.h"

#include <string>

class Interpreter {
    std::string source;
    Parser parser;

  public:
    Interpreter(char *source);
    void read_eval_print();
};

#endif