#pragma once
#include "pacer/tokenizer.h"
#include <chrono>

class Pacing {
  public:
    explicit Pacing(int wordsPerMinute);

    [[nodiscard]] std::chrono::milliseconds delayFor(const Token &token) const;

    [[nodiscard]] std::size_t wordsInDuration(std::chrono::milliseconds duration) const;

    void setWordsPerMinute(int wpm);
    [[nodiscard]] int wordsPerMinute() const;

  private:
    int m_wpm;
};
