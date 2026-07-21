#pragma once
#include "pacer/display.h"

class TerminalDisplay : public Display {
  public:
    TerminalDisplay();
    ~TerminalDisplay() override;

    void init() override;
    void shutdown() override;
    void showWord(const WordDisplayState &state) override;

    UserAction pollInput() override;
};
