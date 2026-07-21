#include "pacer/plain_text_source.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

PlainTextSource::PlainTextSource(std::string filePath) : m_filePath(std::move(filePath)) {
}

std::string PlainTextSource::extractText() const {
    std::ifstream file(m_filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + m_filePath);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
