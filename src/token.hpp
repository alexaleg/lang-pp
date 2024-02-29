#pragma once

#include <any>
#include <string>
#include <utility>

#include "tokenType.hpp"

using std::any;
using std::string;

class Token {
    public:
    const TokenType type;
    const string lexeme;
    const any literal;
    const int line;
    
    Token(TokenType type, string lexeme, any literal, int line):
      type{type}, lexeme{std::move(lexeme)}, 
      literal{std::move(literal)}, line{line} 
    {}

    bool operator==(const Token &other)  {
      return type == other.type && lexeme == other.lexeme ;
    }

    string toString(void) const{
      string literal_text;
      switch (type) {
        case t_NUMBER:
          literal_text = std::to_string(std::any_cast<double>(literal));
          break;
        case t_IDENTIFIER:
          literal_text = lexeme;
          break;
        //case t_PLUS:
          //literal_text = std::any_cast<string>(literal);
          //break;
        default:
          literal_text = "None";
          break;
      }
      return std::to_string(type) + " " + lexeme + " " + literal_text;
    }

};


