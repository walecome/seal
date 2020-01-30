#pragma once

class IrProgram;
class IrFunction;

class Interpreter {
   public:
    Interpreter(const IrProgram* ir_program) : m_ir_program { ir_program } {}

    void interpret();

   private:
    void interpret_function(const IrFunction* function);

    const IrProgram* m_ir_program;
};