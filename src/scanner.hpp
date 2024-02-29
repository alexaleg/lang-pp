#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "error.hpp"
#include "token.hpp"

using std::string;
using std::string_view;
using std::vector;

class Scanner {
  static const std::map<string, TokenType> keywords;

  string_view source;
  vector<Token> tokens;
  int start = 0;
  int current = 0;
  int line = 1;

public:
  Scanner(string_view source) : source{source} {}

  vector<Token> scanTokens() {
    while (!isAtEnd()) {
      start = current;
      scanToken();
    }
    tokens.emplace_back(TokenType::t_EOF, "", nullptr, line);
    return tokens;
  }

private:
  void scanToken() {

    char c = advance();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      break;

    case '\n':
      line++;
      break;

    case '+':
      addToken(t_PLUS, "+");
      break;
    default:
      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        error(line, "Unexpected character");
      } else {
        error(line, "Found unexpected character: " + string{c} + " at position " + std::to_string(current));
      }
      break;
    };
  }

  bool isAtEnd() { return current >= source.length(); }

  char advance() { return source[current++]; }

  char peek() {
    if (isAtEnd())
      return '\0';
    return source[current];
  }

  char peekNext() {
    if (current + 1 >= source.length())
      return '\0';
    return source[current + 1];
  }

  void addToken(TokenType type, std::any literal) {
    string lexeme{source.substr(start, current - start)};
    tokens.emplace_back(type, std::move(lexeme), std::move(literal), line);
  }

  void addToken(TokenType type) { addToken(type, nullptr); }

  bool isDigit(char c) { return c >= '0' && c <= '9'; }

  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  void number() {
    while (isDigit(peek()))
      advance();
    if (peek() == '.' && isDigit(peekNext())) {
      advance();
    }
    while (isDigit(peek()))
      advance();

    addToken(t_NUMBER, std::stod(string{source.substr(start, current - start)}));
  }
};

const std::map<std::string, TokenType> Scanner::keywords = {
    {"and", TokenType::t_AND},
    //{"class",  TokenType::CLASS},
    //{"else",   TokenType::ELSE},
    //{"false",  TokenType::FALSE},
    //{"for",    TokenType::FOR},
    //{"fun",    TokenType::FUN},
    //{"if",     TokenType::IF},
    //{"nil",    TokenType::NIL},
    //{"or",     TokenType::OR},
    //{"print",  TokenType::PRINT},
    //{"return", TokenType::RETURN},
    //{"super",  TokenType::SUPER},
    //{"this",   TokenType::THIS},
    //{"true",   TokenType::TRUE},
    //{"var",    TokenType::VAR},
    //{"while",  TokenType::WHILE},
};
