#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pacer/tokenizer.h"

TEST_CASE("splits plain words on whitespace") {
    auto tokens = Tokenizer::tokenize("hello world foo");
    REQUIRE(tokens.size() == 3);
    CHECK(tokens[0].text == "hello");
    CHECK(tokens[1].text == "world");
    CHECK(tokens[2].text == "foo");
}

TEST_CASE("detects sentence-ending punctuation") {
    auto tokens = Tokenizer::tokenize("This is a test.");
    REQUIRE(tokens.size() == 4);
    CHECK(tokens[3].endsSentence == true);
    CHECK(tokens[3].endsClause == false);
}

TEST_CASE("detects clause-ending punctuation") {
    auto tokens = Tokenizer::tokenize("First, second.");
    REQUIRE(tokens.size() == 2);
    CHECK(tokens[0].endsClause == true);
    CHECK(tokens[0].endsSentence == false);
}

TEST_CASE("sees through trailing quotes and brackets") {
    auto tokens = Tokenizer::tokenize("She said \"hello.\" Then left.");
    REQUIRE(tokens.size() == 5);
    CHECK(tokens[2].text == "\"hello.\"");
    CHECK(tokens[2].endsSentence == true);
}

TEST_CASE("known abbreviations do not end a sentence") {
    auto tokens = Tokenizer::tokenize("Dr. Smith arrived.");
    REQUIRE(tokens.size() == 3);
    CHECK(tokens[0].text == "Dr.");
    CHECK(tokens[0].endsSentence == false);
    CHECK(tokens[2].endsSentence == true);
}

TEST_CASE("abbreviation check is case-insensitive") {
    auto tokens = Tokenizer::tokenize("dr. Smith arrived.");
    CHECK(tokens[0].endsSentence == false);
}

TEST_CASE("dehyphenates line-break-split words") {
    auto tokens = Tokenizer::tokenize("This is an exam-\nple sentence.");
    REQUIRE(tokens.size() == 5);
    CHECK(tokens[3].text == "example");
}

TEST_CASE("does not dehyphenate mid-line hyphenated words") {
    auto tokens = Tokenizer::tokenize("This is well-known information.");
    auto it = std::find_if(tokens.begin(), tokens.end(),
                           [](const Token &t) { return t.text == "well-known"; });
    CHECK(it != tokens.end());
}

TEST_CASE("empty input produces no tokens") {
    auto tokens = Tokenizer::tokenize("");
    CHECK(tokens.empty());
}
