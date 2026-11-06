// Tokens
%token INT  
%token FLOAT
%token LPAREN
%token RPAREN
%token PLUS
%token SUB
%token MULT
%token DIV
%token REST
%token NOT
%token AND
%token OR
%token LESS
%token MORE
%token EQUAL
%token LEQUAL
%token MEQUAL
%token DIFF
%token ID
%token BEG
%token END
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token SEMI
%token DECLARE
%token ELIP
%token INTEGER
%token REAL
%token CHAR
%token STRING
%token BOOL
%token READ
%token WRITE
%token TRUE
%token FALSE

  
// Operator associativity & precedence
%left MORE LESS LEQUAL MEQUAL
%left EQUAL DIFF
%left PLUS SUB 
%left DIV MULT REST 


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  float floatValue;
  char* strValue;
  ArithExpr* arithExpr; 
  BoolExpr* boolExpr;
  Expr* exprValue;
  Cmd* cmdValue;
}

%type <strValue> ID 
%type <floatValue> FLOAT
%type <intValue> INT INTEGER CHAR REAL STRING BOOL type
%type <arithExpr> arith
%type <boolExpr> boolean
%type <exprValue> expr
%type <cmdValue> cmd

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
extern Cmd* root;
}

%%
program: cmd { root = $1; }

expr : arith { $$ = expr_ast_arith($1);}
     |
     boolean { $$ = expr_ast_bool($1);}
     ;

arith: LPAREN arith RPAREN { $$ = $2;}
  |
  ID {
    $$ = ast_id($1);
  }
  |
  INT { 
    $$ = ast_integer($1); 
  }
  | 
  FLOAT { 
    $$ = ast_float($1); 
  }
  | 
  arith PLUS arith { 
    $$ = ast_operation(PLUS, $1, $3); 
  } 
  | 
  arith SUB arith { 
    $$ = ast_operation(SUB, $1, $3); 
  } 
  | 
  arith MULT arith { 
    $$ = ast_operation(MULT, $1, $3); 
  } 
  | 
  arith DIV arith { 
    $$ = ast_operation(DIV, $1, $3); 
  } 
  | 
  arith REST arith { 
    $$ = ast_operation(REST, $1, $3); 
  } 
  ;

boolean : LPAREN boolean RPAREN { $$ = $2;}
  |
  TRUE { boolean_ast_value(true); }
  |
  FALSE { boolean_ast_value(false); }
  |
  ID {
    $$ = boolean_ast_id($1);
  }
  |
  arith LESS arith { 
    $$ = boolean_ast_relational(LESS, $1, $3); 
  } 
  | 
  arith MORE arith { 
    $$ = boolean_ast_relational(MORE, $1, $3); 
  }
  | 
  arith EQUAL arith { 
    $$ = boolean_ast_relational(EQUAL, $1, $3); 
  }
  | 
  arith LEQUAL arith { 
    $$ = boolean_ast_relational(LEQUAL, $1, $3); 
  }
  | 
  arith MEQUAL arith { 
    $$ = boolean_ast_relational(MEQUAL, $1, $3); 
  }
  |
  arith DIFF arith { 
    $$ = boolean_ast_relational(DIFF, $1, $3); 
  }
  |
  boolean AND boolean { 
    $$ = boolean_ast_operation(AND, $1, $3); 
  }
  |
  boolean OR boolean { 
    $$ = boolean_ast_operation(OR, $1, $3); 
  }
  |
  NOT boolean { 
    $$ = boolean_ast_neg($2); 
  }
  ;

type : INTEGER { $$ = $1;}
  |
  REAL { $$ = $1;}
  |
  CHAR { $$ = $1;}
  |
  STRING { $$ = $1;}
  |
  BOOL { $$ = $1;}
  ;

cmd : expr { $$ = cmd_ast_expr($1);}
  | 
  ID ELIP type DECLARE expr
  { 
    $$ = cmd_ast_declare($1,$3,$5); 
  }
  | 
  ID ELIP expr
  { 
    $$ = cmd_ast_assigment($1,$3); 
  } 
  | 
  IF boolean THEN cmd END cmd
  { 
    $$ = cmd_ast_conditional($2, $4, $6); 
  } 
  | 
  IF boolean THEN cmd ELSE cmd END cmd
  { 
    $$ = cmd_ast_conditional_else($2, $4, $6, $8); 
  } 
  | 
  cmd SEMI cmd
  { 
    $$ = cmd_ast_list($1, $3); 
  } 
  | 
  WHILE boolean DO cmd END cmd
  { 
    $$ = cmd_ast_loop($2, $4, $6); 
  } 
  |
  BEG cmd END
  {
    $$ = $2;
  }
  |
  READ LPAREN ID RPAREN  { $$ = cmd_ast_read($3); }
  |
  WRITE LPAREN ID RPAREN { $$ = cmd_ast_write($3); }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

