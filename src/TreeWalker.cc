#include "Interpreter.hh"

void Interpreter::interpret_argument_list(ArgumentList* arg_list) {
    arg_list->for_each_argument([this](auto arg) { interpret_expr(arg); });
}

// @TODO
void Interpreter::interpret_array_literal(ArrayLiteral* array_literal) {
    ASSERT_NOT_REACHED();
    array_literal->for_each_element(
        [this](auto element) { interpret_expr(element); });
}

expr_value_t Interpreter::interpret_assign_expr(AssignExpression* expr) {
    expr_value_t value = interpret_expr(expr->right());
    auto identifier =
        dynamic_cast<VariableExpression*>(expr->left())->identifier().value;
    current_env()->set_variable(identifier, value, false);

    return value;
}

template <class T>
T bin_op(T left, T right, OperatorSym op) {
    switch (op) {
        case OperatorSym::PLUS:
            return left + right;
        case OperatorSym::MINUS:
            return left - right;
        case OperatorSym::MULT:
            return left * right;
        case OperatorSym::DIV:
            return left / right;
        default:
            ASSERT_NOT_REACHED();
    }

    ASSERT_NOT_REACHED();
}

std::string bin_op(const std::string& left, const std::string& right,
                   OperatorSym op) {
    switch (op) {
        case OperatorSym::PLUS:
            return left + right;
        case OperatorSym::MINUS:
        case OperatorSym::MULT:
        case OperatorSym::DIV:
        default:
            ASSERT_NOT_REACHED();
    }

    ASSERT_NOT_REACHED();
}

// Operators other than +, -, * and / should be matched by the other functions
expr_value_t Interpreter::interpret_binary_expr(BinaryExpression* expr) {
    expr_value_t left_value = interpret_expr(expr->left());
    expr_value_t right_value = interpret_expr(expr->right());

    ASSERT(right_value.index() == left_value.index());

    if (std::holds_alternative<int>(right_value)) {
        return bin_op(std::get<int>(left_value), std::get<int>(right_value),
                      expr->op()->symbol());
    }
    if (std::holds_alternative<float>(right_value)) {
        return bin_op(std::get<float>(left_value), std::get<float>(right_value),
                      expr->op()->symbol());
    }
    if (std::holds_alternative<std::string>(right_value)) {
        return bin_op(std::get<std::string>(left_value),
                      std::get<std::string>(right_value), expr->op()->symbol());
    }
    if (std::holds_alternative<bool>(right_value)) {
        return bin_op(std::get<bool>(left_value), std::get<bool>(right_value),
                      expr->op()->symbol());
    }

    ASSERT_NOT_REACHED();
}

void Interpreter::interpret_block(Block*) {
    ENV_GUARD()
    // for (auto& node : block->nodes()) {
    //     // @FIXME
    //     ASSERT_NOT_REACHED();
    // }
}

expr_value_t Interpreter::interpret_compare_expr(CompareExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right());
    expr_value_t left_value = interpret_expr(expr->left());

    switch (expr->op()->symbol()) {
        case OperatorSym::GT:
            return left_value > right_value;
            break;
        case OperatorSym::GTEQ:
            return left_value >= right_value;
            break;
        case OperatorSym::LT:
            return left_value < right_value;
            break;
        case OperatorSym::LTEQ:
            return left_value <= right_value;
            break;
        default:
            ASSERT_NOT_REACHED();
    }
}

void Interpreter::interpret_compilation_unit(CompilationUnit* unit) {
    FunctionDecl* main { nullptr };

    for (auto& function : unit->functions()) {
        if (function->identifier().value == "main") {
            main = function.get();
            break;
        }
    }
    try {
        interpret_block(main->body());
        std::cout << "Function main finished without a status" << std::endl;
    } catch (const ReturnException& ret) {
        std::cout << "Function main finished with status: "
                  << std::get<int>(ret.value) << std::endl;
    }

    ASSERT(environments.empty());
}

