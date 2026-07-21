#pragma once
#include <memory>
#include <string>
#include <vector>

class TextSource {
  public:
    virtual ~TextSource() = default;
    [[nodiscard]] virtual std::string extractText() const = 0;
    static std::unique_ptr<TextSource> create(const std::string &filePath);
};
