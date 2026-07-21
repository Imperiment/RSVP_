#pragma once
#include <string>

constexpr int DEFAULT_WPM = 300;

struct CliOptions {
    std::string filePath;
    int wordsPerMinute = DEFAULT_WPM;
    bool startPaused = false;
};

class Cli {
  public:
    static CliOptions parse(int argc, char **argv);
};
