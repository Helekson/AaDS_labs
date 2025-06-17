#include "logical_values_array.h"

logical_values_array::logical_values_array(unsigned int val) : value(val) {}

unsigned int logical_values_array::get_value() const {
    return value;
}

logical_values_array logical_values_array::inversion() const {
    return logical_values_array(~value);
}

logical_values_array logical_values_array::conjunction(const logical_values_array& other) const {
    return logical_values_array(value & other.value);
}

logical_values_array logical_values_array::disjunction(const logical_values_array& other) const {
    return logical_values_array(value | other.value);
}

logical_values_array logical_values_array::implication(const logical_values_array& other) const {
    return inversion().disjunction(other);
}

logical_values_array logical_values_array::compplication(const logical_values_array& other) const {
    return disjunction(other.inversion());
}

logical_values_array logical_values_array::xor_op(const logical_values_array& other) const {
    return logical_values_array(value ^ other.value);
}

logical_values_array logical_values_array::equivalence(const logical_values_array& other) const {
    return conjunction(other).disjunction(inversion().conjunction(other.inversion()));
}

logical_values_array logical_values_array::pierce_arrow() const {
    return disjunction(logical_values_array()).inversion();
}

logical_values_array logical_values_array::sheffer_stroke() const {
    return conjunction(logical_values_array()).inversion();
}

bool logical_values_array::equals(const logical_values_array& a, const logical_values_array& b) {
    return a.value == b.value;
}

bool logical_values_array::get_bit(size_t pos) const {
    return (value >> pos) & 1;
}

bool logical_values_array::operator[](size_t pos) const {
    return get_bit(pos);
}

void logical_values_array::to_binary_string(char* buffer) const {
    for (int i = 31; i >= 0; --i) {
        buffer[31 - i] = (value & (1 << i)) ? '1' : '0';
    }
    buffer[32] = '\0';
}
