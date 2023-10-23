#include <doggoscript/types/classes/error_class.hpp>
#include <doggoscript/types/classes/string_class.hpp>

Instance* ErrorClass::new_instance(BaseError initial_value) {
    auto* str_cls  = new ErrorClass(initial_value);
    auto* instance = new Instance(str_cls);

    return instance;
}

ErrorClass::ErrorClass(BaseError error) : BuiltInClass("Error") {
    this->cls_type = BuiltInType::Error;
    this->error_str = error.str();

    this->symbol_table->set("$name", new StringClass(error.error_name));
    this->symbol_table->set("$details", new StringClass(error.details));
    this->symbol_table->set("__string", new BuiltInFunction(
        "__string", {}, [this](std::vector<Object*> args) -> RuntimeResult {
            return *RuntimeResult().success(new StringClass(this->error_str));
        }));
}
