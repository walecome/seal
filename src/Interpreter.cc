#include "Interpreter.hh"

void Interpreter::interpret_argument_list(ArgumentList* arg_list) {
    for (auto& argument : arg_list->arguments) {
        interpret_expr(argument.get());
    }
}

void Interpreter::interpret_array_literal(ArrayLiteral* array_literal) {
    for (auto& expr : array_literal->expressions) {
        interpret_expr(expr.get());
    }
}

expr_value_t Interpreter::interpret_assign_expr(AssignExpression* expr) {
    interpret_expr(expr->right.get());
    // @TODO: Set variables
}

template <typename T, typename Op>
expr_value_t bin_op(expr_value_t a, expr_value_t b, Op op) {
    return op(std::get<T>(a), std::get<T>(b));
}

template <typename Op>
expr_value_t bin_op(expr_value_t a, expr_value_t b, Op op) {
    assert(a.index() == b.index());
    if (std::holds_alternative<int>(a)) return bin_op<int>(a, b, op);
    if (std::holds_alternative<float>(a)) return bin_op<float>(a, b, op);
    if (std::holds_alternative<std::string>(a))
        return bin_op<std::string>(a, b, std::plus());
    if (std::holds_alternative<bool>(a)) return bin_op<bool>(a, b, op);

    assert(false);
}

// Operators other than +, -, * and / should be matched by the other functions
expr_value_t Interpreter::interpret_binary_expr(BinaryExpression* expr) {
    expr_value_t right_value = interpret_expr(expr->right.get());
    expr_value_t left_value = interpret_expr(expr->left.get());

    switch (expr->op->operator_symbol) {
        case OperatorSym::PLUS:
            return bin_op(right_value, left_value, std::plus());
        case OperatorSym::MINUS:
            assert(expr->type != Primitive::STRING);
            return bin_op(right_value, left_value, std::minus());
        case OperatorSym::MULT:
            assert(expr->type != Primitive::STRING);
            return bin_op(right_value, left_value, std::multiplies());
        case OperatorSym::DIV:
            assert(expr->type != Primitive::STRING);
            return bin_op(right_value, left_value, std::divides());
        default:
            assert(false);
    }
}

void Interpreter::interpret_block(Block* block) {
    enter_block();
    for (auto& statement : block->statements) {
        interpret_statement(statement.get());
    }
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

void Interpreter::interpret_compilation_unit(CompilationUnit*) {}

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

void Interpreter::interpret_function(Function*) {
    // @TODO: Register function
}

expr_value_t Interpreter::interpret_function_call(FunctionCall*) {
    // @TODO: Look up function decl and assign params in env
}

void Interpreter::interpret_if_statement(IfStatement*) {}

void Interpreter::interpret_literal(Literal*) {}

void Interpreter::interpret_operator(Operator*) {}

void Interpreter::interpret_param_list(ParameterList*) {}

void Interpreter::interpret_return(ReturnStatement*) {}

void Interpreter::interpret_statement(Statement* statement) {
    // @TODO: Dynamic cast to statements :(
}

void Interpreter::interpret_type(Type*) {}

expr_value_t Interpreter::interpret_unary_expr(UnaryExpression*) {}

void Interpreter::interpret_variable_decl(VariableDecl*) {}

expr_value_t Interpreter::interpret_variable_expr(VariableExpression*) {}

void Interpreter::interpret_while(While*) {}

Environment* Interpreter::current_env() { return environments.top().get(); }

void Interpreter::enter_block() {
    auto new_env = std::make_unique<Environment>();
    new_env->parent = current_env();
    environments.push(std::move(new_env));
}

void Interpreter::exit_block() { environments.pop(); }