#include "pacer/progress_store.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace {
std::string defaultStorePath() {
    namespace fs = std::filesystem;

    fs::path base;
    if (const char *xdgData = std::getenv("XDG_DATA_HOME")) {
        base = xdgData;
    } else if (const char *home = std::getenv("HOME")) {
        base = home;
    } else {
        base = ".";
    }

    fs::path dir = base / "reading-pacer";
    std::error_code ec;
    fs::create_directories(dir, ec);

    return (dir / "progress.tsv").string();
}

struct Entry {
    std::string path;
    std::size_t wordIndex;
    int wordsPerMinute;
};

std::vector<Entry> readAllEntries(const std::string &storePath) {
    std::vector<Entry> entries;
    std::ifstream file(storePath);
    if (!file.is_open()) {
        return entries;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream lineStream(line);
        Entry entry;
        std::string wordIndexStr;
        std::string wpmStr;

        if (std::getline(lineStream, entry.path, '\t') &&
            std::getline(lineStream, wordIndexStr, '\t') &&
            std::getline(lineStream, wpmStr, '\t')) {
            entry.wordIndex = std::stoull(wordIndexStr);
            entry.wordsPerMinute = std::stoi(wpmStr);
            entries.push_back(entry);
        }
    }
    return entries;
}

void writeAllEntries(const std::string &storePath, const std::vector<Entry> &entries) {
    std::ofstream file(storePath, std::ios::trunc);
    for (const auto &entry : entries) {
        file << entry.path << '\t' << entry.wordIndex << '\t' << entry.wordsPerMinute << '\n';
    }
}
} // namespace

ProgressStore::ProgressStore() : m_storePath(defaultStorePath()) {
}
ProgressStore::ProgressStore(std::string storePath) : m_storePath(std::move(storePath)) {
}

std::optional<Progress> ProgressStore::load(const std::string &absoluteFilePath) const {
    auto entries = readAllEntries(m_storePath);
    for (const auto &entry : entries) {
        if (entry.path == absoluteFilePath) {
            return Progress{entry.wordIndex, entry.wordsPerMinute};
        }
    }
    return std::nullopt;
}

void ProgressStore::save(const std::string &absoluteFilePath, const Progress &progress) {
    auto entries = readAllEntries(m_storePath);

    bool found = false;
    for (auto &entry : entries) {
        if (entry.path == absoluteFilePath) {
            entry.wordIndex = progress.wordIndex;
            entry.wordsPerMinute = progress.wordsPerMinute;
            found = true;
            break;
        }
    }
    if (!found) {
        entries.push_back({absoluteFilePath, progress.wordIndex, progress.wordsPerMinute});
    }

    writeAllEntries(m_storePath, entries);
}

void ProgressStore::clear(const std::string &absoluteFilePath) {
    auto entries = readAllEntries(m_storePath);

    entries.erase(std::remove_if(entries.begin(), entries.end(),
                                 [&](const Entry &e) { return e.path == absoluteFilePath; }),
                  entries.end());

    writeAllEntries(m_storePath, entries);
}
