#ifndef AST_H
#define AST_H

#include "Token.h"

#include <vector>

struct Ast {
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Ast() = default;
};

class LiteralAst : public Ast {
  private:
    Token root;
    bool is_vector;
    union {
        std::vector<long> *vec;
        long scalar;
    };

    std::string to_string(int depth = 0);

  public:
    LiteralAst(Token root, long scalar);
    LiteralAst(Token root, std::vector<long> *vec);
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