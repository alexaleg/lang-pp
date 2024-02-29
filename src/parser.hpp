#pragma once

#include <stdexcept>
#include <string_view>
#include <utility>
#include <memory>
#include <cassert>
#include <vector>

#include "Expr.hpp"
#include "error.hpp"
#include "token.hpp"

class Parser {
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> &tokens;
  int current = 0;

public:
  Parser(const std::vector<Token> &tokens) : tokens{tokens} {}

  std::shared_ptr<Expr> parse() {
    try {
      return expression();
    } catch (ParseError error) {
      return nullptr;
    }
  }

private:
  bool isAtEnd() {
    return current >= tokens.size() || peek().type == TokenType::t_EOF;
  }

  bool check(TokenType type) {
    if (isAtEnd())
      return false;
    return peek().type == type;
  }

  Token peek() { return tokens[current]; }

  Token previous() { return tokens[current - 1]; }

  Token advance() {
    if (!isAtEnd())
      current++;
    return previous();
  }

  Token consume(TokenType type, std::string_view message) {
    if (check(type))
      return advance();

    throw error(peek(), message);
  }

  ParseError error(const Token &token, std::string_view message) {
    ::error(token, message);
    return ParseError{""};
  }

  std::shared_ptr<Expr> expression() {
    std::shared_ptr<Expr> expr = primary();

    while (match(t_PLUS)) {
      Token op = previous();
      std::shared_ptr<Expr> right = expression();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> primary() {
    if (match(t_NIL))
      return std::make_shared<Literal>(nullptr);

    if (match(t_NUMBER)) {
      return std::make_shared<Literal>(previous().literal);
    }

    throw error(peek(), "Expected number or literal.");
  }

  template <class... T>
  bool match(T... type) {
    assert((... && std::is_same_v<T, TokenType>));

    if ((... || check(type))) {
      advance();
      return true;
    }

    return false;
  }
  
};
