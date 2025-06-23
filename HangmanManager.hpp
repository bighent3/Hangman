#ifndef HANGMAN_MANAGER_HPP
#define HANGMAN_MANAGER_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>

class HangmanManager {
private:
    std::set<std::string> currentWords;
    std::set<char> guessedLetters;
    int maxGuesses;
    int guessesLeft;
    std::string currentPattern;

    std::string createFirstPattern(int length) {
        std::ostringstream oss;
        for (int i = 0; i < length; i++) {
            oss << "-";
            if (i != length - 1) oss << " ";
        }
        return oss.str();
    }

    std::string buildPattern(const std::string& word) {
        std::ostringstream oss;
        for (size_t i = 0; i < word.length(); i++) {
            if (guessedLetters.count(word[i])) {
                oss << word[i];
            } else {
                oss << "-";
            }
            if (i != word.length() - 1) oss << " ";
        }
        return oss.str();
    }

    int numOccurrences(const std::string& pattern, char guess) {
        return std::count(pattern.begin(), pattern.end(), guess);
    }

public:
    HangmanManager(const std::vector<std::string>& dictionary, int length, int max) {
        if (length < 1 || max < 0) {
            throw std::invalid_argument("Length and guesses must be valid.");
        }

        for (const auto& word : dictionary) {
            if (word.length() == static_cast<size_t>(length)) {
                currentWords.insert(word);
            }
        }

        if (currentWords.empty()) {
            throw std::invalid_argument("No words of the user-given length.");
        }

        maxGuesses = max;
        guessesLeft = max;
        currentPattern = createFirstPattern(length);
    }

    std::set<std::string> words() const {
        return currentWords;
    }

    int getGuessesLeft() const {
        return guessesLeft;
    }

    std::set<char> guesses() const {
        return guessedLetters;
    }

    std::string pattern() const {
        if (currentWords.empty()) {
            throw std::logic_error("No words available.");
        }
        return currentPattern;
    }

    int record(char guess) {
        if (guessesLeft < 1 || currentWords.empty()) {
            throw std::logic_error("Game is over or no words available.");
        }
        if (guessedLetters.count(guess)) {
            throw std::invalid_argument("Letter already guessed.");
        }

        guessedLetters.insert(guess);

        std::map<std::string, std::set<std::string>> patternGroups;
        for (const auto& word : currentWords) {
            std::string pattern = buildPattern(word);
            patternGroups[pattern].insert(word);
        }

        std::string largestPattern;
        size_t maxGroupSize = 0;
        for (const auto& entry : patternGroups) {
            if (entry.second.size() > maxGroupSize) {
                largestPattern = entry.first;
                maxGroupSize = entry.second.size();
            }
        }

        currentWords = patternGroups[largestPattern];
        currentPattern = largestPattern;
        int occurrences = numOccurrences(currentPattern, guess);
        if (occurrences == 0) {
            guessesLeft--;
        }

        return occurrences;
    }
};

#endif
