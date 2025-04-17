// Детерминированный конечный автомат
// Фиксированный алфавит: {a, b}

#include <iostream>
#include <string>

enum stateCase { // Множество состояний
    S0,
    S1,
    S2,
    BAD
};

int check_string_table(const std::string &s) {
    static constexpr int transition[4][2] = { //C99 designated initializers
        [S0]     = { [0] = S0,     [1] = S1 },
        [S1]     = { [0] = S1,     [1] = S2 },
        [S2]     = { [0] = S2,     [1] = BAD },
        [BAD] = { [0] = BAD, [1] = BAD }
    };
    
    stateCase state = S0;
    for (char c : s) {
        int symbol_index;
        if (c == 'a') symbol_index = 0;
        else if (c == 'b') symbol_index = 1;
        else return 0;
        state = static_cast<stateCase>(transition[state][symbol_index]);
        if (state == BAD) return 0;
    }
    return (state == S2);
}

int main() {
    std::string input;
    std::cout << "Введите строку: ";
    std::getline(std::cin, input);
    
    if (check_string_table(input)) std::cout << "Строка принята" << std::endl;
    else std::cout << "Строка отклонена"  << std::endl;
    
    return 0;
}