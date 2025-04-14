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
    "ab",
    "aab",
    "abb",
    "aabb",
    "baba",
    "baab",
    "abc",
    ""
]

for s in test_strings:
    result = check_string(s)
    print(f"Строка {s!r}: {'подходит' if result else 'не подходит'}")