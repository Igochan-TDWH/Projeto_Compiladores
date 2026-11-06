#include "ast.h"
// Stack definitions
#ifndef __stack_h__
#define __stack_h__

// Node definition
struct _Node {
    struct {
        char* name;
        struct _Expr* value;
        int type;
    } var;
    struct _Node* next;
};

typedef struct _Node Node; // Convenience typedef

// Stack for variables
struct _Stack {
    struct _Node* top;
};

typedef struct _Stack Stack; // Convenience typedef

void push(Stack* stack,Node* n); 
void initialize(Stack* stack); 
Node* pop(Stack* stack); 
bool empty(Stack* stack);
Node* contains(Stack* stack, char* name);
#endif