#include "pacer/cli.h"
#include <CLI/CLI.hpp>

CliOptions Cli::parse(int argc, char **argv) {
    CliOptions options;

    CLI::App app{"Reading pacer - flashes words at a target speed"};

    app.add_option("file", options.filePath, "Path to the text file to read")->required();

    app.add_option("-s,--speed", options.wordsPerMinute, "Words per minute")
        ->default_val(DEFAULT_WPM);

    app.add_flag("-p,--paused", options.startPaused, "Start paused");

    int page = 0;
    app.add_option("--page", page, "Start from this page number (PDF only, 1-indexed)");

    app.add_flag("--resume", options.resume, "Resume from saved progress for this file");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::exit(app.exit(e));
    }

    if (page > 0) {
        options.startPage = page;
    }

    return options;
}
