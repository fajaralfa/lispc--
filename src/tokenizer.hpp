#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace tokenizer {

enum TokenType {
  TOKEN_SYMBOL,
  TOKEN_NUMBER,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_STRING,
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
  std::string source;
  std::vector<Token> tokens;
  size_t start;
  TokenPos pos;
};

void tokenize(Tokenizer &t);
void print_tokens(Tokenizer &t);

}

#endif