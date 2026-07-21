cmake_minimum_required(VERSION 3.14)
project(reading_pacer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUTILS/CLI11
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(CLI11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER REQUIRED poppler-cpp)
find_package(Curses REQUIRED)

add_executable(pacer
    src/main.cpp
    src/cli.cpp
    src/plain_text_source.cpp
    src/pdf_text_source.cpp
    src/tokenizer.cpp
    src/pacing.cpp
    src/terminal_display.cpp
)

target_include_directories(pacer PRIVATE
    include
    ${CURSES_INCLUDE_DIR}
    ${POPPLER_INCLUDE_DIRS}
)

target_link_libraries(pacer PRIVATE
    CLI11::CLI11
    ${CURSES_LIBRARIES}
    ${POPPLER_LIBRARIES}
)
