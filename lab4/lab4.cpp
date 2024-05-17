#include <iostream>
#include <string>
using namespace std;

struct Stack {
    double info;
    Stack* next;
} *begin;

Stack* InStack1(Stack* p, char in) {
    Stack* t = new Stack;
    t->info = in;
    t->next = p;
    return t;
}
Stack* InStack2(Stack* p, double in) {
    Stack* t = new Stack;
    t->info = in;
    t->next = p;
    return t;
}

Stack* OutStack1(Stack*& p, double& out) {
    if (p != NULL) {
        Stack* t = p;
        out = p->info;
        p = p->next;
        delete t;
    }
    return p;
}
Stack* OutStack2(Stack*& p, char& out) {
    if (p != NULL) {
        Stack* t = p;
        out = p->info;
        p = p->next;
        delete t;
    }
    return p;
}

int Priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '$') return 2;
    return 0;
}

string InfixToPostfix(const string& infix) {
    string postfix = "";
    Stack* stack = NULL;
    char topSymbol = '\0';

    for (char symbol : infix) {
        if (isalpha(symbol)) {
            postfix += symbol;
        }
        else if (symbol == '(') {
            stack = InStack1(stack, symbol);
        }
        else if (symbol == ')') {
            while (stack != NULL && stack->info != '(') {
                stack = OutStack2(stack, topSymbol);
                postfix += topSymbol;
            }
            stack = OutStack2(stack, topSymbol);
        }
        else {
            while (stack != NULL && Priority(stack->info) >= Priority(symbol)) {
                stack = OutStack2(stack, topSymbol);
                postfix += topSymbol;
            }
            stack = InStack1(stack, symbol);
        }
    }

    while (stack != NULL) {
        stack = OutStack2(stack, topSymbol);
        postfix += topSymbol;
    }

    return postfix;
}

double EvaluatePostfix(const string& postfix, double values[]) {
    Stack* stack = NULL;
    for (char symbol : postfix) {
        if (isalpha(symbol)) {
            stack = InStack2(stack, values[symbol - 'a']);
        }
        else {
            double a, b;
            OutStack1(stack, b);
            OutStack1(stack, a);
            switch (symbol) {
            case '+': stack = InStack2(stack, a + b); break;
            case '-': stack = InStack2(stack, a - b); break;
            case '*': stack = InStack2(stack, a * b); break;
            case '/': stack = InStack2(stack, a / b); break;
            case '$': stack = InStack2(stack, (a + b) * 2); break;
            }
        }
    }
    double result;
    OutStack1(stack, result);
    return result;
}

int main() {
    double values[5];
    string variables = "abcde";

    for (int i = 0; i < 5; i++) {
        cout << "Введите значение для " << variables[i] << ": ";
        cin >> values[i];
    }
    
    string infix = "a$b+c+d+e";
    cout << "Инфиксное выражение: " << infix << endl;
    string postfix = InfixToPostfix(infix);
    cout << "Постфиксное выражение: " << postfix << endl;

    double result = EvaluatePostfix(postfix, values);
    cout << "Результат вычисления: " << result << endl;
    return 0;
}
//добавить $ = (a+b)*2 $=*