from transitions.extensions import GraphMachine

def is_a(**kwargs): return kwargs.get('char') == 'a'
def is_not_a(**kwargs): return kwargs.get('char') != 'a'
def is_b(**kwargs): return kwargs.get('char') == 'b'
def not_a_or_b(**kwargs): return kwargs.get('char') not in ['a', 'b']

detector = type("Detector", (), {})()
states = ['q0', 'q1', 'q2', 'q3', 'q4']
machine = GraphMachine(model=detector, states=states, initial='q0', show_conditions=True)

transitions = [
    ('q0', 'advance', 'q1', is_a),
    ('q0', 'advance', 'q0', is_not_a),

    ('q1', 'advance', 'q2', is_b),
    ('q1', 'advance', 'q1', is_a),
    ('q1', 'advance', 'q0', not_a_or_b),

    ('q2', 'advance', 'q3', is_b),
    ('q2', 'advance', 'q1', is_a),
    ('q2', 'advance', 'q0', not_a_or_b),

    ('q3', 'advance', 'q4', is_a),
    ('q3', 'advance', 'q0', is_not_a),

    ('q4', 'advance', 'q4', True)
]

for source, trigger, dest, cond in transitions:
    machine.add_transition(trigger, source, dest, conditions=cond)

test_string = "xxabba123"
print(f"Проверяем строку: {test_string}")

for ch in test_string:
    detector.advance(char=ch)
    if detector.state == 'q4':
        print(f"Строка '{test_string}' содержит подстроку 'abba'.")
        break

graph = detector.get_graph()
graph.layout(prog="dot")
graph.draw("state_diagram.png")
print("Диаграмма автомата сохранена в файле 'state_diagram.png'.")