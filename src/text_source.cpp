#include "pacer/text_source.h"
#include "pacer/plain_text_source.h"
#include <filesystem>
#include <stdexcept>

std::unique_ptr<TextSource> TextSource::create(const std::string &filePath) {
    const std::string ext = std::filesystem::path(filePath).extension().string();

    if (ext == ".txt" || ext == ".md") {
        return std::make_unique<PlainTextSource>(filePath);
    }

    throw std::runtime_error("Unsupported file type: " + ext + " (PDF support coming soon)");
}
