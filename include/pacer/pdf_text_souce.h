#pragma once
#include "text_source.h"

class PdfTextSource : public TextSource {
  public:
    explicit PdfTextSource(const std::string &filePath);

    [[nodiscard]] std::string extractText() const override;

  private:
    std::string m_filePath;
};
