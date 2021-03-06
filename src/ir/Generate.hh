#pragma once

#include <map>
#include <unordered_map>

#include "Constants.hh"
#include "Operand.hh"
#include "ir/IrFunction.hh"
#include "ir/Quad.hh"
#include "ir/QuadCollection.hh"

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
class FunctionCall;
class IndexExpression;
class Literal;
class UnaryExpression;
class VariableExpression;
class AssignExpression;
class EqualityExpression;
class CompareExpression;
class ArrayLiteral;
class BooleanLiteral;
class RealLiteral;
class StringLiteral;
class CompilationUnit;

class IrProgram;
class FunctionDecl;

class Generate {
    MAKE_NONMOVABLE(Generate)
    MAKE_NONCOPYABLE(Generate)

   public:
    Generate(const CompilationUnit* compilation_unit)
        : m_compilation_unit { compilation_unit } {}

    QuadCollection generate();

   private:
    // Quads
    void gen_block(const Block*);
    void gen_statement(const Statement*);
    void gen_function_decl(const FunctionDecl*);

    void gen_variable_decl(const VariableDecl*);
    void gen_if_statement(const IfStatement*);
    void gen_while(const While*);
    void gen_return(const ReturnStatement*);
    void gen_for(const For*);

    Operand gen_function_call(const FunctionCall*);
    Operand gen_expression(const Expression*);
    Operand gen_binary_expression(const BinaryExpression*);
    Operand gen_assign_expression(const AssignExpression*);
    Operand gen_equality_expression(const EqualityExpression*);
    Operand gen_compare_expression(const CompareExpression*);
    Operand gen_variable_expression(const VariableExpression*);
    Operand gen_index_expression(const IndexExpression*);
    Operand gen_unary_expression(const UnaryExpression*);

    auto env() const { return m_current_ir_function.get(); }

    // Literals
    Operand create_literal(const Literal*);
    Operand create_array_literal(const ArrayLiteral*);
    Operand create_boolean_literal(const BooleanLiteral*);
    Operand create_float_literal(const RealLiteral*);
    Operand create_integer_literal(const IntegerLiteral*);
    Operand create_string_literal(const StringLiteral*);

    const CompilationUnit* m_compilation_unit;
    ptr_t<IrFunction> m_current_ir_function;
};