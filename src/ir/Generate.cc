#include "Generate.hh"
#include "IrFunction.hh"
#include "OperandType.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/BooleanLiteral.hh"
#include "ast/CompareExpression.hh"
#include "ast/CompilationUnit.hh"
#include "ast/EqualityExpression.hh"
#include "ast/Expression.hh"
#include "ast/For.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/IndexExpression.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Literal.hh"
#include "ast/RealLiteral.hh"
#include "ast/ReturnStatement.hh"
#include "ast/StringLiteral.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
#include "ast/While.hh"
#include "builtin/BuiltIn.hh"
#include "ir/IrProgram.hh"

ptr_t<IrProgram> Generate::generate() {
    auto ir_program = std::make_unique<IrProgram>();

    m_compilation_unit->for_each_function([&](auto function_decl) {
        m_current_ir_function = std::make_unique<IrFunction>(function_decl);
        gen_function_decl(function_decl);
        ir_program->add_function(m_current_ir_function);
    });

    return ir_program;
}

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
    env()->bind_variable(var, variable_decl->identifier());

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
    auto value = gen_expression(return_statement->return_value());
    env()->add_quad(OPCode::RET, value, {}, {});
}

Operand Generate::gen_expression(const Expression *expression) {
    if (auto ptr = dynamic_cast<const BinaryExpression *>(expression)) {
        return gen_binary_expression(ptr);
    } else if (auto ptr = dynamic_cast<const FunctionCall *>(expression)) {
        return gen_function_call(ptr);
    } else if (auto ptr = dynamic_cast<const IndexExpression *>(expression)) {
        return gen_index_expression(ptr);
    } else if (auto ptr = dynamic_cast<const Literal *>(expression)) {
        return create_literal(ptr);
    } else if (auto ptr = dynamic_cast<const UnaryExpression *>(expression)) {
        return gen_unary_expression(ptr);
    } else if (auto ptr =
                   dynamic_cast<const VariableExpression *>(expression)) {
        return gen_variable_expression(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_function_call(const FunctionCall *func_call) {
    // TODO: Fix
    if (BuiltIn::is_builtin(func_call->identifier())) {
        throw std::runtime_error("Built-in function call not implemented");
    }

    func_call->argument_list()->for_each_argument([this](auto arg) {
        auto arg_operand = gen_expression(arg);
        env()->add_quad(OPCode::PUSH_ARG, arg_operand, {}, {});
    });

    // @TODO: Handle if there is not return value
    auto return_value = env()->create_variable();
    auto function_id = func_call->declaration()->function_id();

    env()->add_quad(OPCode::CALL, return_value,
                    env()->create_function_from_id(function_id), {});

    return return_value;
}

Operand Generate::gen_assign_expression(const AssignExpression *assign_expr) {
    ASSERT(assign_expr->op()->symbol() == OperatorSym::ASSIGN);

    auto var = dynamic_cast<VariableExpression *>(assign_expr->left());
    ASSERT(var != nullptr);

    // Order important as gen_variable_expression will generate a new id
    auto right = gen_expression(assign_expr->right());
    auto left = gen_variable_expression(var, true);

    env()->add_quad(OPCode::MOVE, left, right, {});

    // @FIXME: Return what?
    return left;
}

Operand Generate::gen_equality_expression(
    const EqualityExpression *equality_expr) {
    auto result = env()->create_variable();

    auto left = gen_expression(equality_expr->left());
    auto right = gen_expression(equality_expr->right());

    switch (equality_expr->op()->symbol()) {
        case OperatorSym::EQ:
            env()->add_quad(OPCode::CMP_EQ, result, left, right);
            break;

        case OperatorSym::NOT_EQ:
            env()->add_quad(OPCode::CMP_NOTEQ, result, left, right);
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return result;
}

Operand Generate::gen_compare_expression(
    const CompareExpression *compare_expr) {
    auto result = env()->create_variable();

    auto left = gen_expression(compare_expr->left());
    auto right = gen_expression(compare_expr->right());

    switch (compare_expr->op()->symbol()) {
        case OperatorSym::LT:
            env()->add_quad(OPCode::CMP_LT, result, left, right);
            break;

        case OperatorSym::LTEQ:
            env()->add_quad(OPCode::CMP_LTEQ, result, left, right);
            break;

        case OperatorSym::GT:
            env()->add_quad(OPCode::CMP_GT, result, left, right);
            break;

        case OperatorSym::GTEQ:
            env()->add_quad(OPCode::CMP_GTEQ, result, left, right);
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return result;
}

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

    auto var = env()->create_variable();

    switch (bin_expr->op()->symbol()) {
        case OperatorSym::PLUS:
            env()->add_quad(OPCode::ADD, var, left, right);
            break;

        case OperatorSym::MINUS:
            env()->add_quad(OPCode::SUB, var, left, right);
            break;

        case OperatorSym::MULT:
            env()->add_quad(OPCode::MULT, var, left, right);
            break;

        case OperatorSym::DIV:
            env()->add_quad(OPCode::DIV, var, left, right);
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

    return var;
}

Operand Generate::gen_index_expression(const IndexExpression *) {
    throw std::runtime_error(
        "IR generation of index expression not implemented");
}

Operand Generate::create_literal(const Literal *literal) {
    if (auto ptr = dynamic_cast<const ArrayLiteral *>(literal)) {
        return create_array_literal(ptr);
    } else if (auto ptr = dynamic_cast<const BooleanLiteral *>(literal)) {
        return create_boolean_literal(ptr);
    } else if (auto ptr = dynamic_cast<const RealLiteral *>(literal)) {
        return create_float_literal(ptr);
    } else if (auto ptr = dynamic_cast<const IntegerLiteral *>(literal)) {
        return create_integer_literal(ptr);
    } else if (auto ptr = dynamic_cast<const StringLiteral *>(literal)) {
        return create_string_literal(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_unary_expression(const UnaryExpression *unary_expr) {
    auto target_operand = gen_expression(unary_expr->expression());
    auto sym = unary_expr->op()->symbol();

    if (sym == OperatorSym::MINUS) {
        auto result_operand = env()->create_variable();
        env()->add_quad(OPCode::SUB, result_operand,
                        env()->create_immediate_int(0), target_operand);
        return result_operand;
    }

    auto var_expr =
        dynamic_cast<VariableExpression *>(unary_expr->expression());
    ASSERT(var_expr != nullptr);

    Operand result_operand;
    if (sym == OperatorSym::INC) {
        result_operand = gen_variable_expression(var_expr, true);
        env()->add_quad(OPCode::ADD, result_operand, target_operand,
                        env()->create_immediate_int(1));
    } else if (sym == OperatorSym::DEC) {
        result_operand = gen_variable_expression(var_expr, true);
        env()->add_quad(OPCode::SUB, result_operand, target_operand,
                        env()->create_immediate_int(1));
    } else {
        ASSERT_NOT_REACHED();
    }

    return result_operand;
}

Operand Generate::gen_variable_expression(const VariableExpression *var_expr,
                                          bool give_new_id) {
    if (!give_new_id) {
        return env()->get_variable(var_expr->identifier());
    }

    auto var = env()->create_variable();
    env()->bind_variable(var, var_expr->identifier());
    return var;
}

Operand Generate::create_integer_literal(
    const IntegerLiteral *integer_literal) {
    return env()->create_immediate_int(integer_literal->value());
}

Operand Generate::create_array_literal(const ArrayLiteral *) {
    throw std::runtime_error("IR generation of array literal not implemented");
}

Operand Generate::create_boolean_literal(
    const BooleanLiteral *boolean_literal) {
    // @TODO: Check if this is correct

    if (boolean_literal->value()) {
        return env()->create_immediate_int(1);
    } else {
        return env()->create_immediate_int(0);
    }
}

Operand Generate::create_float_literal(const RealLiteral *real_literal) {
    return env()->create_immediate_real(real_literal->value());
}

Operand Generate::create_string_literal(const StringLiteral *string_literal) {
    return env()->create_immediate_string(string_literal->value());
}
