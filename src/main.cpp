#include "pacer/cli.h"
#include "pacer/session.h"
#include "pacer/terminal_display.h"
#include "pacer/text_source.h"
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
    try {
        CliOptions options = Cli::parse(argc, argv);

        auto source = TextSource::create(options.filePath);
        auto display = std::make_unique<TerminalDisplay>();

        Session session(std::move(source), std::move(display), options.wordsPerMinute);
        session.run();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
