#pragma once

#include <map>
#include <unordered_map>

#include "Constants.hh"
#include "ir/IrFunction.hh"
#include "ir/IrOperand.hh"
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
class RelocatedQuad;

class ConstantPool;

class Generate {
    MAKE_NONMOVABLE(Generate)
    MAKE_NONCOPYABLE(Generate)

   public:
    Generate(const CompilationUnit* compilation_unit,
             ConstantPool* constant_pool);

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

    IrOperand gen_function_call(const FunctionCall*);
    IrOperand gen_expression(const Expression*);
    IrOperand gen_binary_expression(const BinaryExpression*);
    IrOperand gen_assign_expression(const AssignExpression*);
    IrOperand gen_equality_expression(const EqualityExpression*);
    IrOperand gen_compare_expression(const CompareExpression*);
    IrOperand gen_variable_expression(const VariableExpression*);
    IrOperand gen_index_expression(const IndexExpression*);
    IrOperand gen_unary_expression(const UnaryExpression*);

    std::vector<RelocatedQuad> relocate_quads(const std::vector<Quad>& quads);
    RelocatedQuad relocate_quad(const Quad& quad);

    IrFunction& env() const {
        return *m_current_ir_function;
    }

    Register return_register() const;

    // Literals
    IrOperand create_literal(const Literal*);
    IrOperand create_array_literal(const ArrayLiteral*);
    IrOperand create_boolean_literal(const BooleanLiteral*);
    IrOperand create_float_literal(const RealLiteral*);
    IrOperand create_integer_literal(const IntegerLiteral*);
    IrOperand create_string_literal(const StringLiteral*);

    ConstantPool& get_constant_pool();

    const CompilationUnit* m_compilation_unit;
    ConstantPool* m_constant_pool;
    ptr_t<IrFunction> m_current_ir_function;
    // Register 0 is reserved for function returns.
    unsigned m_register_count { 0 };
    QuadCollection m_quad_collection {};
};
