# Reading Pacer

A terminal-based RSVP (Rapid Serial Visual Presentation) reader. Flashes one word at a time at a configurable speed, so you can read faster than normal eye-scanning allows. Supports plain text and PDF input.

## Features

- Configurable reading speed (words per minute)
- Automatic pacing adjustments for sentence/clause punctuation
- Pause / resume
- Time-based rewind and fast-forward (jump back/forward ~1 second's worth of words)
- Live speed adjustment while reading
- PDF and plain text (`.txt`, `.md`) support

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- `ncurses`
- `poppler-cpp` (PDF text extraction)
- `pkg-config`

### Arch Linux

```bash
sudo pacman -S ncurses poppler
```

### Debian / Ubuntu

```bash
sudo apt install libncurses-dev libpoppler-cpp-dev
```

## Building

This project uses a plain `Makefile` — no CMake required.

```bash
git clone <your-repo-url>
cd reading-pacer

# Fetch header-only dependencies (one-time setup)
git clone --depth 1 --branch v2.4.2 https://github.com/CLIUtils/CLI11 third_party/CLI11
mkdir -p third_party/doctest
curl -L https://raw.githubusercontent.com/doctest/doctest/v2.4.11/doctest/doctest.h \
     -o third_party/doctest/doctest.h

make
```

This produces a `pacer` binary in the project root.

## Usage

```bash
./pacer <file> [--speed WPM] [--paused]
```

**Examples:**

```bash
./pacer book.txt --speed 300
./pacer document.pdf --speed 450
```

### Controls

| Key            | Action                     |
|----------------|----------------------------|
| `space`        | Pause / resume             |
| `h` / `←`      | Rewind ~1 second           |
| `l` / `→`      | Fast-forward ~1 second     |
| `j` / `-`      | Slow down (−25 WPM)        |
| `k` / `+`      | Speed up (+25 WPM)         |
| `q`            | Quit                       |

## Running tests

Unit tests cover the tokenizer and pacing logic (pure functions, no I/O):

```bash
make test
```

## Project structure

```
reading-pacer/
├── include/pacer/       # Public headers / interfaces
├── src/                 # Implementation
├── tests/               # Unit tests (doctest)
├── third_party/         # Fetched header-only dependencies (CLI11, doctest)
└── Makefile
```

### Architecture

The pipeline is: **file → raw text → tokens → paced display loop**.

- **`TextSource`** — abstract interface for extracting raw text from a file. Implementations: `PlainTextSource` (.txt/.md), `PdfTextSource` (.pdf, via poppler). Chosen automatically by file extension.
- **`Tokenizer`** — splits raw text into `Token`s (word + punctuation metadata). Handles quoted/bracketed punctuation, common abbreviations (`Dr.`, `etc.`), and PDF line-break hyphenation (`exam-\nple` → `example`).
- **`Pacing`** — pure logic converting words-per-minute into per-word delays, with longer pauses after sentence/clause-ending punctuation.
- **`Display`** — abstract interface for rendering one word and polling input. Implementation: `TerminalDisplay` (ncurses).
- **`Session`** — owns the main loop: pulls tokens, asks `Pacing` for timing, asks `Display` to render, and handles pause/rewind/speed input.

Each interface is designed to be swapped independently — e.g., a future GUI display or an EPUB text source would only require a new implementation of `Display`/`TextSource`, with no changes elsewhere.

## Known limitations

- Abbreviation detection uses a fixed list — uncommon abbreviations may be misread as sentence endings.
- Scanned (image-only) PDFs have no extractable text layer and will produce empty output; OCR is not implemented.
- Multi-column or table-heavy PDF layouts may extract in a scrambled reading order, since poppler extracts text in the PDF's internal ordering, not necessarily visual reading order.

## License

MIT — see [LICENSE](LICENSE) for details.
