#pragma once
#include <optional>
#include <string>

constexpr int DEFAULT_WPM = 300;

struct CliOptions {
    std::string filePath;
    int wordsPerMinute = DEFAULT_WPM;
    bool startPaused = false;
    std::optional<int> startPage;
    bool resume = false;
};

class Cli {
  public:
    static CliOptions parse(int argc, char **argv);
};
