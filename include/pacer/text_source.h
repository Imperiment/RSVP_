#pragma once
#include <memory>
#include <optional>
#include <string>

class TextSource {
  public:
    virtual ~TextSource() = default;
    [[nodiscard]] virtual std::string extractText() const = 0;
    [[nodiscard]] virtual std::optional<int> pageCount() const { return std::nullopt; }

    [[nodiscard]] virtual std::string extractTextFromPage(int startPage) const {
        (void)startPage;
        return extractText();
    }
    static std::unique_ptr<TextSource> create(const std::string &filePath);
};
