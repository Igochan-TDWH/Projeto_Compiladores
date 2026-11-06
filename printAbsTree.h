// Print definitions
#ifndef __print_ast_h__
#define __print_ast_h__

#include "ast.h"

void printArith(ArithExpr* expr);
void printBool(BoolExpr* expr);
void printExpr(Expr* expr);
void printCmd(Cmd* cmd);

#endif