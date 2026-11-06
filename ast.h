
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _ArithExpr {
  enum { 
    E_ID,
    E_INTEGER,
    E_FLOAT,
    E_OPERATION
  } kind;
  union {
    int int_value; // for integer values
    int float_value; // for float values
    char* id;
    struct { 
      int operator; // PLUS, MINUS, etc 
      struct _ArithExpr* left;
      struct _ArithExpr* right;
    } op; // for binary expressions
  } attr;
};

typedef struct _ArithExpr ArithExpr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
ArithExpr* ast_id(char* name);
ArithExpr* ast_integer(int v);
ArithExpr* ast_float(float v);
ArithExpr* ast_operation(int operator, ArithExpr* left, ArithExpr* right);

// AST for boolean expressions
struct _BoolExpr {
  enum { 
    E_BOOL_VAL,
    E_BOOL_ID,
    E_BOOL_EXPRETION,
    E_BOOL_NEGATION,
    E_BOOL_RELATIONAL,
    E_BOOL_OPERATION
  } kind;
  union {
    bool boolean_value;
    struct _ArithExpr* exp;
    struct _BoolExpr* neg;
    char* id;
    struct {
      int operator; // AND, OR
      struct _BoolExpr* left;
      struct _BoolExpr* right;
    } op;
    struct { 
      int operator; // LESS, EQUAL etc 
      struct _ArithExpr* left;
      struct _ArithExpr* right;
    } rel; // for relational expressions
  } attr;
};

typedef struct _BoolExpr BoolExpr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
BoolExpr* boolean_ast_value(bool v);
BoolExpr* boolean_ast_id(char* name);
BoolExpr* boolean_ast_neg(BoolExpr* v);
BoolExpr* boolean_ast_operation(int operator, BoolExpr* left, BoolExpr* right);
BoolExpr* boolean_ast_relational(int operator, ArithExpr* left, ArithExpr* right);

// AST for expressions
struct _Expr {
  enum { 
    E_ARITH,
    E_BOOL,
    E_STR,
    E_CHAR
  } kind;
  union {
    struct _ArithExpr* arith;
    struct _BoolExpr* boolean;
    char* str;
  } attr;
};

typedef struct _Expr Expr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Expr* expr_ast_arith(ArithExpr* v);
Expr* expr_ast_bool(BoolExpr* v);

// AST for commands
struct _Cmd {
  enum { 
    CMD_EXPR,
    CMD_ASSIGN,
    CMD_DECLARE,
    CMD_LIST,
    CMD_IF,
    CMD_IF_ELSE,
    CMD_WHILE,
    CMD_READ,
    CMD_WRITE
  } kind;
  union {
    struct _Expr* expr; // for arithimetic expressions
    struct { 
      char* var;
      int type;
      struct _Expr* value;
    } declare; // for variable declaration
    struct { 
      char* var;
      struct _Expr* value;
    } assign; // for variable assigment
    struct { 
      struct _Cmd* head;
      struct _Cmd* rest;
    } list; // for list of commands
    struct { 
      struct _BoolExpr* condition;
      struct _Cmd* cmd;
      struct _Cmd* cmd_next;
    } cond; // for if then conditionals
    struct {
      struct _BoolExpr* condition;
      struct _Cmd* cmd_then;
      struct _Cmd* cmd_else;
      struct _Cmd* cmd_next;
    } cond_else; // for if then else conditionals
    struct {
      struct _BoolExpr* condition;
      struct _Cmd* cmd;
      struct _Cmd* cmd_next;
    } loop; // for while loops
    char* read_var;
    char* write_var;
  } attr;
};

typedef struct _Cmd Cmd; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Cmd* cmd_ast_expr(Expr* v);
Cmd* cmd_ast_declare(char* var,int type, Expr* value);
Cmd* cmd_ast_assigment(char* var, Expr* value);
Cmd* cmd_ast_conditional(BoolExpr* condition,Cmd* cmd, Cmd* cmd_next);
Cmd* cmd_ast_conditional_else(BoolExpr* condition,Cmd* cmd_then, Cmd* cmd_else, Cmd* cmd_next);
Cmd* cmd_ast_list(Cmd* head, Cmd* rest);
Cmd* cmd_ast_loop(BoolExpr* condition, Cmd* cmd, Cmd* cmd_next);
Cmd* cmd_ast_read(char* var);
Cmd* cmd_ast_write(char* var);

#endif
