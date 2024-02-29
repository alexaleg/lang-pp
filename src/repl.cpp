#include <iostream>
#include <string>

#include "AstPrinter.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "repl.hpp"
#include "scanner.hpp"
#include "token.hpp"

void main_loop() {
  string buffer;
  std::cout << "Write 'exit' to exit"
            << "\n";
  while (true) {
    std::cout << "> ";
    getline(std::cin, buffer);

    if (buffer == "exit") {
      std::cout << "Exiting REPL"
                << "\n";
      break;
    }
    // std::cout << "Got: " << buffer << "\n";
    std::shared_ptr<Expr> expr;
      Scanner scanner{buffer};
      std::vector<Token> tokens = scanner.scanTokens();
      // for (const Token token : tokens) {
      // std::cout << token.toString() << "\n";
      //}
      Parser parser{tokens};
      expr = parser.parse();

      if (had_error) {
        std::cout << "Error parsing input" << "\n";
        had_error = false;
        continue;
      }
      else {
        std::cout << AstPrinter{}.print(expr) << "\n";
      }
  }
}

