#ifndef AST_H
#define AST_H

#include "Error.h"
#include "Number.h"
#include "Token.h"
#include "Value.h"

#include <vector>

class RuntimeError : public OrngError {
    std::string msg;
    std::string source;
    unsigned int column;

  public:
    RuntimeError(std::string msg, std::string source, unsigned int column);
    std::string caused_by();
};

struct Expr {
    virtual std::unique_ptr<Value> eval() = 0;
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Expr() = default;
};

struct Ast {
    Expr *root;
    std::string to_string(int depth = 0);
    std::unique_ptr<Value> eval();
    ~Ast();
};

class LiteralExpr : public Expr {
  private:
    Token root;
    Value *value;
    std::string to_string(int depth = 0);

  public:
    LiteralExpr(Token root, Value *value);

    std::unique_ptr<Value> eval();
};

class UnaryExpr : public Expr {
  private:
    Token root;
    Expr *next;

    std::string to_string(int depth = 0);

  public:
    UnaryExpr(Token root, Expr *next) : root{root}, next{next} {}
    std::unique_ptr<Value> eval();
    ~UnaryExpr();

    std::unique_ptr<Value> iota();
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
    std::unique_ptr<Value> eval();
    ~BinaryExpr();
};

#endif