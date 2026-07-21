#pragma once
#include "../include/pacer/pacing.h"
#include <algorithm>
#include <chrono>

namespace {
constexpr int kMillisecondsPerMinute = 60'000;
constexpr double kSentencePauseMultiplier = 2.0;
constexpr double kClausePauseMultiplier = 1.5;
} // namespace

Pacing::Pacing(int wordsPerMinute) : m_wpm(wordsPerMinute) {
}

std::chrono::milliseconds Pacing::delayFor(const Token &token) const {
    const double baseMs = static_cast<double>(kMillisecondsPerMinute) / m_wpm;

    double multiplier = 1.0;
    if (token.endsSentence) {
        multiplier = kSentencePauseMultiplier;
    } else if (token.endsClause) {
        multiplier = kClausePauseMultiplier;
    }

    return std::chrono::milliseconds(static_cast<long long>(baseMs * multiplier));
}

std::size_t Pacing::wordsInDuration(std::chrono::milliseconds duration) const {
    const double baseMs = static_cast<double>(kMillisecondsPerMinute) / m_wpm;
    const double words = static_cast<double>(duration.count()) / baseMs;
    return static_cast<std::size_t>(std::max(0.0, words));
}

void Pacing::setWordsPerMinute(int wpm) {
    m_wpm = wpm;
}

int Pacing::wordsPerMinute() const {
    return m_wpm;
}
