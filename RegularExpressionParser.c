#include <stdio.h>
#include <stdlib.h>

/* Типы узлов дерева разбора регулярного выражения. */
typedef enum {
    NODE_LITERAL,
    NODE_CONCAT,
    NODE_ALTERNATION,
    NODE_STAR
} NodeType;

/* Структура узла для дерева разбора. */
typedef struct Node {
    NodeType type;
    char c;               // Символ для узлов литералов.
    struct Node *left;    // Левый подузел (например, операнд).
    struct Node *right;   // Правый подузел (например, для конкатенации или альтернативы).
} Node;

/* Функция вывода сообщения об ошибке и завершения программы. */
void error(const char *message, char symbol) {
    if (symbol != '\0') {
        fprintf(stderr, "Ошибка: %s '%c'\n", message, symbol);
    } else {
        fprintf(stderr, "Ошибка: %s\n", message);
    }
    exit(EXIT_FAILURE);
}

/* Функция создания нового узла дерева. */
Node* create_node(NodeType type, char c, Node* left, Node* right) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        error("Ошибка выделения памяти", '\0');
    }
    node->type = type;
    node->c = c;
    node->left = left;
    node->right = right;
    return node;
}

/* Декларации функций парсера.
   Каждая функция принимает указатель на входную строку (через ссылку на указатель),
   что позволяет обновлять позицию в строке во время рекурсивного спуска. */
Node* parse_expression(const char **input);
Node* parse_term(const char **input);
Node* parse_factor(const char **input);
Node* parse_base(const char **input);

/* Парсинг выражения: term ('|' term)* */
Node* parse_expression(const char **input) {
    Node *node = parse_term(input);
    while (**input == '|') {
        (*input)++;  // Пропускаем символ '|'
        Node *right = parse_term(input);
        node = create_node(NODE_ALTERNATION, '\0', node, right);
    }
    return node;
}

/* Парсинг терма: factor (factor)* */
Node* parse_term(const char **input) {
    Node *node = parse_factor(input);
    while (**input && **input != ')' && **input != '|') {
        Node *next = parse_factor(input);
        node = create_node(NODE_CONCAT, '\0', node, next);
    }
    return node;
}

/* Парсинг фактора: base ('*')* */
Node* parse_factor(const char **input) {
    Node *node = parse_base(input);
    while (**input == '*') {
        (*input)++;  // Пропускаем символ '*'
        node = create_node(NODE_STAR, '\0', node, NULL);
    }
    return node;
}

/* Парсинг базового элемента: литерал или выражение в скобках */
Node* parse_base(const char **input) {
    if (**input == '(') {
        (*input)++;  // Пропускаем '('
        Node *node = parse_expression(input);
        if (**input != ')') {
            error("Ожидалась ')'", **input);
        }
        (*input)++;  // Пропускаем ')'
        return node;
    }
    if (**input == '\0' || **input == '|' || **input == ')') {
        error("Неожиданный символ", **input);
    }
    char c = **input;
    (*input)++;  // Продвигаемся на следующий символ
    return create_node(NODE_LITERAL, c, NULL, NULL);
}

/* Функция для вывода дерева разбора в человекочитаемом виде. */
void print_tree(Node *node) {
    if (!node)
        return;
    
    switch (node->type) {
        case NODE_LITERAL:
            printf("%c", node->c);
            break;
        case NODE_CONCAT:
            printf("CONCAT(");
            print_tree(node->left);
            printf(", ");
            print_tree(node->right);
            printf(")");
            break;
        case NODE_ALTERNATION:
            printf("ALT(");
            print_tree(node->left);
            printf(", ");
            print_tree(node->right);
            printf(")");
            break;
        case NODE_STAR:
            printf("STAR(");
            print_tree(node->left);
            printf(")");
            break;
        default:
            break;
    }
}

/* Очистка дерева разбора, освобождение памяти. */
void free_tree(Node *node) {
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

/* Основная функция программы. */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <регулярное_выражение>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *input = argv[1];
    Node *root = parse_expression(&input);
    if (*input != '\0') {
        fprintf(stderr, "Ошибка: не обработанные символы: %s\n", input);
        free_tree(root);
        return EXIT_FAILURE;
    }
    printf("Дерево разбора регулярного выражения:\n");
    print_tree(root);
    printf("\n");
    free_tree(root);
    return EXIT_SUCCESS;
}