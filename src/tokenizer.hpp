#pragma once

#include <cstddef>
#include <string>

namespace Tokenizer {

enum TokenType {
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_DOUBLE_QUOTE,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_EQ,
  TOKEN_LESSER,
  TOKEN_LESSER_EQ,
  TOKEN_GREATER,
  TOKEN_GREATER_EQ,
};

struct TokenPos {
  std::size_t ln;
  std::size_t col;
};

struct Token {
  TokenType type;
  size_t from;
  size_t to;
  TokenPos pos;
};

struct Tokenizer {
  char *source;
  Token *tokens;
  size_t size;
  size_t capacity;
  size_t start;
  TokenPos pos;
};

void tokenize(Tokenizer *t);
void tokenizer_init(Tokenizer *t, std::string *source);
void tokenizer_free(Tokenizer *t);
void tokenizer_debug(Tokenizer *t);

}
