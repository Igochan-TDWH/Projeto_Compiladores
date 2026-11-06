#include <stdlib.h> // for malloc
#include <stdio.h> // for printf
#include "parser.h"
#include "ast.h"
#include "stack.h"


void printArith(ArithExpr* expr ) {
    if (expr == 0) {
        printf("Null expression!!");
    }
    else if (expr->kind == E_ID) {
        printf(expr->attr.id);
    }
    else if (expr->kind == E_INTEGER) { 
        printf("%d",expr->attr.int_value);
    }
    else if (expr->kind == E_FLOAT) {
        printf("%f",expr->attr.float_value);
    }  
    else if (expr->kind == E_OPERATION) {
        printf("(");
        printArith(expr->attr.op.left);
        switch (expr->attr.op.operator) {
            case PLUS: 
                printf("+");
                break;
            case SUB: 
                printf("-");
                break;
            case DIV: 
                printf("/");
                break;
            case MULT: 
                printf("*");
                break;
            case REST: 
                printf("rest");
                break;
            default: printf("Unknown operator!");
        }
        printArith(expr->attr.op.right);
        printf(")");
    }
}
void printBool(BoolExpr* expr){
    if (expr == 0) {
        printf("Null expression!!");
    }
    else if (expr->kind == E_BOOL_VAL) {
        printf("%s",expr->attr.boolean_value);
    }
    else if (expr->kind == E_BOOL_ID) {
        printf(expr->attr.id);
    }
    else if (expr->kind == E_BOOL_EXPRETION) {
        printArith(expr->attr.exp); 
    } 
    else if (expr->kind == E_BOOL_NEGATION) {
        printf("not");
        printBool(expr->attr.neg); 
    } 
    else if (expr->kind == E_BOOL_RELATIONAL) {
        printf("(");
        printArith(expr->attr.rel.left);
        switch (expr->attr.rel.operator) {
            case LESS: 
                printf("<");
                break;
            case MORE: 
                printf(">");
                break;
            case EQUAL: 
                printf("=");
                break;
            case MEQUAL: 
                printf(">=");
                break;
            case LEQUAL: 
                printf("<=");
                break;
            case DIFF: 
                printf("!=");
                break;
            default: printf("Unknown operator!");
        }
        printArith(expr->attr.rel.right);
        printf(")");
    }  
        else if (expr->kind == E_BOOL_OPERATION) {
        printf("(");
        printBool(expr->attr.op.left);
        switch (expr->attr.op.operator) {
            case AND: 
                printf(" and ");
                break;
            case OR: 
                printf(" or ");
                break;
            default: printf("Unknown operator!");
        }
        printBool(expr->attr.op.right);
        printf(")");

    } 
}
void printExpr(Expr* expr){
    if (expr == 0) {
        printf("Null expression!!");
    }
    else if (expr->kind == E_BOOL) {
        printBool(expr->attr.boolean); 
    } 
    else if (expr->kind == E_ARITH) {
        printArith(expr->attr.arith); 
    } 
    else if (expr->kind == E_CHAR) {
        printf("%c",expr->attr.str);
    }
    else if (expr->kind == E_STR) {
        printf("%s",expr->attr.str);
    }
}

void printCmd(Cmd* cmd){
    if (cmd == 0) {
        printf("Null command!!");
    }
    else if (cmd->kind == CMD_EXPR) {
        printExpr(cmd->attr.expr); 
    } 
    else if (cmd->kind == CMD_ASSIGN) {
        printf("%s :",cmd->attr.assign.var);
        printExpr(cmd->attr.assign.value);
    }
    else if (cmd->kind == CMD_DECLARE) {
        printf("%s :",cmd->attr.declare.var);
        printExpr(cmd->attr.declare.value);
    }
    else if (cmd->kind == CMD_LIST) {
        printCmd(cmd->attr.list.head);
        printf(";\n");
        printCmd(cmd->attr.list.rest);
    }
    else if (cmd->kind == CMD_IF) {
        printf("if ");
        printBool(cmd->attr.cond.condition);
        printf(" then ");
        printCmd(cmd->attr.cond.cmd);
        printf("end.\n");
        printCmd(cmd->attr.cond.cmd_next);
    }
    else if (cmd->kind == CMD_IF_ELSE) {
        printf("if ");
        printBool(cmd->attr.cond_else.condition);
        printf(" then ");
        printCmd(cmd->attr.cond_else.cmd_then);
        printf(" else ");
        printCmd(cmd->attr.cond_else.cmd_else);
        printf("end.\n");
        printCmd(cmd->attr.cond_else.cmd_next);
    }
    else if (cmd->kind == CMD_WHILE) {
        printf("while ");
        printBool(cmd->attr.loop.condition);
        printf(" do\n");
        printCmd(cmd->attr.loop.cmd);
        printf("end.\n");
        printCmd(cmd->attr.loop.cmd_next);
    }
}

