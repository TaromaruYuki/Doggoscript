#include <doggoscript/types/classes/boolean_class.hpp>

Instance* BooleanClass::new_instance(bool initial_value) {
    auto* str_cls  = new BooleanClass(initial_value);
    auto* instance = new Instance(str_cls);

    return instance;
}

BooleanClass::BooleanClass(bool initial_value) : BuiltInClass("Boolean") {
    this->value    = initial_value;
    this->cls_type = BuiltInType::Boolean;
}
