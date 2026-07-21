#include "pacer/cli.h"
#include "pacer/progress_store.h"
#include "pacer/session.h"
#include "pacer/terminal_display.h"
#include "pacer/text_source.h"
#include <filesystem>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
    try {
        CliOptions options = Cli::parse(argc, argv);

        const std::string absolutePath = std::filesystem::absolute(options.filePath).string();

        auto source = TextSource::create(options.filePath);

        ProgressStore progressStore;
        std::size_t startIndex = 0;

        if (options.startPage) {
            if (!source->pageCount()) {
                std::cerr << "Warning: --page is ignored for non-PDF files.\n";
            } else {
                const std::string pageText = source->extractTextFromPage(*options.startPage - 1);
                source = TextSource::create(options.filePath);
            }
        } else if (options.resume) {
            if (auto saved = progressStore.load(absolutePath)) {
                startIndex = saved->wordIndex;
                options.wordsPerMinute = saved->wordsPerMinute;
            }
        }

        auto display = std::make_unique<TerminalDisplay>();
        Session session(std::move(source), std::move(display), options.wordsPerMinute, startIndex);
        session.run();

        const std::size_t finalIndex = session.currentWordIndex();
        const std::size_t total = session.totalWords();

        if (total > 0 && finalIndex + 1 >= total) {
            progressStore.clear(absolutePath);
        } else {
            progressStore.save(absolutePath, {finalIndex, options.wordsPerMinute});
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
