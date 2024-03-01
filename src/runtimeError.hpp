#pragma once
#include <stdexcept>
#include <string_view>

#include "token.hpp"

class RuntimeError: public std::runtime_error {
  public:
    const Token& token;

    RuntimeError(const Token& token, std::string_view message) 
      : std::runtime_error(message.data()), token{token} {
      }
};
