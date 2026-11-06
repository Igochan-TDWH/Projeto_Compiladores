// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header


ArithExpr* ast_id(char* name) {
  ArithExpr* node = (ArithExpr*) malloc(sizeof(ArithExpr));
  node->kind = E_ID;
  node->attr.id = name;
  return node;
}

ArithExpr* ast_integer(int v) {
  ArithExpr* node = (ArithExpr*) malloc(sizeof(ArithExpr));
  node->kind = E_INTEGER;
  node->attr.int_value = v;
  return node;
}

ArithExpr* ast_float(float v) {
  ArithExpr* node = (ArithExpr*) malloc(sizeof(ArithExpr));
  node->kind = E_FLOAT;
  node->attr.float_value = v;
  return node;
}

ArithExpr* ast_operation
(int operator, ArithExpr* left, ArithExpr* right) {
  ArithExpr* node = (ArithExpr*) malloc(sizeof(ArithExpr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}
BoolExpr* boolean_ast_value(bool v){
    BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
    node->kind = E_BOOL_VAL;
    node->attr.boolean_value = v;
    return node;
}

BoolExpr* boolean_ast_id(char* name){
    BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
    node->kind = E_BOOL_ID;
    node->attr.id = name;
    return node;
}

BoolExpr* boolean_ast_neg(BoolExpr* v) {
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_NEGATION;
  node->attr.neg = v;
  return node;
}

BoolExpr* boolean_ast_operation
(int operator, BoolExpr* left, BoolExpr* right) {
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

BoolExpr* boolean_ast_relational
(int operator, ArithExpr* left,ArithExpr* right) {
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_RELATIONAL;
  node->attr.rel.operator = operator;
  node->attr.rel.left = left;
  node->attr.rel.right = right;
  return node;
}

Expr* expr_ast_arith(ArithExpr* v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_ARITH;
  node->attr.arith = v;
  return node;
}
Expr* expr_ast_bool(BoolExpr* v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_BOOL;
  node->attr.boolean = v;
  return node;
}

Cmd* cmd_ast_expr(Expr* v){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_EXPR;
  node->attr.expr = v;
  return node;
}

Cmd* cmd_ast_declare(char* var,int type, Expr* value){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_DECLARE;
  node->attr.declare.var = var;
  node->attr.declare.type = type;
  node->attr.declare.value = value;
  return node;
}

Cmd* cmd_ast_assigment(char* var, Expr* value){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_DECLARE;
  node->attr.assign.var = var;
  node->attr.assign.value = value;
  return node;
}
Cmd* cmd_ast_conditional(BoolExpr* condition,Cmd* cmd, Cmd* cmd_next){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_IF;
  node->attr.cond.condition = condition;
  node->attr.cond.cmd = cmd;
  node->attr.cond.cmd_next = cmd_next;
  return node;
}

Cmd* cmd_ast_conditional_else(BoolExpr* condition,Cmd* cmd_then, Cmd* cmd_else, Cmd* cmd_next){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_IF_ELSE;
  node->attr.cond_else.condition = condition;
  node->attr.cond_else.cmd_then = cmd_then;
  node->attr.cond_else.cmd_else = cmd_else;
  node->attr.cond_else.cmd_next = cmd_next;
  return node;
}

Cmd* cmd_ast_list(Cmd* head, Cmd* rest){
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_LIST;
  node->attr.list.head = head;
  node->attr.list.rest = rest;
  return node;
}

Cmd* cmd_ast_loop(BoolExpr* condition, Cmd* cmd, Cmd* cmd_next){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_WHILE;
  node->attr.loop.condition = condition;
  node->attr.loop.cmd = cmd;
  node->attr.loop.cmd_next = cmd_next;
  return node;
}

Cmd* cmd_ast_read(char* var){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_READ;
  node->attr.read_var = var;
  return node;
}

Cmd* cmd_ast_write(char* var){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_WRITE;
  node->attr.write_var = var;
  return node;
}