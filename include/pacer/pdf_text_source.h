#pragma once
#include "pacer/text_source.h"

class PdfTextSource : public TextSource {
  public:
    explicit PdfTextSource(std::string filePath);

    [[nodiscard]] std::string extractText() const override;
    [[nodiscard]] std::optional<int> pageCount() const override;
    [[nodiscard]] std::string extractTextFromPage(int startPage) const override;

  private:
    std::string m_filePath;
};
