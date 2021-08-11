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

#include "fmt/format.h"

QuadCollection Generate::generate() {
    auto ir_program = std::make_unique<IrProgram>();

    QuadCollection ret {};

    m_compilation_unit->for_each_function([&](auto function_decl) {
        m_current_ir_function = std::make_unique<IrFunction>(function_decl);
        gen_function_decl(function_decl);

        ret.function_to_quad.insert(
            { m_current_ir_function->declaration()->function_id(),
              ret.quads.size() });
        for (const Quad &quad : m_current_ir_function->quads()) {
            for (unsigned label_id : quad.label_ids()) {
                ret.label_to_quad.insert({ label_id, ret.quads.size() });
            }
            ret.quads.push_back(quad);
        }

        ir_program->add_function(m_current_ir_function);
    });

    ret.main_function_id = ir_program->main_function_id();

    return ret;
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

    if (function_decl->type().is_void()) {
        env()->add_quad(OPCode::RET, {}, {}, {});
    }
}

void Generate::gen_variable_decl(const VariableDecl *variable_decl) {
    if (variable_decl->value()) {
        auto value = gen_expression(variable_decl->value());
        auto dest = QuadDest {env()->create_variable(variable_decl->identifier(), [this] {
            return create_register();
        })};
        env()->add_quad(OPCode::MOVE, dest, value, {});
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

    auto else_label = QuadDest {env()->create_label() };
    env()->add_quad(OPCode::JMP_Z, else_label, condition, {});

    gen_block(if_statement->if_body());

    if (if_statement->else_body()) {
        auto end_label = QuadDest{ env()->create_label()};
        env()->add_quad(OPCode::JMP, end_label, {}, {});
        env()->queue_label(else_label.as_label());
        gen_block(if_statement->else_body());
        env()->queue_label(end_label.as_label());
    } else {
        env()->queue_label(else_label.as_label());
    }
}

void Generate::gen_while(const While *while_statement) {
    auto condition_label = QuadDest{env()->create_and_queue_label()};
    auto condition = gen_expression(while_statement->condition());

    auto end_label = QuadDest{env()->create_label()};
    env()->add_quad(OPCode::JMP_Z, end_label, condition, {});

    gen_block(while_statement->body());

    env()->add_quad(OPCode::JMP, condition_label, {}, {});
    env()->queue_label(end_label.as_label());

    while_statement->body();
}

void Generate::gen_for(const For *for_statement) {
    gen_variable_decl(for_statement->initial_expression());

    auto condition_label = env()->create_and_queue_label();

    auto stop_condition = gen_expression(for_statement->stop_condition());

    auto end_label = env()->create_label();
    env()->add_quad(OPCode::JMP_Z, end_label, stop_condition, {});

    gen_block(for_statement->body());
    gen_expression(for_statement->per_iteration_expression());
    env()->add_quad(OPCode::JMP, condition_label, {}, {});

    env()->queue_label(end_label);
}

void Generate::gen_return(const ReturnStatement *return_statement) {
    auto value = gen_expression(return_statement->return_value());
    env()->add_quad(OPCode::RET, value, {}, {});
}

QuadSource Generate::gen_expression(const Expression *expression) {
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

QuadSource Generate::gen_function_call(const FunctionCall *func_call) {
    bool is_builtin = BuiltIn::is_builtin(func_call->identifier());

    // Push arguments
    func_call->argument_list()->for_each_enumerated_argument(
        [&](auto arg, unsigned index) {
            auto arg_operand = gen_expression(arg);

            if (is_builtin) {
                // TODO: This is an ugly fix for builtins...
                env()->add_quad(OPCode::PUSH_ARG, env()->create_tmp_variable(),
                                arg_operand, {});
            } else {
                std::string_view parameter_name = func_call->declaration()
                                                      ->parameter_list()
                                                      ->parameter_at(index)
                                                      ->identifier();
                env()->add_quad(OPCode::PUSH_ARG,
                                env()->create_variable(parameter_name),
                                arg_operand, {});
            }
        });

    auto function_id =
        is_builtin
            ? BuiltIn::function_id_from_identifier(func_call->identifier())
            : func_call->declaration()->function_id();

    FunctionOperand func = env()->create_function_from_id(function_id);

    auto return_register = create_register();

    env()->add_quad(OPCode::CALL, return_register, QuadSource { func }, {});

    return QuadSource { return_register };
}

QuadSource Generate::gen_assign_expression(
    const AssignExpression *assign_expr) {
    ASSERT(assign_expr->op()->symbol() == OperatorSym::ASSIGN);

    QuadSource right = gen_expression(assign_expr->right());

    VariableExpression *var =
        dynamic_cast<VariableExpression *>(assign_expr->left());
    if (var) {
        QuadSource left = gen_variable_expression(var);
        env()->add_quad(OPCode::MOVE, left.as_register(), right, {});
        return QuadSource { left };
    }

    // TODO: Not sure if this should be done here or somewhere else. The
    // IndexExpression in not evaluated not, which is kind of counter
    // intuitive...
    // IndexExpression *index_expr =
    //     dynamic_cast<IndexExpression *>(assign_expr->left());

    // if (index_expr) {
    //     Operand indexed = gen_expression(index_expr->indexed_expression());
    //     Operand index = gen_expression(index_expr->index());
    //     env()->add_quad(OPCode::INDEX_ASSIGN, indexed, index, right);
    //     return right;
    // }

    ASSERT_NOT_REACHED();
}

QuadSource Generate::gen_equality_expression(
    const EqualityExpression *equality_expr) {
    auto result = create_register();

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

    return QuadSource { result };
}

QuadSource Generate::gen_compare_expression(
    const CompareExpression *compare_expr) {
    auto result = create_register();

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

    return QuadSource { result };
}

QuadSource Generate::gen_binary_expression(const BinaryExpression *bin_expr) {
    if (auto ptr = dynamic_cast<const AssignExpression *>(bin_expr)) {
        return gen_assign_expression(ptr);
    } else if (auto ptr = dynamic_cast<const CompareExpression *>(bin_expr)) {
        return gen_compare_expression(ptr);
    } else if (auto ptr = dynamic_cast<const EqualityExpression *>(bin_expr)) {
        return gen_equality_expression(ptr);
    }

    auto right = gen_expression(bin_expr->right());
    auto left = gen_expression(bin_expr->left());

    auto result = create_register();

    switch (bin_expr->op()->symbol()) {
        case OperatorSym::PLUS:
            env()->add_quad(OPCode::ADD, result, left, right);
            break;

        case OperatorSym::MINUS:
            env()->add_quad(OPCode::SUB, result, left, right);
            break;

        case OperatorSym::MULT:
            env()->add_quad(OPCode::MULT, result, left, right);
            break;

        case OperatorSym::DIV:
            env()->add_quad(OPCode::DIV, result, left, right);
            break;

        case OperatorSym::AND:
            env()->add_quad(OPCode::AND, result, left, right);
            break;

        case OperatorSym::OR:
            env()->add_quad(OPCode::OR, result, left, right);
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

    return QuadSource { result };
}

QuadSource Generate::gen_index_expression(const IndexExpression *expr) {
    // Operand indexed = gen_expression(expr->indexed_expression());
    // Operand index = gen_expression(expr->index());
    // Operand result = env()->create_tmp_variable();

    // env()->add_quad(OPCode::INDEX_MOVE, result, indexed, index);

    // return result;
    // TODO: Implement
    ASSERT_NOT_REACHED();
    return QuadSource { create_register() };
}

ValueOperand Generate::create_literal(const Literal *literal) {
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

QuadSource Generate::gen_unary_expression(const UnaryExpression *unary_expr) {
    auto target_operand = gen_expression(unary_expr->expression());
    auto sym = unary_expr->op()->symbol();

    if (sym == OperatorSym::MINUS) {
        auto result = create_register();
        auto zero = QuadSource { env()->create_immediate_int(0) };
        env()->add_quad(OPCode::SUB, result, zero, target_operand);
        return QuadSource { result };
    }

    VariableExpression *var_expr =
        dynamic_cast<VariableExpression *>(unary_expr->expression());

    Register result = var_expr ? gen_variable_expression(var_expr).as_register()
                               : create_register();

    auto one = QuadSource { env()->create_immediate_int(1) };

    if (sym == OperatorSym::INC) {
        env()->add_quad(OPCode::ADD, result, target_operand, one);
    } else if (sym == OperatorSym::DEC) {
        env()->add_quad(OPCode::SUB, result, target_operand, one);
    } else {
        ASSERT_NOT_REACHED();
    }

    return QuadSource { result };
}

QuadSource Generate::gen_variable_expression(
    const VariableExpression *var_expr) {
    return QuadSource { env()->create_variable(
        var_expr->identifier(), [this] { return this->create_register(); }) };
}

ValueOperand Generate::create_integer_literal(
    const IntegerLiteral *integer_literal) {
    return env()->create_immediate_int(integer_literal->value());
}

ValueOperand Generate::create_array_literal(const ArrayLiteral *array) {
    ValueOperand vector_operand = env()->create_vector_operand();

    array->for_each_element([&](Expression *element) {
        ValueOperand element_operand = gen_expression(element).as_value();
        vector_operand.as_vector()->push_back(element_operand);
    });

    return vector_operand;
}

ValueOperand Generate::create_boolean_literal(
    const BooleanLiteral *boolean_literal) {
    // @TODO: Check if this is correct

    if (boolean_literal->value()) {
        return env()->create_immediate_int(1);
    } else {
        return env()->create_immediate_int(0);
    }
}

ValueOperand Generate::create_float_literal(const RealLiteral *real_literal) {
    return env()->create_immediate_real(real_literal->value());
}

ValueOperand Generate::create_string_literal(const StringLiteral *string_literal) {
    return env()->create_immediate_string(string_literal->value());
}

Register Generate::create_register() const {
    static unsigned register_count = 0;
    return Register(register_count++);
}
