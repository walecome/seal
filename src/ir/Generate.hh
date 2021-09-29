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
class FunctionDeclUser;
class FunctionDeclC;

class Generate {
    MAKE_NONMOVABLE(Generate)
    MAKE_NONCOPYABLE(Generate)

   public:
    Generate(StringTable* string_table, const CompilationUnit* compilation_unit)
        : m_string_table(string_table), m_compilation_unit { compilation_unit } {}

    QuadCollection generate();

   private:
    // Quads
    void gen_block(const Block*);
    void gen_statement(const Statement*);
    void gen_function_decl(const FunctionDecl*);
    void gen_user_function_decl(const FunctionDeclUser*);
    void gen_c_function_decl(const FunctionDeclC*);

    void gen_variable_decl(const VariableDecl*);
    void gen_if_statement(const IfStatement*);
    void gen_while(const While*);
    void gen_return(const ReturnStatement*);
    void gen_for(const For*);

    QuadSource gen_function_call(const FunctionCall*);
    QuadSource gen_expression(const Expression*);
    QuadSource gen_binary_expression(const BinaryExpression*);
    QuadSource gen_assign_expression(const AssignExpression*);
    QuadSource gen_equality_expression(const EqualityExpression*);
    QuadSource gen_compare_expression(const CompareExpression*);
    QuadSource gen_variable_expression(const VariableExpression*);
    QuadSource gen_index_expression(const IndexExpression*);
    QuadSource gen_unary_expression(const UnaryExpression*);

    auto env() const { return m_current_ir_function.get(); }

    // Literals
    ValueOperand create_literal(const Literal*);
    ValueOperand create_array_literal(const ArrayLiteral*);
    ValueOperand create_boolean_literal(const BooleanLiteral*);
    ValueOperand create_float_literal(const RealLiteral*);
    ValueOperand create_integer_literal(const IntegerLiteral*);
    ValueOperand create_string_literal(const StringLiteral*);
    
    Register create_register();
    Register get_return_register() const;
    Register previous_register() const;
    Register current_register() const;

    StringTable* m_string_table;
    const CompilationUnit* m_compilation_unit;
    ptr_t<IrFunction> m_current_ir_function;
    // Register 0 is reserved for function returns.
    unsigned m_register_count{1};
};
