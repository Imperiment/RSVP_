#pragma once
#include <string>
#include <vector>

struct Token {
  std::string text;
  bool endsSentence = false;
  bool endsClause = false;
};

class Tokenizer {
public:
  static std::vector<Token> tokenize(const std::string &rawText);
};
