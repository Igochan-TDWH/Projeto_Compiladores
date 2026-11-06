#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "stack.h"
#include "parser.h"
#include "printAbsTree.h"

Cmd* root = NULL;

float arith_eval(ArithExpr* expr, Stack* vars) {
  float result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  }
  else if (expr->kind == E_ID){
    Node* v = contains(vars,expr->attr.id);
    if (v == NULL) yyerror("Variable not declared!");
    if (v->var.type == INTEGER) result = v->var.value->attr.arith->attr.int_value;
    else if (v->var.type == REAL) result = v->var.value->attr.arith->attr.float_value;
    else yyerror("Type Error!");
  }
  else if (expr->kind == E_INTEGER) {
    result = expr->attr.int_value; 
  } 
  else if (expr->kind == E_FLOAT) {
    result = expr->attr.float_value; 
  } 
  else if (expr->kind == E_OPERATION) {
    float vLeft = arith_eval(expr->attr.op.left,vars);
    float vRight = arith_eval(expr->attr.op.right,vars);
    switch (expr->attr.op.operator) {
      case PLUS: 
        result = vLeft + vRight;
        break;
      case SUB: 
        result = vLeft - vRight; 
        break;
      case DIV: 
        result = vLeft / vRight; 
        break;
      case MULT: 
        result = vLeft * vRight; 
        break;
      case REST: 
        result = (int)vLeft % (int)vRight; 
        break;
      default: yyerror("Unknown operator!");
    }
  }
  return result;
}

bool bool_eval(BoolExpr* expr, Stack* vars) {
  bool result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  }
  else if (expr->kind == E_BOOL_ID) {
    Node* v = contains(vars,expr->attr.id);
    if (v == NULL) yyerror("Variable not declared!");
    if (v->var.type != BOOL) yyerror("Type error!");
    result = v->var.value->attr.boolean->attr.boolean_value;
  } 
  else if (expr->kind == E_BOOL_VAL) {
    result = expr->attr.boolean_value; 
  } 
  else if (expr->kind == E_BOOL_NEGATION) {
    result = !expr->attr.neg; 
  } 
  else if (expr->kind == E_BOOL_EXPRETION) {
    result = arith_eval(expr->attr.exp,vars); 
  } 
  else if (expr->kind == E_BOOL_OPERATION) {
    bool vLeft = bool_eval(expr->attr.op.left,vars);
    bool vRight = bool_eval(expr->attr.op.right,vars);
    switch (expr->attr.op.operator) {
      case AND:
        result = vLeft && vRight;
        break;
      case OR:
        result = vLeft || vRight;
        break;
      default: yyerror("Unknown operator!");
    }
  }
  else if (expr->kind == E_BOOL_RELATIONAL) {
    bool vLeft = arith_eval(expr->attr.rel.left,vars);
    bool vRight = arith_eval(expr->attr.rel.right,vars);
    switch (expr->attr.op.operator) {
      case LESS: 
        result = vLeft < vRight; 
        break;
      case MORE: 
        result = vLeft > vRight; 
        break;
      case EQUAL: 
        result = vLeft == vRight; 
        break;
      case LEQUAL: 
        result = vLeft <= vRight; 
        break;
      case MEQUAL: 
        result = vLeft >= vRight; 
        break;
      case DIFF: 
        result = vLeft != vRight; 
        break;
      default: yyerror("Unknown operator!");
    }
  }
  return result;
}

long int expr_eval(Expr* expr,Stack* vars){
  long int result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  }
  else if (expr->kind == E_ARITH) {
    result = (long int)arith_eval(expr->attr.arith,vars); 
  } 
  else if (expr->kind == E_BOOL) {
    result = (long int)bool_eval(expr->attr.boolean,vars); 
  } 
  else if (expr->kind == E_CHAR || expr->kind == E_STR){
    result = (long int)expr->attr.str;
  }
  return result;
}

