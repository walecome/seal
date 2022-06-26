#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/Type.hh"

#include "common/BuiltinFunctionAddress.hh"
#include "common/Value.hh"

class ArgumentList;
class Context;
class FunctionCall;

namespace BuiltIn {

class BuiltinFunction {
   public:
    BuiltinFunction(size_t id);
    virtual ~BuiltinFunction();
    virtual Type typecheck(const FunctionCall& call) const = 0;
    virtual Value call(const std::vector<Value>& args) const = 0;
    virtual std::string_view name() const = 0;

    size_t id() const;

   protected:
   private:
    size_t m_id;
};

BuiltinFunction* find_builtin(std::string_view name);
BuiltinFunction& get_builtin(BuiltinFunctionAddress addr);

}  // namespace BuiltIn
