#include <doggoscript/types/classes/number_class.hpp>

Instance *NumberClass::new_instance(double initial_value) {
    auto *str_cls = new NumberClass(initial_value);
    auto *instance = new Instance(str_cls);

    return instance;
}

NumberClass::NumberClass(double initial_value) : BuiltInClass("Number") {
    this->value = initial_value;
    this->cls_type = BuiltInType::Number;
}
