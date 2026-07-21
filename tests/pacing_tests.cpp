#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pacer/pacing.h"

TEST_CASE("base delay matches wpm formula") {
    Pacing pacing(300); // 60000ms / 300 = 200ms per word
    Token plain{"word", false, false};
    CHECK(pacing.delayFor(plain).count() == 200);
}

TEST_CASE("sentence-ending tokens get a longer delay") {
    Pacing pacing(300);
    Token sentenceEnd{"word.", true, false};
    Token plain{"word", false, false};
    CHECK(pacing.delayFor(sentenceEnd).count() > pacing.delayFor(plain).count());
}

TEST_CASE("clause-ending tokens get a delay between plain and sentence-end") {
    Pacing pacing(300);
    Token clauseEnd{"word,", false, true};
    Token sentenceEnd{"word.", true, false};
    Token plain{"word", false, false};

    CHECK(pacing.delayFor(clauseEnd).count() > pacing.delayFor(plain).count());
    CHECK(pacing.delayFor(clauseEnd).count() < pacing.delayFor(sentenceEnd).count());
}

TEST_CASE("higher wpm produces shorter delays") {
    Pacing slow(150);
    Pacing fast(600);
    Token plain{"word", false, false};
    CHECK(slow.delayFor(plain).count() > fast.delayFor(plain).count());
}

TEST_CASE("setWordsPerMinute updates subsequent delays") {
    Pacing pacing(300);
    Token plain{"word", false, false};
    auto before = pacing.delayFor(plain);

    pacing.setWordsPerMinute(600);
    auto after = pacing.delayFor(plain);

    CHECK(after.count() < before.count());
}

TEST_CASE("wordsInDuration is consistent with delayFor") {
    Pacing pacing(300); // 200ms/word
    // 1000ms should fit ~5 words at 200ms each
    CHECK(pacing.wordsInDuration(std::chrono::milliseconds(1000)) == 5);
}

TEST_CASE("wordsInDuration handles zero duration") {
    Pacing pacing(300);
    CHECK(pacing.wordsInDuration(std::chrono::milliseconds(0)) == 0);
}
