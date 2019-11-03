#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {

void print(param_type_t args) {
    // @TODO: Maybe allow additional arguments later
    ASSERT(args.size() == 1);

    auto value = args.at(0);

    if (std::holds_alternative<bool>(value))
        std::cout << std::boolalpha << std::get<bool>(value);
    else if (std::holds_alternative<std::string>(value))
        std::cout << std::get<std::string>(value);
    else if (std::holds_alternative<int>(value))
        std::cout << std::get<int>(value);
    else if (std::holds_alternative<float>(value))
        std::cout << std::get<float>(value);
    else
        ASSERT_NOT_REACHED();

    std::cout << std::flush;
}

void println(param_type_t args) {
    print(args);
    std::cout << std::endl;
}
}  // namespace BuiltIn
