#pragma once

#include <any>
#include <cassert>
#include <sstream>
#include <type_traits>


#include "Expr.hpp"

class AstPrinter : public ExprVisitor {
public:
  std::string print(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
  }

  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, expr->left, expr->right);
  }

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) {
    if(expr->value.type() == typeid(nullptr)) {
      return "nil";
    } 
    else if(expr->value.type() == typeid(double)) {
      return std::to_string(std::any_cast<double>(expr->value));
    }  else {
      return "Error: Unexpected literal type ";
    }
    
  }

private:
  template <class... E>
  std::string parenthesize(std::string_view name, E... expr) {
    assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));

    std::ostringstream builder;

    builder << "(" << name;
    ((builder << " " << print(expr)), ...);
    builder << ")";

    return builder.str();
  }
};
