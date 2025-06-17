#ifndef LOGICAL_VALUES_ARRAY_H
#define LOGICAL_VALUES_ARRAY_H

class logical_values_array {
private:
    unsigned int value;

public:
    logical_values_array(unsigned int val = 0);
    unsigned int get_value() const;
    logical_values_array inversion() const;
    logical_values_array conjunction(const logical_values_array& other) const;
    logical_values_array disjunction(const logical_values_array& other) const;
    logical_values_array implication(const logical_values_array& other) const;
    logical_values_array compplication(const logical_values_array& other) const;
    logical_values_array xor_op(const logical_values_array& other) const;
    logical_values_array equivalence(const logical_values_array& other) const;
    logical_values_array pierce_arrow() const;
    logical_values_array sheffer_stroke() const;
    static bool equals(const logical_values_array& a, const logical_values_array& b);
    bool get_bit(size_t pos) const;
    bool operator[](size_t pos) const;
    void to_binary_string(char* buffer) const;
};

#endif
