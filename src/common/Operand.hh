#include <optional>

#include "common/BuiltinFunctionAddress.hh"
#include "common/ConstantPool.hh"
#include "common/Register.hh"

#include "interpreter/InstructionAddress.hh"

    class Operand {
       public:
        explicit Operand(BuiltinFunctionAddress addr);
        explicit Operand(ConstantPool::Entry constant);
        explicit Operand(InstructionAddress addr);
        explicit Operand(Register reg);

        static Operand empty();

        bool is_address() const;
        bool is_builtin() const;
        bool is_constant() const;
        bool is_register() const;
        bool is_used() const;

        BuiltinFunctionAddress as_builtin() const;
        ConstantPool::Entry as_constant() const;
        InstructionAddress as_address() const;
        Register as_register() const;

        std::string to_debug_string() const;
        std::string to_string() const;

       private:
        Operand();

        enum Type {
          BuiltinAddress,
          ConstantEntry,
          Empty,
          Address,
          Reg,
        };

        union Data {
          Data(BuiltinFunctionAddress addr);
          Data(ConstantPool::Entry constant_entry);
          Data(InstructionAddress addr);
          Data(Register reg);

          BuiltinFunctionAddress builtin_addr;
          ConstantPool::Entry constant_entry;
          InstructionAddress addr;
          Register reg;
        };

        Type m_type;
        std::optional<Data> m_data;
    };

