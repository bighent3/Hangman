#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "HangmanManager.hpp"

using namespace std;

const string DICTIONARY_FILE = "dictionary.txt";
const bool SHOW_COUNT = false;

void playGame(HangmanManager& hangman) {
    while (hangman.getGuessesLeft() > 0 && hangman.pattern().find('-') != string::npos) {
        cout << "guesses : " << hangman.getGuessesLeft() << endl;
        if (SHOW_COUNT) {
            cout << "words   : " << hangman.words().size() << endl;
        }

        cout << "guessed : ";
        for (char c : hangman.guesses()) {
            cout << c << " ";
        }
        cout << "\ncurrent : " << hangman.pattern() << endl;

        cout << "Your guess? ";
        string input;
        cin >> input;
        char guess = tolower(input[0]);

        if (hangman.guesses().count(guess)) {
            cout << "You already guessed that" << endl;
        } else {
            int count = hangman.record(guess);
            if (count == 0) {
                cout << "Sorry, there are no " << guess << "'s" << endl;
            } else if (count == 1) {
                cout << "Yes, there is one " << guess << endl;
            } else {
                cout << "Yes, there are " << count << " " << guess << "'s" << endl;
            }
        }

        cout << endl;
    }
}

void showResults(const HangmanManager& hangman) {
    string answer = *hangman.words().begin();
    cout << "answer = " << answer << endl;
    if (hangman.getGuessesLeft() > 0) {
        cout << "You beat me" << endl;
    } else {
        cout << "Sorry, you lose" << endl;
    }
}

int main() {
    cout << "Welcome to the C++ Hangman game.\n\n";

    ifstream file(DICTIONARY_FILE);
    if (!file) {
        cerr << "Could not open dictionary file.\n";
        return 1;
    }

    vector<string> dictionary;
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.push_back(word);
    }

    int length, max;
    cout << "What length word do you want to use? ";
    cin >> length;
    cout << "How many wrong answers allowed? ";
    cin >> max;
    cout << endl;

    try {
        HangmanManager hangman(dictionary, length, max);
        if (hangman.words().empty()) {
            cout << "No words of that length in the dictionary." << endl;
        } else {
            playGame(hangman);
            showResults(hangman);
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
