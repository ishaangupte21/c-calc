#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#ifndef CALC_H
#define CALC_H

typedef enum _token {
  INTEGER,
  ADD ,
  SUB,
  MUL,
  DIV,
  LPAREN,
  RPAREN,
  ABS_BAR,
  EXP,
  SIN,
  COS,
  TAN,
  LOG,
  END
} Token;

typedef enum {
  OP, INT
} TreeType;

typedef struct _ast AST;
struct _ast {
  TreeType type;
  union {
    Token op;
    int i;
  } val;
  AST *left, *right;
};

AST *create_node(TreeType type);

Token lex(void);
int get_precedence(Token tok);

// parsing methods
AST *parse_primary(void);
AST *parse_paren(void);
AST *parse_expr(void);
AST *parse_rhs(AST *lhs, int min_prec);
AST *parse_abs(void);
AST *parse_negative(void);
AST *parse_sin(void);
AST *parse_cos(void);
AST *parse_tan(void);
AST *parse_log(void);

double deg_2_rad(int deg);
void next_tok(void);

int eval_tree(AST *tree);

#endif