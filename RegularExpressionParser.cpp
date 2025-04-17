#include <iostream>
#include <string>
using namespace std;

string input;
size_t pos = 0;

bool expr();

bool base() { // base  -> '(' expr ')' | символ
    if (input[pos] == ' ') return false;
    if (pos == input.size()) return false;
    if (input[pos] == '(') {
        pos++;   
        if (!expr() || pos == input.size() || input[pos] != ')') return false;
        pos++; 
        return true;
    }
    if (input[pos] == ')' || input[pos] == '|' || input[pos] == '*') return false;
    pos++; 
    return true; 
}

bool factor() { // factor -> base ('*')*
    if (!base() || (input[pos] == '*' && pos == 0)) return false;
    while (pos < input.size() && input[pos] == '*') {

        if(input[pos-1] == '*') return false;
        pos++;
        
    }
    return true;
}

bool term() { //term -> factor+ (хотя бы один)
    bool ok = false;
    while (pos < input.size() && input[pos] != ')' && input[pos] != '|') {
        if (!factor()) return false;
        ok = true;
    }
    return ok;
}

bool expr() { // expr -> term ('|' term)* 
    if (!term()) return false;
    while (pos < input.size() && input[pos] == '|') {
        pos++; 
        if (!term()) return false;
    }
    return true;
}

int main() {
    cout << "Введите регулярное выражение: ";
    getline(cin, input);
    cout << ((input.size() == 0) ||(expr() && pos == input.size() && input != "*") ? "Выражение является регулярным\n" : "Выражение не является регулярным\n");
    return 0;
}