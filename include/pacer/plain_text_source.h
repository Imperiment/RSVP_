#pragma once
#include "text_source.h"

class PlainTextSource : public TextSource {
  public:
    explicit PlainTextSource(const std::string &filePath);
    [[nodiscard]] std::string extractText() const override;

  private:
    std::string m_filePath;
};
