#include "tokenizer.hpp"
#include <iostream>
#include <string>

static std::string get_token_label(tokenizer::TokenType type) {
  switch (type) {
    case tokenizer::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
    case tokenizer::TOKEN_NUMBER: return "TOKEN_NUMBER";
    case tokenizer::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
    case tokenizer::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
    case tokenizer::TOKEN_DOUBLE_QUOTE: return "TOKEN_DOUBLE_QUOTE";
    case tokenizer::TOKEN_PLUS: return "TOKEN_PLUS";
    case tokenizer::TOKEN_MINUS: return "TOKEN_MINUS";
    case tokenizer::TOKEN_STAR: return "TOKEN_STAR";
    case tokenizer::TOKEN_SLASH: return "TOKEN_SLASH";
    case tokenizer::TOKEN_EQ: return "TOKEN_EQ";
    case tokenizer::TOKEN_LESSER: return "TOKEN_LESSER";
    case tokenizer::TOKEN_LESSER_EQ: return "TOKEN_LESSER_EQ";
    case tokenizer::TOKEN_GREATER: return "TOKEN_GREATER";
    case tokenizer::TOKEN_GREATER_EQ: return "TOKEN_GREATER_EQ";
    default: return "";
  }
}

void tokenizer::printTokens(tokenizer::Tokenizer &t) {
  for (std::size_t i { 0 }; i < t.tokens.size(); i++) {
    Token tok = t.tokens[i];
    std::cout << "range[" << tok.from << ":" << tok.to << "]\tpos[" << tok.pos.ln << ":" << tok.pos.col << "]\t" << get_token_label(tok.type) << "\n";
  }
}

static bool is_numeric(char c) {
  return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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

static void tokenize_identifier(tokenizer::Tokenizer &t) {
  size_t start{t.start};
  char c{t.source[t.start]};
  tokenizer::TokenPos pos = t.pos;
  while (is_alpha(c) || is_numeric(c)) {
    t.pos.col++;
    t.start++;
    c = t.source[t.start];
  }
  t.tokens.push_back(tokenizer::Token{tokenizer::TOKEN_IDENTIFIER, start, t.start, pos});
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
    case '+': {
      t.tokens.push_back(tokenizer::Token{TOKEN_PLUS, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '-': {
      t.tokens.push_back(tokenizer::Token{TOKEN_MINUS, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '*': {
      t.tokens.push_back(tokenizer::Token{TOKEN_STAR, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '/': {
      t.tokens.push_back(tokenizer::Token{TOKEN_SLASH, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '=': {
      t.tokens.push_back(tokenizer::Token{TOKEN_EQ, t.start, t.start + 1, t.pos});
      t.pos.col++;
      t.start++;
      break;
    }
    case '<': {
      if (t.source[t.start + 1] == '=') {
        t.tokens.push_back(tokenizer::Token{TOKEN_LESSER_EQ, t.start, t.start + 2, t.pos});
        t.pos.col += 2;
        t.start += 2;
      } else {
        t.tokens.push_back(tokenizer::Token{TOKEN_LESSER, t.start, t.start + 1, t.pos});
        t.pos.col++;
        t.start++;
      }
      break;
    }
    case '>': {
      if (t.source[t.start + 1] == '=') {
        t.tokens.push_back(tokenizer::Token{TOKEN_GREATER_EQ, t.start, t.start + 2, t.pos});
        t.pos.col += 2;
        t.start += 2;
      } else {
        t.tokens.push_back(tokenizer::Token{TOKEN_GREATER, t.start, t.start + 1, t.pos});
        t.pos.col++;
        t.start++;
      }
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
    default:
      if (is_numeric(c)) {
        tokenize_number(t);
      } else if (is_alpha(c)) {
        tokenize_identifier(t);
      } else {
        fprintf(stderr, "Invalid token at %ld:%ld\n", t.pos.ln, t.pos.col);
        exit(EXIT_FAILURE);
      }
    }
  }
}

