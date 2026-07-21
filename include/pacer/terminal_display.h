#pragma once
#include "pacer/display.h"

class TerminalDisplay : public Display {
  public:
    TerminalDisplay();
    ~TerminalDisplay() override;

    void init() override;
    void shutdown() override;
    void showWord(const std::string &word) override;

    UserAction pollInput() override;
};
