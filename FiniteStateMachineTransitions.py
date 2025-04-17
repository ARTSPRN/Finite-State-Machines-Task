from transitions.extensions import GraphMachine


def a(**kwargs):
    return kwargs.get('char') == 'a'


def b(**kwargs):
    return kwargs.get('char') == 'b'


detector = type("DKA", (), {})()
states = ['q0', 'q1', 'q2', 'q3', 'q4']
machine = GraphMachine(
    model=detector,
    states=states,
    initial='q0',
    show_conditions=True
)

transitions = [
    ('q0', 'go', 'q1', a),
    ('q0', 'go', 'q0', b),

    ('q1', 'go', 'q2', b),
    ('q1', 'go', 'q1', a),

    ('q2', 'go', 'q3', b),
    ('q2', 'go', 'q1', a),

    ('q3', 'go', 'q4', a),
    ('q3', 'go', 'q0', b),

    ('q4', 'go', 'q4', True)
]

for source, trigger, dest, cond in transitions:
    machine.add_transition(trigger, source, dest, conditions=cond)

test_string = input()
print(f"Проверяем строку: {test_string}")

for ch in test_string:
    detector.go(char=ch)
    if detector.state == 'q4':
        print(f"Строка '{test_string}' содержит подстроку 'abba'.")
        break

graph = detector.get_graph()
graph.layout(prog="dot")
graph.draw("state_diagram.png")
print(
    "Диаграмма автомата сохранена в файле "
    "'state_diagram.png'."
)
