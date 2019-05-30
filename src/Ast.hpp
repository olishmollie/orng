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
    virtual std::unique_ptr<Matrix> eval() = 0;
    virtual std::string to_string(int depth = 0) = 0;
    virtual ~Expr() = default;
};

struct Ast {
    Expr *root;
    std::string to_string(int depth = 0);
    std::unique_ptr<Matrix> eval();
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
    std::unique_ptr<Matrix> eval();
};

class UnaryExpr : public Expr {
  private:
    Token root;
    Expr *next;
    std::string to_string(int depth = 0);
    ~UnaryExpr();

    std::unique_ptr<Matrix> iota();
    std::unique_ptr<Matrix> shape();
    std::unique_ptr<Matrix> pi();
    std::unique_ptr<Matrix> abs();
    std::unique_ptr<Matrix> roll();

  public:
    UnaryExpr(Token root, Expr *next);
    std::unique_ptr<Matrix> eval();
};

class BinaryExpr : public Expr {
  private:
    Token root;
    Expr *left;
    Expr *right;
    std::string to_string(int depth = 0);
    ~BinaryExpr();

    std::unique_ptr<Matrix> reshape();
    std::unique_ptr<Matrix> deal();
    std::unique_ptr<Matrix> add();

  public:
    BinaryExpr(Token root, Expr *left, Expr *right);
    std::unique_ptr<Matrix> eval();
};

#endif