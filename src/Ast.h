#ifndef AST_H
#define AST_H

#include "Number.h"
#include "Token.h"
#include "Value.h"

#include <vector>

struct Expr {
    virtual Value *eval() = 0;
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Expr() = default;
};

struct Ast {
    Expr *root;
    std::string to_string(int depth = 0);
    Value *eval();
    ~Ast();
};

class LiteralExpr : public Expr {
  private:
    Token root;
    Value *value;
    std::string to_string(int depth = 0);

  public:
    LiteralExpr(Token root, Value *value);
    ~LiteralExpr();

    Value *eval();
};

class UnaryExpr : public Expr {
  private:
    Token root;
    Expr *next;

    std::string to_string(int depth = 0);

  public:
    UnaryExpr(Token root, Expr *next) : root{root}, next{next} {}
    Value *eval();
    ~UnaryExpr();
};

class BinaryExpr : public Expr {
  private:
    Token root;
    Expr *left;
    Expr *right;

    std::string to_string(int depth = 0);

  public:
    BinaryExpr(Token root, Expr *left, Expr *right)
        : root{root}, left{left}, right{right} {}
    Value *eval();
    ~BinaryExpr();
};

#endif