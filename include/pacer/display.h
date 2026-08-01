#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

enum class UserAction : std::uint8_t {
    None,
    Pause,
    Resume,
    Quit,
    SpeedUp,
    SlowDown,
    Rewind,
    FastForward,
    ToggleContext
};

struct WordDisplayState {
    std::string word;
    std::string previousWord;
    std::string nextWord;
    std::size_t currentIndex = 0;
    std::size_t totalWords = 0;
    int wordsPerMinute = 0;
    bool paused = false;
};

class Display {
  public:
    virtual ~Display() = default;

    virtual void init() = 0;
    virtual void shutdown() = 0;

    virtual void showWord(const WordDisplayState &state) = 0;
    virtual UserAction pollInput() = 0;
};
