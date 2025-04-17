# Детерминированный конечный автомат (ДКА)
# Фиксированный алфавит: {a, b}


def check_string(s: str) -> bool:
    state = 0

    for char in s:
        if char not in ('a', 'b'):
            return False

        if state == 0:
            if char == 'a':
                state = 1
        elif state == 1:
            if char == 'a':
                state = 1
            elif char == 'b':
                state = 2
        elif state == 2:
            if char == 'a':
                state = 1
            elif char == 'b':
                state = 0

    return state == 2


test_strings = [
    "aaaaaaaaaaaaaaaaaba"
]

for s in test_strings:
    result = "подходит" if check_string(s) else "не подходит"
    print(f"Строка {s!r}: {result}")
