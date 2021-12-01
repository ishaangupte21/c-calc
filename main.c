#include "calc.h"

static char *lval;
static Token current_tok;

int get_precedence(Token tok) {
  switch(tok) {
    case ADD: return 5;
    case SUB: return 5;
    case MUL: return 10;
    case DIV: return 10;
    default: return -1;
  }
}

Token lex(void) {
  char c = ' ', *buffer = NULL;
  size_t buflen = 0;
  while(isspace(c)) {
    if(c == '\n') {
      return END;
    }

    c = getchar();
  }

  if(c == '+') {
    return ADD;
  }
  if(c == '-') {
    return SUB;
  }
  if(c == '*') {
    return MUL;
  }
  if(c == '/') {
    return DIV;
  }
  if(c == '(') {
    return LPAREN;
  }
  if(c == ')') {
    return RPAREN;
  }
  if(isdigit(c)) {
    buffer = realloc(buffer, (buflen + 1) * sizeof(char));
    buffer[buflen++] = c;
    while(isdigit(c = getchar())) {
    buffer = realloc(buffer, (buflen + 1) * sizeof(char));
    buffer[buflen++] = c;
    }
    buffer = realloc(buffer, (buflen + 1) * sizeof(char));
    buffer[buflen++] = '\0';

  ungetc(c, stdin);
  lval = buffer;
  return INTEGER;
  }

  fprintf(stderr, "Unrecognized character: %c\n", c);
  exit(1);
}

AST *create_node(TreeType type) {
 AST *node = (AST *)malloc(sizeof(AST)); 
 node->type = type;

 return node;
}

AST *parse_primary(void) {
  if(current_tok == INTEGER) {
    AST *node = create_node(INT); 
    node->val.i = atoi(lval);
    // eat int token
    next_tok();
    return node;
  }
  if(current_tok == LPAREN) {
   return parse_paren();
  }
  return NULL;
}

AST *parse_paren(void) {
  // eat lparen
  next_tok();
  AST *v = parse_expr();
  if(!v) {
    fprintf(stderr, "Syntax error: expected expression\n");
    exit(1);
  }
  if(current_tok != RPAREN) {
    fprintf(stderr, "Expected '('\n");
    exit(1);
  }
  //eat rparen
  next_tok();
  return v;
}

AST *parse_expr(void) {
  AST *lhs = parse_primary();
  if(!lhs) {
    return NULL;
  }
  return parse_rhs(lhs, 0);
}

AST *parse_rhs(AST *lhs, int min_prec) {
  Token peek = current_tok;
  while(get_precedence(peek) >= min_prec) {
    Token op = current_tok;
    next_tok();
    AST *rhs = parse_primary();
    if(rhs == NULL) {
      return NULL;
    }
    peek = current_tok;
    while(get_precedence(peek) > get_precedence(op)) {
      AST *rhs = parse_rhs(rhs, get_precedence(op) + 5);
      if(rhs == NULL) {
        return NULL;
      }
      peek = current_tok;
    }
    AST *new_node = create_node(OP);
    new_node->val.op = op;
    new_node->left = lhs;
    new_node->right = rhs;
    lhs = new_node; 
  }
  return lhs;
}

void next_tok(void) {
  current_tok = lex();
}

int eval_tree(AST *tree) {
  if(!tree) {
    return 0;
  }
  if(tree->type == INT) {
    return tree->val.i;
  }
  int lhs = eval_tree(tree->left);
  int rhs = eval_tree(tree->right);

  switch(tree->val.op) {
    case ADD: return lhs + rhs;
    case SUB: return lhs - rhs;
    case MUL: return lhs * rhs;
    case DIV: return lhs / rhs;
    default: {
      fprintf(stderr, "Unknown operator during evaluation");
      exit(1);
    }
  }
}




int main(void) {
  printf("Enter expression\n> ");
  next_tok();
  AST *tree = parse_expr();
  if(!tree) {
    fprintf(stderr, "Syntax Error: expected expression\n");
    exit(1);
  }
  if(current_tok != END) {
    fprintf(stderr, "Syntax Error: expected end of input\n");
    exit(1);
  }
  
  int result = eval_tree(tree); 

  printf("%d\n", result);
}