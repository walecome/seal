#include "Generate.hh"
#include "IrFunction.hh"
#include "OperandType.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"
#include "ast/For.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/ReturnStatement.hh"
#include "ast/VariableDecl.hh"
#include "ast/While.hh"

Generate::Generate(const FunctionDecl *declaration)
    : m_declaration { declaration },
      m_ir_function { std::make_unique<IrFunction>() } {}

void Generate::generate() { gen_function_decl(m_declaration); }

void Generate::gen_block(const Block *block) {
    block->for_each_node([this](Node *node) {
        if (auto x = dynamic_cast<Statement *>(node))
            gen_statement(x);

        else if (auto x = dynamic_cast<FunctionDecl *>(node))
            gen_function_decl(x);

        else
            ASSERT_NOT_REACHED();
    });
}

void Generate::gen_function_decl(const FunctionDecl *function_decl) {
    gen_block(function_decl->body());
}

void Generate::gen_variable_decl(const VariableDecl *variable_decl) {
    auto value = gen_expression(variable_decl->value());

    // auto var = Operand::create_variable(
    //     // @FIXME: What is the Type?
    //     { ValueKind::PTR, 0 },
    //     std::string(variable_decl->identifier().value));

    // Quad quad { OPCode::MOVE, std::move(var), std::move(value), {} };

    ASSERT_NOT_REACHED();
}

void Generate::gen_statement(const Statement *statement) {
    if (auto ptr = dynamic_cast<const Block *>(statement)) {
        gen_block(ptr);
    } else if (auto ptr = dynamic_cast<const VariableDecl *>(statement)) {
        gen_variable_decl(ptr);
    } else if (auto ptr = dynamic_cast<const Expression *>(statement)) {
        gen_expression(ptr);
    } else if (auto ptr = dynamic_cast<const IfStatement *>(statement)) {
        gen_if_statement(ptr);
    } else if (auto ptr = dynamic_cast<const While *>(statement)) {
        gen_while(ptr);
    } else if (auto ptr = dynamic_cast<const ReturnStatement *>(statement)) {
        gen_return(ptr);
    } else if (auto ptr = dynamic_cast<const For *>(statement)) {
        gen_for(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

void Generate::gen_if_statement(const IfStatement *if_statement) {
    auto condition = gen_expression(if_statement->condition());

    ASSERT_NOT_REACHED();
}
void Generate::gen_while(const While *while_statement) { ASSERT_NOT_REACHED(); }
void Generate::gen_return(const ReturnStatement *return_statement) {
    ASSERT_NOT_REACHED();
}
void Generate::gen_for(const For *for_statement) { ASSERT_NOT_REACHED(); }

Operand Generate::gen_expression(const Expression *expression) {
    ASSERT_NOT_REACHED();
}

void Generate::gen_binary_expression(const BinaryExpression *bin_expr) {
    auto right = gen_expression(bin_expr->right());
    auto left = gen_expression(bin_expr->left());

    auto var = m_ir_function->create_variable();

    switch (bin_expr->op()->symbol()) {
        case OperatorSym::ASSIGN:
            m_ir_function->emplace_quad(OPCode::MOVE, left, right, {});
            break;

        case OperatorSym::PLUS:
            m_ir_function->emplace_quad(OPCode::ADD, var, left, right);
            break;

        case OperatorSym::MINUS:
            m_ir_function->emplace_quad(OPCode::SUB, var, left, right);
            break;

        case OperatorSym::MULT:
            m_ir_function->emplace_quad(OPCode::MULT, var, left, right);
            break;

        case OperatorSym::DIV:
            m_ir_function->emplace_quad(OPCode::DIV, var, left, right);
            break;

        case OperatorSym::EQ:
            m_ir_function->emplace_quad(OPCode::CMP_EQ, var, left, right);
            break;

        case OperatorSym::GT:
            m_ir_function->emplace_quad(OPCode::CMP_GT, var, left, right);
            break;

        case OperatorSym::GTEQ:
            m_ir_function->emplace_quad(OPCode::CMP_GTEQ, var, left, right);
            break;

        case OperatorSym::LT:
            m_ir_function->emplace_quad(OPCode::CMP_LT, var, left, right);
            break;

        case OperatorSym::LTEQ:
            m_ir_function->emplace_quad(OPCode::CMP_LTEQ, var, left, right);
            break;

        case OperatorSym::NOT_EQ:
            m_ir_function->emplace_quad(OPCode::CMP_NOTEQ, var, left, right);
            break;

        case OperatorSym::MODULO:
        case OperatorSym::INC:
        case OperatorSym::DEC:
        default:
            ASSERT_NOT_REACHED();
    }
}

Operand Generate::create_integer_literal(
    const IntegerLiteral *integer_literal) {
    OperandType type { ValueKind::SIGNED, 4 };

    return IrFunction::create_immediate(integer_literal->value());
}