expr_value_t Interpreter::interpret_expr(Expression* expr) {
    if (auto ptr = dynamic_cast<AssignExpression*>(expr)) {
        return interpret_assign_expr(ptr);
    } else if (auto ptr = dynamic_cast<UnaryExpression*>(expr)) {
        return interpret_unary_expr(ptr);
    } else if (auto ptr = dynamic_cast<CompareExpression*>(expr)) {
        return interpret_compare_expr(ptr);
    } else if (auto ptr = dynamic_cast<AssignExpression*>(expr)) {
        return interpret_assign_expr(ptr);
    } else if (auto ptr = dynamic_cast<EqualityExpression*>(expr)) {
        return interpret_equality_expr(ptr);
    } else if (auto ptr = dynamic_cast<BinaryExpression*>(expr)) {
        return interpret_binary_expr(ptr);
    } else if (auto ptr = dynamic_cast<VariableExpression*>(expr)) {
        return interpret_variable_expr(ptr);
    } else if (auto ptr = dynamic_cast<FunctionCall*>(expr)) {
        return interpret_function_call(ptr);
    } else if (auto ptr = dynamic_cast<Literal*>(expr)) {
        return interpret_literal(ptr);
    }

    ASSERT_NOT_REACHED();
}

expr_value_t Interpreter::interpret_equality_expr(EqualityExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right());
    expr_value_t left_value = interpret_expr(expr->left());

    if (expr->op()->symbol() == OperatorSym::EQ) {
        return right_value == left_value;
    } else if (expr->op()->symbol() == OperatorSym::NOT_EQ) {
        return right_value != left_value;
    }

    ASSERT_NOT_REACHED();
}

template <class T>
void print(T t, bool is_bool = false) {
    if (is_bool) {
        std::cout << std::boolalpha;
    }

    std::cout << t << std::endl;
}

void Interpreter::handle_print(Expression* expr) {
    expr_value_t value = interpret_expr(expr);

    switch (expr->type().primitive()) {
        case Primitive::FLOAT:
            print(std::get<float>(value));
            break;
        case Primitive::INT:
            print(std::get<int>(value));
            break;
        case Primitive::STRING:
            print(std::get<std::string>(value));
            break;
        case Primitive::BOOL:
            print(std::get<bool>(value), true);
            break;
        case Primitive::VOID:
        default:
            ASSERT_NOT_REACHED();
    }
}

std::vector<expr_value_t> Interpreter::prepare_expression_values(
    const std::vector<ptr_t<Expression>>& expressions) {
    std::vector<expr_value_t> values {};

    for (auto& x : expressions) {
        values.push_back(interpret_expr(x.get()));
    }

    return values;
}

expr_value_t Interpreter::interpret_function_call(FunctionCall* function_call) {
    ENV_GUARD()

    auto ident = function_call->identifier().value;

    if (BuiltIn::is_builtin(ident)) {
        BuiltIn::dispatch_builtin_function(
            ident, prepare_expression_values(
                       function_call->argument_list()->arguments()));
        return 0;
    }

    auto func = function_scope->get_function(function_call->identifier().value);
    auto arg_list = function_call->argument_list();

    for (unsigned i = 0; i < arg_list->nr_args(); ++i) {
        expr_value_t value = interpret_expr(arg_list->argument_at(i));
        std::string_view ident =
            func->parameter_list()->parameter_at(i)->identifier().value;
        current_env()->set_variable(ident, value, true);
    }

    expr_value_t value;

    try {
        interpret_block(func->body());
        if (func->type().primitive() != Primitive::VOID) {
            ASSERT_NOT_REACHED();
        }
    } catch (const ReturnException& ret) {
        value = ret.value;
    }

    return value;
}

bool is_truthy(expr_value_t value) {
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    if (std::holds_alternative<std::string>(value))
        return std::get<std::string>(value) == "";
    if (std::holds_alternative<int>(value)) return std::get<int>(value);
    if (std::holds_alternative<float>(value)) return std::get<float>(value);

    ASSERT_NOT_REACHED();
}

void Interpreter::interpret_if_statement(IfStatement* if_statement) {
    expr_value_t condition_value = interpret_expr(if_statement->condition());

    if (is_truthy(condition_value)) {
        interpret_block(if_statement->if_body());
    } else if (if_statement->else_body()) {
        interpret_block(if_statement->else_body());
    }
}

