#pragma once

#include <unordered_map>

#include "Constants.hh"
#include "Operand.hh"

// AST nodes
class IntegerLiteral;
class Block;
class Statement;
class Expression;
class VariableDecl;
class IfStatement;
class While;
class ReturnStatement;
class For;
class BinaryExpression;

class IrFunction;
class FunctionDecl;

class Generate {
    MAKE_NONMOVABLE(Generate)
    MAKE_NONCOPYABLE(Generate)

   public:
    Generate(const FunctionDecl* declaration);

    void generate();

   private:
    // Quads
    void gen_block(const Block*);
    void gen_statement(const Statement*);
    Operand gen_expression(const Expression*);
    void gen_function_decl(const FunctionDecl*);

    void gen_variable_decl(const VariableDecl*);
    void gen_if_statement(const IfStatement*);
    void gen_while(const While*);
    void gen_return(const ReturnStatement*);
    void gen_for(const For*);

    void gen_binary_expression(const BinaryExpression*);

    auto env() const { return m_ir_function.get(); }

    // Operands
    Operand create_integer_literal(const IntegerLiteral*);

    const FunctionDecl* m_declaration;
    ptr_t<IrFunction> m_ir_function;
};