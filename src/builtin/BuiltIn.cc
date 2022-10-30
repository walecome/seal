#include <algorithm>
#include <functional>
#include <map>

#include "fmt/format.h"

#include "BuiltIn.hh"

#include "ast/FunctionCall.hh"
#include "common/Value.hh"

namespace {
unsigned get_and_increment_func_id() {
    static unsigned func_id = 0;
    return func_id++;
}

Value print(const std::vector<Value>& args) {
    std::vector<std::string> stringified_args {};
    for (auto arg : args) {
        stringified_args.push_back(arg.stringify());
    }
    std::string s = fmt::format("{}", fmt::join(stringified_args, ""));
    fmt::print("{}", s);
    return Value::create_integer(s.size());
}

class Print : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~Print() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>& args) const override {
        return print(args);
    }

    std::string_view name() const override {
        return "print";
    }
};

class Println : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~Println() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>& args) const override {
        unsigned long chars_printed = print(args).as_integer().value();
        fmt::print("\n");
        return Value::create_integer(chars_printed + 1);
    }

    std::string_view name() const override {
        return "println";
    }
};

class CreateArray : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~CreateArray() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>& args) const override {
        ASSERT(args.size() == 1);
        Value size = args[0];

        ASSERT(size.is_integer());
        Value vec = Value::create_vector();

        for (int i = 0; i < size.as_integer().value(); ++i) {
            vec.as_vector().add(Value {});
        }

        return vec;
    }

    std::string_view name() const override {
        return "create_array";
    }
};

class AddElement : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~AddElement() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>& args) const override {
        ASSERT(args.size() == 2);

        Value vec = args[0];
        ASSERT(vec.is_vector());
        vec.as_vector().add(args[1]);

        return Value::create_integer(vec.as_vector().length());
    }

    std::string_view name() const override {
        return "add_element";
    }
};

class GetLength : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~GetLength() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>& args) const override {
        ASSERT(args.size() == 1);

        Value target = args[0];

        if (target.is_string()) {
            return Value::create_integer(target.as_string().length());
        }

        if (target.is_vector()) {
            return Value::create_integer(target.as_vector().length());
        }

        ASSERT_NOT_REACHED();
    }

    std::string_view name() const override {
        return "get_length";
    }
};

class Halt : public BuiltIn::BuiltinFunction {
   public:
    using BuiltIn::BuiltinFunction::BuiltinFunction;
    ~Halt() override = default;

    Type typecheck(const FunctionCall&) const override {
        return Type { Primitive::DONT_CARE };
    }

    Value call(const std::vector<Value>&) const override {
        fmt::print("Builtin halt() called!\n");
        exit(0);
    }

    std::string_view name() const override {
        return "halt";
    }
};

template <class T>
void add_map_entry(
    std::map<std::string_view, ptr_t<BuiltIn::BuiltinFunction>>& map) {
    auto func = std::make_unique<T>(get_and_increment_func_id());
    map.insert({ func->name(), std::move(func) });
}

static const std::map<std::string_view, ptr_t<BuiltIn::BuiltinFunction>>
    func_name_map = [] {
        std::map<std::string_view, ptr_t<BuiltIn::BuiltinFunction>> func_map {};

        add_map_entry<Print>(func_map);
        add_map_entry<Println>(func_map);
        add_map_entry<CreateArray>(func_map);
        add_map_entry<AddElement>(func_map);
        add_map_entry<GetLength>(func_map);
        add_map_entry<Halt>(func_map);

        return func_map;
    }();

static const std::vector<BuiltIn::BuiltinFunction*> func_id_map = [] {
    std::vector<BuiltIn::BuiltinFunction*> id_map {};
    id_map.resize(func_name_map.size());
    for (const auto& entry : func_name_map) {
        id_map[entry.second->id()] = entry.second.get();
    }
    return id_map;
}();

}  // namespace

namespace BuiltIn {

BuiltinFunction::BuiltinFunction(size_t id) : m_id(id) {
}

BuiltinFunction::~BuiltinFunction() = default;

size_t BuiltinFunction::id() const {
    return m_id;
}

BuiltinFunction* find_builtin(std::string_view name) {
    auto it = func_name_map.find(name);
    if (it == func_name_map.end()) {
        return nullptr;
    }
    return it->second.get();
}

BuiltinFunction& get_builtin(BuiltinFunctionAddress addr) {
    ASSERT(addr.id() < func_id_map.size());
    return *func_id_map[addr.id()];
}

}  // namespace BuiltIn
