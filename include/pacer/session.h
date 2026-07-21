#pragma once
#include "display.h"
#include "pacing.h"
#include "text_source.h"
#include "tokenizer.h"
#include <chrono>
#include <memory>
#include <vector>

class Session {
  public:
    Session(std::unique_ptr<TextSource> source, std::unique_ptr<Display> display,
            int wordsPerMinute);

    void run();

  private:
    bool handleInput(UserAction action);

    void rewind(std::chrono::milliseconds duration);
    void fastForward(std::chrono::milliseconds duration);

    std::unique_ptr<TextSource> m_source;
    std::unique_ptr<Display> m_display;
    Pacing m_pacing;

    std::vector<Token> m_token;
    std::size_t m_currentIndex = 0;
    bool m_paused = false;

    static constexpr std::chrono::milliseconds kSeekDuration{1000};
};
