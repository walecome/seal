#pragma once

class ArgumentList;
class ArrayLiteral;
class AssignExpression;
class Block;
class BooleanLiteral;
class CalculateExpression;
class CompareExpression;
class CompilationUnit;
class EqualityExpression;
class For;
class FunctionCall;
class FunctionDeclC;
class FunctionDeclUser;
class IfStatement;
class IndexExpression;
class IntegerLiteral;
class LogicalExpression;
class Operator;
class ParameterList;
class RealLiteral;
class ReturnStatement;
class StringLiteral;
class UnaryExpression;
class VariableDecl;
class VariableExpression;
class While;

class AstVisitor {
   public:
    virtual ~AstVisitor() = default;
    virtual void visit(ArgumentList&) const = 0;
    virtual void visit(ArrayLiteral&) const = 0;
    virtual void visit(AssignExpression&) const = 0;
    virtual void visit(Block&) const = 0;
    virtual void visit(BooleanLiteral&) const = 0;
    virtual void visit(CalculateExpression&) const = 0;
    virtual void visit(CompareExpression&) const = 0;
    virtual void visit(CompilationUnit&) const = 0;
    virtual void visit(EqualityExpression&) const = 0;
    virtual void visit(For&) const = 0;
    virtual void visit(FunctionCall&) const = 0;
    virtual void visit(FunctionDeclC&) const = 0;
    virtual void visit(FunctionDeclUser&) const = 0;
    virtual void visit(IfStatement&) const = 0;
    virtual void visit(IndexExpression&) const = 0;
    virtual void visit(IntegerLiteral&) const = 0;
    virtual void visit(LogicalExpression&) const = 0;
    virtual void visit(Operator&) const = 0;
    virtual void visit(ParameterList&) const = 0;
    virtual void visit(RealLiteral&) const = 0;
    virtual void visit(ReturnStatement&) const = 0;
    virtual void visit(StringLiteral&) const = 0;
    virtual void visit(UnaryExpression&) const = 0;
    virtual void visit(VariableDecl&) const = 0;
    virtual void visit(VariableExpression&) const = 0;
    virtual void visit(While&) const = 0;
};
