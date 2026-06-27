#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

std::string load_file_fast(const std::string& path);

int main(void) {
  std::string filename{"examples/example.lisp"};
  std::string source = load_file_fast(filename);

  tokenizer::Tokenizer tokenizerData{
      .source = source,
      .tokens = {},
      .start = 0,
      .pos = {1, 1},
  };
  tokenizer::tokenize(tokenizerData);
  tokenizer::printTokens(tokenizerData);

  return 0;
}

std::string load_file_fast(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";

    // Get file size
    auto size = std::filesystem::file_size(path);
    
    // Pre-allocate string
    std::string content(size, '\0');
    
    // Read directly into the string's buffer
    file.read(content.data(), size);
    
    return content;
}