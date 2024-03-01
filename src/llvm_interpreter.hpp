#pragma once
#include <any>
#include <iostream>

#include "Expr.hpp"
#include "runtimeError.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

class Interpreter_LLVM: public ExprVisitor {
private:
  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::Module> Module;
  std::unique_ptr<llvm::IRBuilder<>> Builder;

  std::any evaluate(std::shared_ptr<Expr> expr) { return (expr->accept(*this)); }

  void initialize() {
    Context = std::make_unique<llvm::LLVMContext>();
    Module = std::make_unique<llvm::Module>("my cool jit", *Context);
    Builder = std::make_unique<llvm::IRBuilder<>>(*Context);
  }

public:
  Interpreter_LLVM() { initialize(); }

  void interpret(std::shared_ptr<Expr> expr) {
    try {
      std::any value = evaluate(expr);
      //std::cout << std::any_cast<t(value) << "\n";
    } catch (RuntimeError error) {
      std::cout << "Error: " << error.what() << std::endl;
    }
  }

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override {
    return llvm::ConstantFP::get(*Context, llvm::APFloat(std::any_cast<double>(expr->value)));
  }

  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override {
    llvm::Value *left = std::any_cast<llvm::Value*>( evaluate(expr->left));
    llvm::Value *right = std::any_cast<llvm::Value*>( evaluate(expr->right));
    switch (expr->op.type) {
    case TokenType::t_PLUS:
      return Builder->CreateFAdd(left, right, "addtmp");
    default:
      RuntimeError(expr->op, "Unexpected binary operator.");
    }
    return {};
  }
  void print_IR() {
    if (llvm::verifyModule(*Module)) {
      std::cout << "IR verification failed." << std::endl;
    } else {
      Module->print(llvm::outs(), nullptr);
    }
  }


private:
  void checkNumberOperand(Token op, std::any &operand) {
    if (operand.type() != typeid(double)) {
      throw RuntimeError(op, "Operand must be a number.");
    }
  }
};
