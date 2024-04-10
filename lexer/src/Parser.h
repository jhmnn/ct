#ifndef PARSER_H
#define PARSER_H

enum TokenType {
  TOKEN_UNKNOWN,
  TOKEN_CLASS,
  TOKEN_INHERITS,
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_FI,
  TOKEN_WHILE,
  TOKEN_LOOP,
  TOKEN_POOL,
  TOKEN_CASE,
  TOKEN_OF,
  TOKEN_LET,
  TOKEN_IN,
  TOKEN_ISVOID,
  TOKEN_NEW,
  TOKEN_ESAC,
  TOKEN_NOT,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_IDENTIFIER_TYPE,
  TOKEN_IDENTIFIER_OBJECT,
  TOKEN_IDENTIFIER_OTHER,
  TOKEN_INTEGER,
  TOKEN_STRING,
  TOKEN_ASSIGN,
  TOKEN_SEMICOLON,
  TOKEN_COLON,
  TOKEN_DOT,
  TOKEN_COMMA,
  TOKEN_AT,
  TOKEN_ARROW_RIGHT,
  TOKEN_REGULAR_OPEN,
  TOKEN_REGULAR_CLOSE,
  TOKEN_CURLY_OPEN,
  TOKEN_CURLY_CLOSE,
  TOKEN_SQUARE_OPEN,
  TOKEN_SQUARE_CLOSE,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_TILDE,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_EQUAL
};

#endif