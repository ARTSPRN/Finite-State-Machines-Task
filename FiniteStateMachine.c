#include <stdio.h>

// Функция вычисления длины строки с использованием арифметики указателей.
size_t my_strlen(const char *s) {
    const char *p = s;
    while (*p) {
        ++p;
    }
    return (size_t)(p - s);
}

// Удаляет завершающий символ новой строки, если он присутствует.
void remove_newline(char *s) {
    size_t len = my_strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

/* Автомат на основе конструкции switch */
// Язык, распознаваемый автоматом, допускает только символы 'a' и 'b'.
// Строка принимается, если после обработки она достигает состояния STATE_TWO_B.
typedef enum {
    STATE_INIT,
    STATE_ONE_B,
    STATE_TWO_B,
    STATE_REJECT
} state_switch_t;

// Функция перехода в следующее состояние для switch-автомата.
state_switch_t next_state_switch(state_switch_t state, char c) {
    switch (state) {
        case STATE_INIT:
            if (c == 'a') {
                return STATE_INIT;
            } else if (c == 'b') {
                return STATE_ONE_B;
            }
            break;
        case STATE_ONE_B:
            if (c == 'a') {
                return STATE_ONE_B;
            } else if (c == 'b') {
                return STATE_TWO_B;
            }
            break;
        case STATE_TWO_B:
            if (c == 'a') {
                return STATE_TWO_B;
            }
            break;
        default:
            break;
    }
    return STATE_REJECT;
}

// Проверка строки с использованием switch-автомата. Возвращает ненулевое значение, если строка распознана.
int check_string_switch(const char *s) {
    state_switch_t state = STATE_INIT;
    while (*s) {
        state = next_state_switch(state, *s);
        if (state == STATE_REJECT) {
            return 0;
        }
        s++;
    }
    return (state == STATE_TWO_B);
}

/* Автомат на основе таблицы переходов */
// Символ 'a' соответствует индексу 0, 'b' – индексу 1.
// Строка принимается, если после обработки автомат оказывается в состоянии S2_T.
typedef enum {
    S0_T,
    S1_T,
    S2_T,
    REJECT_T
} state_table_t;

// Проверка строки с использованием таблицы переходов. 
int check_string_table(const char *s) {
    static const int transition[4][2] = {
        [S0_T]     = { S0_T,     S1_T },
        [S1_T]     = { S1_T,     S2_T },
        [S2_T]     = { S2_T,     REJECT_T },
        [REJECT_T] = { REJECT_T, REJECT_T }
    };

    state_table_t state = S0_T;
    while (*s) {
        int symbol_index = -1;
        if (*s == 'a') {
            symbol_index = 0;
        } else if (*s == 'b') {
            symbol_index = 1;
        } else {
            return 0;  // Недопустимый символ.
        }
        state = transition[state][symbol_index];
        if (state == REJECT_T) {
            return 0;
        }
        s++;
    }
    return (state == S2_T);
}

int main(void) {
    char input[256];

    printf("Введите строку: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Ошибка ввода!\n");
        return 1;
    }

    remove_newline(input);

    if (check_string_switch(input)) {
        printf("Строка принята (реализация switch)\n");
    } else {
        printf("Строка отклонена (реализация switch)\n");
    }

    if (check_string_table(input)) {
        printf("Строка принята (таблица переходов)\n");
    } else {
        printf("Строка отклонена (таблица переходов)\n");
    }

    return 0;
}