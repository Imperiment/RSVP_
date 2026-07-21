#include "../include/pacer/tokenizer.h"
#include <cctype>
#include <sstream>

namespace {
bool isSentenceEnd(char punc) {
    return punc == '.' || punc == '!' || punc == '?';
}
bool isClauseEnd(char punc) {
    return punc == ',' || punc == ';' || punc == ':';
}

// strip leading whitespaces from raw strings
Token makeToken(const std::string &rawWord) {
    Token token;
    token.text = rawWord;

    if (!rawWord.empty()) {
        char last = rawWord.back();
        token.endsSentence = isSentenceEnd(last);
        token.endsClause = isClauseEnd(last);
    }
    return token;
}
} // namespace

std::vector<Token> Tokenizer::tokenize(const std::string &rawText) {
    std::vector<Token> tokens;
    std::istringstream stream(rawText);
    std::string word;

    while (stream >> word) {
        tokens.push_back(makeToken(word));
    }

    return tokens;
}
