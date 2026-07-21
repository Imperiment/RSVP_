#include "pacer/cli.h"
#include <CLI/CLI.hpp>

CliOptions Cli::parse(int argc, char **argv) {
    CliOptions options;

    CLI::App app{"Reading pacer - flashes words at a target speed"};

    app.add_option("file", options.filePath, "Path to the text file to read")->required();

    app.add_option("-s,--speed", options.wordsPerMinute, "Words per minute")->default_val(300);

    app.add_flag("-p,--paused", options.startPaused, "Start paused");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::exit(app.exit(e));
    }

    return options;
}
