#ifndef AS_LEX_H
#define AS_LEX_H
#include <lib/vec.h>

typedef enum {
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_SYMBOL,
  TOKEN_CHAR,
  TOKEN_REGISTER,
} TokenType;

typedef enum {
  REG_R0,
  REG_R1,
  REG_R2,
  REG_R3,
  REG_R4,
  REG_R5,
  REG_R6,
  REG_R7,
  REG_SP,
  REG_PC,
  REG_IVT,
  REG_PT,
  REG_PF,
} Register;

typedef struct {
  TokenType type;

  union {
    char *_string;
    unsigned int _num;
    char *_symbol;
    Register _register;
    char _char;
  };

} Token;

typedef vec_t(Token) Tokens;

Tokens lex(char *s);

#endif
