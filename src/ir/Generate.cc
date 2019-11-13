#include "Generate.hh"
#include "IrFunction.hh"
#include "OperandType.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/CompareExpression.hh"
#include "ast/EqualityExpression.hh"
#include "ast/Expression.hh"
#include "ast/For.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/IndexExpression.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Literal.hh"
#include "ast/ReturnStatement.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
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
    function_decl->parameter_list()->for_each_parameter(
        [this](auto param) { gen_variable_decl(param); });

    gen_block(function_decl->body());
}

void Generate::gen_variable_decl(const VariableDecl *variable_decl) {
    auto var = env()->create_variable();
    env()->bind_variable(var, variable_decl->identifier().value);

    if (variable_decl->value()) {
        auto value = gen_expression(variable_decl->value());
        env()->add_quad(OPCode::MOVE, var, value, {});
    }
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

    auto else_label = env()->create_label();
    env()->add_quad(OPCode::JMP_NZ, else_label, condition, {});

    gen_block(if_statement->if_body());

    if (if_statement->else_body()) {
        auto end_label = env()->create_label();
        env()->add_quad(OPCode::JMP, end_label, {}, {});
        env()->queue_label(else_label);
        gen_block(if_statement->else_body());
        env()->queue_label(end_label);
    } else {
        env()->queue_label(else_label);
    }
}

void Generate::gen_while(const While *while_statement) {
    auto condition_label = env()->create_and_queue_label();
    auto condition = gen_expression(while_statement->condition());

    auto end_label = env()->create_label();
    env()->add_quad(OPCode::JMP_NZ, end_label, condition, {});

    gen_block(while_statement->body());

    env()->add_quad(OPCode::JMP, condition_label, {}, {});
    env()->queue_label(end_label);

    while_statement->body();
}

void Generate::gen_for(const For *for_statement) {
    gen_variable_decl(for_statement->initial_expression());

    auto condition_label = env()->create_and_queue_label();

    auto stop_condition = gen_expression(for_statement->stop_condition());

    auto end_label = env()->create_label();
    env()->add_quad(OPCode::JMP_NZ, end_label, stop_condition, {});

    gen_block(for_statement->body());
    gen_expression(for_statement->per_iteration_expression());
    env()->add_quad(OPCode::JMP, condition_label, {}, {});

    env()->queue_label(end_label);
}

void Generate::gen_return(const ReturnStatement *return_statement) {
    ASSERT_NOT_REACHED();
}

Operand Generate::gen_expression(const Expression *expression) {
    if (auto ptr = dynamic_cast<const BinaryExpression *>(expression)) {
        return gen_binary_expression(ptr);
    } else if (auto ptr = dynamic_cast<const FunctionCall *>(expression)) {
        return gen_function_call(ptr);
    } else if (auto ptr = dynamic_cast<const IndexExpression *>(expression)) {
        return gen_index_expression(ptr);
    } else if (auto ptr = dynamic_cast<const Literal *>(expression)) {
        return gen_literal(ptr);
    } else if (auto ptr = dynamic_cast<const UnaryExpression *>(expression)) {
        return gen_unary_expression(ptr);
    } else if (auto ptr =
                   dynamic_cast<const VariableExpression *>(expression)) {
        return gen_variable_expression(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_function_call(const FunctionCall *) {}

Operand Generate::gen_assign_expression(const AssignExpression *) {}
Operand Generate::gen_equality_expression(const EqualityExpression *) {}
Operand Generate::gen_compare_expression(const CompareExpression *) {}

Operand Generate::gen_binary_expression(const BinaryExpression *bin_expr) {
    if (auto ptr = dynamic_cast<const AssignExpression *>(bin_expr)) {
        return gen_assign_expression(ptr);
    } else if (auto ptr = dynamic_cast<const CompareExpression *>(bin_expr)) {
        return gen_compare_expression(ptr);
    } else if (auto ptr = dynamic_cast<const EqualityExpression *>(bin_expr)) {
        return gen_equality_expression(ptr);
    }

    auto right = gen_expression(bin_expr->right());
    auto left = gen_expression(bin_expr->left());

    auto var = m_ir_function->create_variable();

    switch (bin_expr->op()->symbol()) {
        case OperatorSym::PLUS:
            m_ir_function->add_quad(OPCode::ADD, var, left, right);
            break;

        case OperatorSym::MINUS:
            m_ir_function->add_quad(OPCode::SUB, var, left, right);
            break;

        case OperatorSym::MULT:
            m_ir_function->add_quad(OPCode::MULT, var, left, right);
            break;

        case OperatorSym::DIV:
            m_ir_function->add_quad(OPCode::DIV, var, left, right);
            break;

        // @TODO: Handle
        case OperatorSym::MODULO:
        case OperatorSym::INC:
        case OperatorSym::DEC:

        case OperatorSym::NOT_EQ:
        case OperatorSym::LTEQ:
        case OperatorSym::LT:
        case OperatorSym::GTEQ:
        case OperatorSym::GT:
        case OperatorSym::EQ:
        case OperatorSym::ASSIGN:
        default:
            ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_index_expression(const IndexExpression *) {}
Operand Generate::gen_literal(const Literal *) {}
Operand Generate::gen_unary_expression(const UnaryExpression *) {}
Operand Generate::gen_variable_expression(const VariableExpression *var_expr) {}

Operand Generate::create_integer_literal(
    const IntegerLiteral *integer_literal) {
    OperandType type { ValueKind::SIGNED, 4 };

    return IrFunction::create_immediate(integer_literal->value());
}