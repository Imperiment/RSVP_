#pragma once
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
};

class Display {
  public:
    virtual ~Display() = default;

    virtual void init() = 0;
    virtual void shutdown() = 0;

    virtual void showWord(const std::string &word) = 0;

    virtual UserAction pollInput() = 0;
};
