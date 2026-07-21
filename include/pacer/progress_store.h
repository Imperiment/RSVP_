#pragma once
#include <cstddef>
#include <optional>
#include <string>

struct Progress {
    std::size_t wordIndex = 0;
    int wordsPerMinute = 300;
};

class ProgressStore {
  public:
    ProgressStore();
    explicit ProgressStore(std::string storePath);

    [[nodiscard]] std::optional<Progress> load(const std::string &absoluteFilePath) const;
    void save(const std::string &absoluteFilePath, const Progress &progress);
    void clear(const std::string &absoluteFilePath);

  private:
    std::string m_storePath;
};
