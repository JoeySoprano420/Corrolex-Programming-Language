#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024

struct CVM {
    int stack[STACK_SIZE];
    int sp; // Stack pointer
};

void cvm_init(struct CVM *cvm) {
    cvm->sp = -1; // Initialize the stack pointer
}

void push(struct CVM *cvm, int value) {
    if (cvm->sp < STACK_SIZE - 1) {
        cvm->stack[++(cvm->sp)] = value;
    } else {
        printf("Stack overflow\n");
    }
}

int pop(struct CVM *cvm) {
    if (cvm->sp >= 0) {
        return cvm->stack[(cvm->sp)--];
    } else {
        printf("Stack underflow\n");
        return -1;
    }
}

void execute(struct CVM *cvm, const char *bytecode) {
    // Execute the bytecode (simplified for demonstration)
    char op;
    int value;

    while (*bytecode != '\0') {
        op = *bytecode++;
        switch (op) {
            case 'P': // PUSH
                value = *bytecode++ - '0';
                push(cvm, value);
                break;
            case 'L': // LOAD
                value = *bytecode++ - '0';
                push(cvm, value);
                break;
            case 'O': // OPERATE
                op = *bytecode++;
                int b = pop(cvm);
                int a = pop(cvm);
                if (op == '+') {
                    push(cvm, a + b);
                } else if (op == '-') {
                    push(cvm, a - b);
                }
                break;
        }
    }
}
