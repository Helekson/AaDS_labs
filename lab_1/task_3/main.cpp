#include "complex_number.h"
#include <iostream>

int main() {
    try {
        complex_number a(3, 4);
        complex_number b(1, -2);
        
        // Тест операций
        std::cout << "a = " << a << "\n";       // (3+4i)
        std::cout << "b = " << b << "\n\n";     // (1-2i)
        
        auto sum = a + b;
        std::cout << "a + b = " << sum << "\n"; // (4+2i)
        
        auto diff = a - b;
        std::cout << "a - b = " << diff << "\n";// (2+6i)
        
        auto product = a * b;
        std::cout << "a * b = " << product << "\n"; // (11-2i)
        
        auto quotient = a / b;
        std::cout << "a / b = " << quotient << "\n";// (-1+2i)
        
        // Тест методов
        std::cout << "|a| = " << a.abs() << "\n";  // 5.0
        std::cout << "arg(a) = " << a.arg() << " rad\n"; 
        
        // Тест ввода
        complex_number c;
        std::cout << "Введите комплексное число в формате (re+imi): ";
        std::cin >> c;
        std::cout << "Вы ввели: " << c << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}
