#include <string.h> // for strcmp
#include<stdio.h> // remove after testing
#include "stack.h" // Stack header

void initialize(Stack* stack){
    stack->top = NULL;
} 

void push(Stack* stack,Node* n) {
    if (stack->top == NULL) {
        n->next = NULL;
        stack->top = n;
    } else {
        n->next = stack->top;
        stack->top = n;
    }
} 

Node* pop(Stack* stack) {
    Node* tmp = stack->top->next;
    Node* res = stack->top;
    stack->top = tmp;
    return res;
}

bool empty(Stack* stack){
    return stack->top == NULL;
}

Node* contains_assist(Node* curr, char* name){
    if (curr == NULL) return NULL;      // end of stack
    if (strcmp(curr->var.name,name) == 0) return curr;
    return contains_assist(curr->next,name);
}

Node* contains(Stack* stack, char* name){
    return contains_assist(stack->top,name);
}

