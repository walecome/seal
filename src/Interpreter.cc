#include "Interpreter.hh"

void Interpreter::interpret_argument_list(ArgumentList* arg_list) {
    for (auto& argument : arg_list->arguments) {
        interpret_expr(argument.get());
    }
}

// @TODO
void Interpreter::interpret_array_literal(ArrayLiteral* array_literal) {
    assert(false);
    for (auto& expr : array_literal->expressions) {
        interpret_expr(expr.get());
    }
}

expr_value_t Interpreter::interpret_assign_expr(AssignExpression* expr) {
    expr_value_t value = interpret_expr(expr->right.get());
    auto identifier =
        dynamic_cast<VariableExpression*>(expr->left.get())->identifier.value;
    current_env()->set_variable(identifier, value);

    return value;
}

template <class T>
T bin_op(T left, T right, OperatorSym op) {
    static_assert(!std::is_same<T, std::string>::value,
                  "General binary operation cannot be perfomed on strings");
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
            assert(false);
    }

    assert(false);
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
            assert(false);
    }

    assert(false);
}

// Operators other than +, -, * and / should be matched by the other functions
expr_value_t Interpreter::interpret_binary_expr(BinaryExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right.get());
    expr_value_t left_value = interpret_expr(expr->left.get());

    assert(right_value.index() == left_value.index());

    if (std::holds_alternative<int>(right_value)) {
        return bin_op(std::get<int>(left_value), std::get<int>(right_value),
                      expr->op->operator_symbol);
    }
    if (std::holds_alternative<float>(right_value)) {
        return bin_op(std::get<float>(left_value), std::get<float>(right_value),
                      expr->op->operator_symbol);
    }
    if (std::holds_alternative<std::string>(right_value)) {
        return bin_op(std::get<std::string>(left_value),
                      std::get<std::string>(right_value),
                      expr->op->operator_symbol);
    }
    if (std::holds_alternative<bool>(right_value)) {
        return bin_op(std::get<bool>(left_value), std::get<bool>(right_value),
                      expr->op->operator_symbol);
    }

    assert(false);
}

void Interpreter::interpret_block(Block* block) {
    enter_block();
    for (auto& statement : block->statements) {
        interpret_statement(statement.get());
    }
    exit_block();
}

expr_value_t Interpreter::interpret_compare_expr(CompareExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right.get());
    expr_value_t left_value = interpret_expr(expr->left.get());

    switch (expr->op->operator_symbol) {
        case OperatorSym::GT:
            return right_value > left_value;
            break;
        case OperatorSym::GTEQ:
            return right_value >= left_value;
            break;
        case OperatorSym::LT:
            return right_value < left_value;
            break;
        case OperatorSym::LTEQ:
            return right_value <= left_value;
            break;
        default:
            assert(false);
    }
}

void Interpreter::interpret_compilation_unit(CompilationUnit* unit) {
    bool found = false;
    for (auto& function : unit->functions) {
        if (function->identifier.value == "main") {
            interpret_block(function->body.get());
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Function " << std::quoted("main")
                  << " not found in program" << std::endl;
        exit(EXIT_FAILURE);
    }
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
    } else {
        throw std::runtime_error("Intepretation of invalid expression");
    }
}

expr_value_t Interpreter::interpret_equality_expr(EqualityExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right.get());
    expr_value_t left_value = interpret_expr(expr->left.get());

    if (expr->op->operator_symbol == OperatorSym::EQ) {
        return right_value == left_value;
    } else if (expr->op->operator_symbol == OperatorSym::NOT_EQ) {
        return right_value != left_value;
    }

    assert(false);
}

expr_value_t Interpreter::interpret_function_call(FunctionCall* function_call) {
    enter_block();
    auto func = function_scope->get_function(function_call->identifier.value);

    auto& arguments = function_call->argument_list->arguments;
    for (unsigned i = 0; i < arguments.size(); ++i) {
        expr_value_t value = interpret_expr(arguments.at(i).get());
        std::string_view ident =
            func->parameter_list->parameters.at(i)->identifier.value;
        current_env()->set_variable(ident, value);
    }

    expr_value_t value;

    try {
        interpret_block(func->body.get());
    } catch (ReturnException& ret) {
        value = ret.value;
    }

    exit_block();
    return value;
}

bool is_truthy(expr_value_t value) {
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value);
    }
    if (std::holds_alternative<std::string>(value)) {
        return std::get<std::string>(value) == "";
    }
    if (std::holds_alternative<int>(value)) {
        return std::get<int>(value);
    }
    if (std::holds_alternative<float>(value)) {
        return std::get<float>(value);
    }

    assert(false);
}

void Interpreter::interpret_if_statement(IfStatement* if_statement) {
    expr_value_t condition_value =
        interpret_expr(if_statement->condition.get());

    if (is_truthy(condition_value)) {
        interpret_block(if_statement->if_body.get());
    } else {
        interpret_block(if_statement->else_body.get());
    }
}

expr_value_t Interpreter::interpret_literal(Literal* literal) {
    if (auto ptr = dynamic_cast<BooleanLiteral*>(literal)) {
        return ptr->value;
    }
    if (auto ptr = dynamic_cast<StringLiteral*>(literal)) {
        return std::string(ptr->value);
    }
    if (auto ptr = dynamic_cast<IntegerLiteral*>(literal)) {
        return ptr->value;
    }

    assert(false);
}

void Interpreter::interpret_operator(Operator*) { assert(false); }

void Interpreter::interpret_param_list(ParameterList*) { assert(false); }

void Interpreter::interpret_return(ReturnStatement* ret) {
    // Unwind stack to caller
    throw ReturnException(interpret_expr(ret->return_value.get()));
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
    }

    assert(false);
}

void Interpreter::interpret_type(Type*) { assert(false); }

expr_value_t Interpreter::interpret_unary_expr(UnaryExpression* expr) {
    expr_value_t value = interpret_expr(expr->expression.get());

    if (expr->op->operator_symbol == OperatorSym::MINUS) {
        if (std::holds_alternative<int>(value)) {
            return -std::get<int>(value);
        } else if (std::holds_alternative<float>(value)) {
            return -std::get<float>(value);
        } else {
            assert(false);
        }
    }

    assert(false);
}

void Interpreter::interpret_variable_decl(VariableDecl* var_decl) {
    current_env()->set_variable(var_decl->identifier.value,
                                interpret_expr(var_decl->value.get()));
}

expr_value_t Interpreter::interpret_variable_expr(VariableExpression* expr) {
    return current_env()->get_variable(expr->identifier.value);
}

void Interpreter::interpret_while(While*) {}

void Interpreter::enter_block() {
    auto new_env = std::make_unique<Environment>();
    new_env->parent = current_env();
    environments.push(std::move(new_env));
}

void Interpreter::exit_block() { environments.pop(); }

void Environment::set_variable(std::string_view ident, expr_value_t data) {
    variable_data[ident] = data;
}

expr_value_t Environment::get_variable(std::string_view ident) {
    auto it = variable_data.find(ident);
    if (it != std::end(variable_data)) {
        return it->second;
    } else if (parent) {
        return parent->get_variable(ident);
    }

    // This should not be able to happen as the typechecker should find these
    // instances and terminate.
    assert(false);
}

Environment* Interpreter::current_env() { return environments.top().get(); }