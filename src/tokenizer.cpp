#include "pacer/tokenizer.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_set>

namespace {
bool isSentenceEnder(char punc) {
    return punc == '.' || punc == '!' || punc == '?';
}
bool isClauseEnder(char punc) {
    return punc == ',' || punc == ';' || punc == ':';
}

bool isTrailingWrapper(char punc) {
    return punc == '"' || punc == '\'' || punc == ')' || punc == ']' || punc == '}';
}

char lastMeaningfulChar(const std::string &word) {
    for (auto it = word.rbegin(); it != word.rend(); ++it) {
        if (!isTrailingWrapper(*it)) {
            return *it;
        }
    }
    return '\0';
}

std::string toLower(const std::string &s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool isKnownAbbreviation(const std::string &word) {
    static const std::unordered_set<std::string> kAbbreviations = {
        "mr.",  "mrs.", "ms.",  "dr.",  "prof.", "sr.",  "jr.", "st.",  "vs.", "etc.",
        "e.g.", "i.e.", "a.m.", "p.m.", "u.s.",  "u.k.", "no.", "vol.", "fig."};
    return kAbbreviations.count(toLower(word)) > 0;
}

Token makeToken(const std::string &rawWord) {
    Token token;
    token.text = rawWord;

    if (!rawWord.empty() && !isKnownAbbreviation(rawWord)) {
        char last = lastMeaningfulChar(rawWord);
        token.endsSentence = isSentenceEnder(last);
        token.endsClause = isClauseEnder(last);
    }

    return token;
}

std::string dehyphenate(const std::string &text) {
    std::string result;
    result.reserve(text.size());

    for (std::size_t i = 0; i < text.size(); ++i) {
        const bool isHyphenAtLineEnd =
            text[i] == '-' && i + 1 < text.size() && text[i + 1] == '\n' && i > 0 &&
            std::isalpha(static_cast<unsigned char>(text[i - 1])) && i + 2 < text.size() &&
            std::isalpha(static_cast<unsigned char>(text[i + 2]));

        if (isHyphenAtLineEnd) {
            ++i;
            continue;
        }
        result += text[i];
    }

    return result;
}
} // namespace

std::vector<Token> Tokenizer::tokenize(const std::string &rawText) {
    std::vector<Token> tokens;

    const std::string cleaned = dehyphenate(rawText);

    std::istringstream stream(cleaned);
    std::string word;

    while (stream >> word) {
        tokens.push_back(makeToken(word));
    }

    return tokens;
}
