# Reading Pacer

A terminal-based RSVP (Rapid Serial Visual Presentation) reader. Flashes one word at a time at a configurable speed, so you can read faster than normal eye-scanning allows. Supports plain text and PDF input.

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
./pacer <file> [--speed WPM] [--paused] [--resume] [--page NUMBER] [--context]
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
| `c`            | Toggle Context             |
| `q`            | Quit                       |

## Running tests

Unit tests cover the tokenizer and pacing logic (pure functions, no I/O):

```bash
make test
```

## Known limitations

- Abbreviation detection uses a fixed list — uncommon abbreviations may be misread as sentence endings.
- Scanned (image-only) PDFs have no extractable text layer and will produce empty output; OCR is not implemented.
- Multi-column or table-heavy PDF layouts may extract in a scrambled reading order, since poppler extracts text in the PDF's internal ordering, not necessarily visual reading order.

## License

MIT — see [LICENSE](LICENSE) for details.
