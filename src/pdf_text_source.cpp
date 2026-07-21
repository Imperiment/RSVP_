#include "pacer/pdf_text_source.h"
#include <memory>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <stdexcept>

PdfTextSource::PdfTextSource(std::string filePath) : m_filePath(std::move(filePath)) {
}

std::string PdfTextSource::extractText() const {
    std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(m_filePath));

    if (!doc) {
        throw std::runtime_error("Could not open PDF: " + m_filePath);
    }

    if (doc->is_locked()) {
        throw std::runtime_error("PDF is password-protected: " + m_filePath);
    }

    std::string fullText;
    const int pageCount = doc->pages();

    for (int i = 0; i < pageCount; ++i) {
        std::unique_ptr<poppler::page> pg(doc->create_page(i));
        // skips unreadable passages
        if (!pg) {
            continue;
        }

        poppler::byte_array bytes = pg->text().to_utf8();
        fullText.append(bytes.begin(), bytes.end());
        fullText.append("\n");
    }

    return fullText;
}
