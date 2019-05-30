#ifndef AST_H
#define AST_H

#include "Error.hpp"
#include "Matrix.hpp"
#include "Number.hpp"
#include "Token.hpp"

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
    virtual Value eval() = 0;
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Expr() = default;
};

struct Ast {
    Expr *root;
    std::string to_string(int depth = 0);
    Value eval();
    ~Ast();
};

class LiteralExpr : public Expr {
  private:
    Token root;
    Matrix *matrix;
    std::string to_string(int depth = 0);
    ~LiteralExpr();

  public:
    LiteralExpr(Token root, Matrix *matrix);
    Value eval();
};

class UnaryExpr : public Expr {
  private:
    Token root;
    Expr *next;
    std::string to_string(int depth = 0);
    ~UnaryExpr();

    Value iota();
    Value shape();

  public:
    UnaryExpr(Token root, Expr *next);
    Value eval();
};

class BinaryExpr : public Expr {
  private:
    Token root;
    Expr *left;
    Expr *right;
    std::string to_string(int depth = 0);
    ~BinaryExpr();

    Value reshape();

  public:
    BinaryExpr(Token root, Expr *left, Expr *right);
    Value eval();
};

#endif