expr_value_t Interpreter::interpret_literal(Literal* literal) {
    if (auto ptr = dynamic_cast<BooleanLiteral*>(literal)) {
        return ptr->value();
    }
    if (auto ptr = dynamic_cast<StringLiteral*>(literal)) {
        return std::string(ptr->value());
    }
    if (auto ptr = dynamic_cast<IntegerLiteral*>(literal)) {
        return ptr->value();
    }
    if (auto ptr = dynamic_cast<RealLiteral*>(literal)) {
        return ptr->value();
    }
    if (auto ptr = dynamic_cast<ArrayLiteral*>(literal)) {
        throw 1;
    }

    ASSERT_NOT_REACHED();
}

void Interpreter::interpret_return(ReturnStatement* ret) {
    // Unwind stack to caller
    throw ReturnException(interpret_expr(ret->return_value()));
}

void Interpreter::interpret_statement(Statement* statement) {
    if (auto ptr = dynamic_cast<Block*>(statement)) {
        interpret_block(ptr);
    } else if (auto ptr = dynamic_cast<VariableDecl*>(statement)) {
        interpret_variable_decl(ptr);
    } else if (auto ptr = dynamic_cast<Expression*>(statement)) {
        interpret_expr(ptr);
    } else if (auto ptr = dynamic_cast<IfStatement*>(statement)) {
        interpret_if_statement(ptr);
    } else if (auto ptr = dynamic_cast<While*>(statement)) {
        interpret_while(ptr);
    } else if (auto ptr = dynamic_cast<ReturnStatement*>(statement)) {
        interpret_return(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

void Interpreter::interpret_type(Type*) { ASSERT_NOT_REACHED(); }

expr_value_t Interpreter::interpret_unary_expr(UnaryExpression* expr) {
    expr_value_t value = interpret_expr(expr->expression());

    if (expr->op()->symbol() == OperatorSym::MINUS) {
        if (std::holds_alternative<int>(value)) {
            return -std::get<int>(value);
        } else if (std::holds_alternative<float>(value)) {
            return -std::get<float>(value);
        } else {
            ASSERT_NOT_REACHED();
        }
    }

    ASSERT_NOT_REACHED();
}

void Interpreter::interpret_variable_decl(VariableDecl* var_decl) {
    current_env()->set_variable(var_decl->identifier().value,
                                interpret_expr(var_decl->value()), true);
}

expr_value_t Interpreter::interpret_variable_expr(VariableExpression* expr) {
    return current_env()->get_variable(expr->identifier().value);
}

void Interpreter::interpret_while(While* while_statement) {
    expr_value_t condition_value = interpret_expr(while_statement->condition());

    while (is_truthy(condition_value)) {
        interpret_block(while_statement->body());
        condition_value = interpret_expr(while_statement->condition());
    }
}

void Interpreter::enter_block() {
    auto new_env = std::make_unique<Environment>(current_env());
    environments.push(std::move(new_env));
}

void Interpreter::exit_block() { environments.pop(); }

Environment* Interpreter::current_env() {
    if (environments.empty()) return nullptr;
    return environments.top().get();
}

EnvGuard Interpreter::acquire_block() { return EnvGuard(this); }

void Environment::set_variable(std::string_view ident, expr_value_t data,
                               bool force_local) {
    if (force_local) {
        m_variables[ident] = data;
    } else {
        Environment* variable_env = env_of_variable(ident);
        if (!variable_env) variable_env = this;
        variable_env->set_variable(ident, data, true);
    }
}

expr_value_t Environment::get_variable(std::string_view ident) const {
    auto it = m_variables.find(ident);
    if (it != std::end(m_variables)) {
        return it->second;
    } else if (m_parent) {
        return m_parent->get_variable(ident);
    }

    // This should not be able to happen as the typechecker should find
    // these instances and terminate.
    ASSERT_NOT_REACHED();
}

Environment* Environment::env_of_variable(std::string_view ident) const {
    auto it = m_variables.find(ident);

    if (it != std::end(m_variables)) {
        return const_cast<Environment*>(this);
    }

    if (m_parent) {
        return m_parent->env_of_variable(ident);
    }

    return nullptr;
}
