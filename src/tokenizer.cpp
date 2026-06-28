#include "tokenizer.hpp"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

static std::string get_token_label(tokenizer::TokenType type) {
  switch (type) {
    case tokenizer::TOKEN_SYMBOL: return "TOKEN_IDENTIFIER";
    case tokenizer::TOKEN_NUMBER: return "TOKEN_NUMBER";
    case tokenizer::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
    case tokenizer::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
    default: return "";
  }
}

void tokenizer::print_tokens(tokenizer::Tokenizer &t) {
  for (std::size_t i { 0 }; i < t.tokens.size(); i++) {
    Token tok = t.tokens[i];
    std::cout << "range=[" << tok.from << "-" << tok.to << "]\tpos=["
              << tok.pos.ln << ":" << tok.pos.col
              << "]\t" << get_token_label(tok.type) << "('"
              << t.source.substr(tok.from, (tok.to - tok.from)) << "')\n";
  }
}

static bool is_numeric(char c) {
  return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_special(char c) {
  const char special[]{'+', '-', '*', '/', '=', '>', '<'};
  for (char i = 0; i < std::size(special); i++) {
    if (c == special[i]) {
      return true;
    }
  }
  return false;
}

static void tokenize_number(tokenizer::Tokenizer &t) {
  size_t start = t.start;
  char c = t.source[t.start];
  tokenizer::TokenPos pos = t.pos;
  while (is_numeric(c)) {
    t.start++;
    t.pos.col++;
    c = t.source[t.start];
  }
  if (c == '.') {
    t.start++;
    t.pos.col++;
    c = t.source[t.start];
    while (is_numeric(c)) {
      t.start++;
      t.pos.col++;
      c = t.source[t.start];
    }
  }
  t.tokens.push_back(tokenizer::Token{tokenizer::TOKEN_NUMBER, start, t.start, pos});
}

static void tokenize_symbol(tokenizer::Tokenizer &t) {
  size_t start{t.start};
  char c{t.source[t.start]};
  tokenizer::TokenPos pos = t.pos;
  while (is_alpha(c) || is_numeric(c) || is_special(c)) {
    t.pos.col++;
    t.start++;
    c = t.source[t.start];
  }
  t.tokens.push_back(tokenizer::Token{tokenizer::TOKEN_SYMBOL, start, t.start, pos});
}

void tokenizer::tokenize(tokenizer::Tokenizer &t) {
  size_t len {t.source.size()};
  while (t.start < len) {
    char c = t.source[t.start];
    switch (c) {
    case '(': {
      t.tokens.push_back(tokenizer::Token{tokenizer::TOKEN_LEFT_BRACE, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case ')': {
      t.tokens.push_back(tokenizer::Token{TOKEN_RIGHT_BRACE, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '\n':
      t.pos.col = 1;
      t.pos.ln++;
      t.start++;
      break;
    case ' ':
    case '\t':
      t.pos.col++;
      t.start++;
      break;
    case '"':
      std::cerr << "String is unimplemented " << t.pos.ln << ":" << t.pos.col << "\n";
      exit(EXIT_FAILURE);
    default:
      if (is_numeric(c)) {
        tokenize_number(t);
      } else if (is_alpha(c) || is_special(c)) {
        tokenize_symbol(t);
      } else {
        std::cerr << "Invalid token at " << t.pos.ln << ":" << t.pos.col << "\n";
        exit(EXIT_FAILURE);
      }
    }
  }
}