void eval(Cmd* cmd,Stack* vars){
  if (cmd == 0) {
    yyerror("Null expression!!");
  }
  else if (cmd->kind == CMD_EXPR) {
    expr_eval(cmd->attr.expr,vars);
  } 
  else if (cmd->kind == CMD_DECLARE) {
    Node* v = (Node*) malloc(sizeof(Node));
    v->var.name = cmd->attr.declare.var;
    v->var.type = cmd->attr.declare.type;
    long int val = expr_eval(cmd->attr.declare.value,vars);
    switch (v->var.type)
    {
      case INTEGER:
        v->var.value->attr.arith->attr.int_value = (int)val;
        break;
      case FLOAT:
        v->var.value->attr.arith->attr.float_value = (float)val;
        break;
      case BOOL:
        v->var.value->attr.boolean->attr.boolean_value = (bool)val;
        break;
      case STRING:
        v->var.value->attr.str = (char*)val;
        break;
      case CHAR:
        v->var.value->attr.str = (char*)val;
        break;
      default: yyerror("Type error!");
    }
    push(vars,v);
  } 
  else if (cmd->kind == CMD_ASSIGN) {
    Node* v = contains(vars,cmd->attr.assign.var);
    if  (v == NULL) yyerror("Variable not declared!");
    v->var.name = cmd->attr.assign.var;
    long int val = expr_eval(cmd->attr.declare.value,vars);
    switch (v->var.type)
    {
      case INTEGER:
        v->var.value->attr.arith->attr.int_value = (int)val;
        break;
      case FLOAT:
        v->var.value->attr.arith->attr.float_value = (float)val;
        break;
      case BOOL:
        v->var.value->attr.boolean->attr.boolean_value = (bool)val;
        break;
      case STRING:
        v->var.value->attr.str = (char*)val;
        break;
      case CHAR:
        v->var.value->attr.str = (char*)val;
        break;
      default: yyerror("Type error!");
    }
  } 
  else if (cmd->kind == CMD_IF) {
    if(bool_eval(cmd->attr.cond.condition,vars)){
      Node* tmp = vars->top;
      eval(cmd->attr.cond.cmd,vars);
      vars->top = tmp;
    }
    eval(cmd->attr.cond.cmd_next,vars);
  }
  else if (cmd->kind == CMD_IF_ELSE) {
    if(bool_eval(cmd->attr.cond_else.condition,vars)){
      Node* tmp = vars->top;
      eval(cmd->attr.cond_else.cmd_then,vars);
      vars->top = tmp;
    } else {
      Node* tmp = vars->top;
      eval(cmd->attr.cond_else.cmd_else,vars);
      vars->top = tmp;
    }
    eval(cmd->attr.cond_else.cmd_next,vars);
  }
  else if (cmd->kind == CMD_WHILE) {
    Node* tmp = vars->top;
    while (bool_eval(cmd->attr.loop.condition,vars))
      eval(cmd->attr.loop.cmd,vars);
    vars->top = tmp;
    eval(cmd->attr.loop.cmd_next,vars);
  } 
  else if (cmd->kind == CMD_LIST){
    eval(cmd->attr.list.head,vars);
    eval(cmd->attr.list.rest,vars);
  }
  else if (cmd->kind == CMD_READ){
    Node* v = contains(vars,cmd->attr.read_var);
    if  (v == NULL) yyerror("Variable not declared!");
    switch (v->var.type) {
      case INTEGER:
        int i;
        scanf("%d",i);
        v->var.value->attr.arith->attr.int_value = i;
        break;
      case FLOAT:
        int f;
        scanf("%f",f);
        v->var.value->attr.arith->attr.float_value = f;
        break;
      case BOOL:
        char* tmp;
        scanf("%s",tmp);
        if(strcmp(tmp,"true"))
          v->var.value->attr.boolean->attr.boolean_value = true;
        else if (strcmp(tmp,"false"))
          v->var.value->attr.boolean->attr.boolean_value = false;
        else 
          yyerror("Type error!");
        break;
      case STRING:
        char* s;
        scanf("%s",s);
        v->var.value->attr.str = s;
        break;
      case CHAR:
        char* c;
        scanf("%c",c);
        v->var.value->attr.str = c;
        break;
      default: yyerror("Type error!");
    }
  }
  else if (cmd->kind == CMD_WRITE){
    Node* v = contains(vars,cmd->attr.write_var);
    if  (v == NULL) yyerror("Variable not declared!");
    switch (v->var.type) {
      case INTEGER:
        int i = v->var.value->attr.arith->attr.int_value;
        printf("%d",i);
        break;
      case FLOAT:
        int f = v->var.value->attr.arith->attr.float_value;
        printf("%f",f);
        break;
      case BOOL:
        bool b = v->var.value->attr.boolean->attr.boolean_value;
        scanf("%s",b);
        break;
      case STRING:
        char* s = v->var.value->attr.str;
        printf("%s",s);
        break;
      case CHAR:
        char* c = v->var.value->attr.str;
        printf("%c",c);
        break;
      default: yyerror("Type error!");
    }
  }
}

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) {
    Stack* vars = (Stack*) malloc(sizeof(Stack));
    initialize(vars);
    eval(root,vars);
    printCmd(root);
    printf("\n");
  }
  return 0;


}
