#ifndef AST_H
#define AST_H

#include "Number.h"
#include "Token.h"
#include "Value.h"

#include <vector>

struct Ast {
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Ast() = default;
};

class LiteralAst : public Ast {
  private:
    Token root;
    Value *value;
    std::string to_string(int depth = 0);

  public:
    LiteralAst(Token root, Value *value);
    ~LiteralAst();
};

class UnaryAst : public Ast {
  private:
    Token root;
    Ast *next;

    std::string to_string(int depth = 0);

  public:
    UnaryAst(Token root, Ast *next) : root{root}, next{next} {}
    ~UnaryAst();
};

class BinaryAst : public Ast {
  private:
    Token root;
    Ast *left;
    Ast *right;

    std::string to_string(int depth = 0);

  public:
    BinaryAst(Token root, Ast *left, Ast *right)
        : root{root}, left{left}, right{right} {}
    ~BinaryAst();
};

#endif