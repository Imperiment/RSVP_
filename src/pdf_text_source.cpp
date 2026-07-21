#include "pacer/pdf_text_source.h"
#include <algorithm>
#include <memory>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <stdexcept>

PdfTextSource::PdfTextSource(std::string filePath) : m_filePath(std::move(filePath)) {
}

namespace {
std::unique_ptr<poppler::document> loadDocument(const std::string &filePath) {
    std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(filePath));
    if (!doc) {
        throw std::runtime_error("Could not open PDF: " + filePath);
    }
    if (doc->is_locked()) {
        throw std::runtime_error("PDF is password-protected: " + filePath);
    }
    return doc;
}
} // namespace

std::string PdfTextSource::extractText() const {
    return extractTextFromPage(0);
}

std::optional<int> PdfTextSource::pageCount() const {
    auto doc = loadDocument(m_filePath);
    return doc->pages();
}

std::string PdfTextSource::extractTextFromPage(int startPage) const {
    auto doc = loadDocument(m_filePath);

    const int pageCount = doc->pages();
    const int firstPage = std::clamp(startPage, 0, pageCount > 0 ? pageCount - 1 : 0);

    std::string fullText;

    for (int i = firstPage; i < pageCount; ++i) {
        std::unique_ptr<poppler::page> pg(doc->create_page(i));
        if (!pg) {
            continue;
        }

        poppler::byte_array bytes = pg->text().to_utf8();
        fullText.append(bytes.begin(), bytes.end());
        fullText.append("\n");
    }

    return fullText;
}
