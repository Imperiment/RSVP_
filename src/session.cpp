#include "pacer/session.h"
#include <algorithm>
#include <thread>

constexpr int SLEEP_DURATION = 50;
constexpr int SPEED_FLOOR = 50;
constexpr int WPM_BUMP = 25;

Session::Session(std::unique_ptr<TextSource> source, std::unique_ptr<Display> display,
                 int wordsPerMinute)
    : m_source(std::move(source)), m_display(std::move(display)), m_pacing(wordsPerMinute) {
}

void Session::run() {
    const std::string rawText = m_source->extractText();
    m_tokens = Tokenizer::tokenize(rawText);

    if (m_tokens.empty()) {
        return;
    }

    m_display->init();

    while (m_currentIndex < m_tokens.size()) {
        UserAction action = m_display->pollInput();
        if (!handleInput(action)) {
            break;
        }

        const Token &token = m_tokens[m_currentIndex];

        WordDisplayState state;
        state.word = token.text;
        state.currentIndex = m_currentIndex + 1;
        state.totalWords = m_tokens.size();
        state.wordsPerMinute = m_pacing.wordsPerMinute();
        state.paused = m_paused;

        m_display->showWord(state);

        if (m_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
            continue;
        }

        std::this_thread::sleep_for(m_pacing.delayFor(token));
        ++m_currentIndex;
    }

    m_display->shutdown();
}

bool Session::handleInput(UserAction action) {
    switch (action) {
    case UserAction::Quit:
        return false;

    case UserAction::Pause:
        m_paused = !m_paused;
        break;

    case UserAction::Resume:
        m_paused = false;
        break;

    case UserAction::SpeedUp:
        m_pacing.setWordsPerMinute(m_pacing.wordsPerMinute() + WPM_BUMP);
        break;

    case UserAction::SlowDown:
        m_pacing.setWordsPerMinute(std::max(SPEED_FLOOR, m_pacing.wordsPerMinute() - WPM_BUMP));
        break;

    case UserAction::Rewind:
        rewind(kSeekDuration);
        m_paused = true;
        break;

    case UserAction::FastForward:
        fastForward(kSeekDuration);
        break;

    case UserAction::None:
    default:
        break;
    }

    return true;
}

void Session::rewind(std::chrono::milliseconds duration) {
    const std::size_t step = m_pacing.wordsInDuration(duration);
    m_currentIndex = (step > m_currentIndex) ? 0 : m_currentIndex - step;
}

void Session::fastForward(std::chrono::milliseconds duration) {
    const std::size_t step = m_pacing.wordsInDuration(duration);
    m_currentIndex = std::min(m_tokens.size() - 1, m_currentIndex + step);
}
