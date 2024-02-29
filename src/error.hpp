#pragma once

#include <iostream>
#include <string_view>

#include "token.hpp"

inline bool had_error = false;

static void report(int line, std::string_view where, std::string_view message) {
  std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
  had_error = true;
}

void error(const Token &token, std::string_view message) {  
  if (token.type == TokenType::t_EOF) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + token.lexeme + "'", message);
  }
}

void error(int line, std::string_view message) {
  report(line, "", message);
}
