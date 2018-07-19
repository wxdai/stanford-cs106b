// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "simpio.h"
#include "strlib.h"
#include "Boggle.h"
#include "console.h"

using namespace std;

const int kNumSize = 16;

bool isValid(string inputText) {
    // check if input has expected size, and elements are all letters.
    if (int(inputText.length()) != kNumSize) {
        return false;
    } else {
        for (auto c : inputText) {
            if ((c > 'Z') || (c < 'A')) {
                return false;
            }
        }
    }
    return true;
}

void playOneGame(Lexicon& dictionary) {
    bool is_random = getYesOrNo("Do you want to generate a random board?");
    string boardText;

    // get user's input for board.
    if (!is_random) {
        while (true) {
            string inputText = toUpperCase(getLine("Type the 16 letters to appear on the board:"));
            if (isValid(inputText)) {
                boardText = inputText;
                break;
            } else {
                cout << "That is not a valid 16-letter board string. Try again." << endl;
            }
        }
    }

    // Initialize the board game.
    Boggle game(dictionary, boardText);

    // user's turn first.
    int flag = 0;
    while (true) {
//        clearConsole();
        if (flag == 0) {
            cout << "It's your turn!" << endl;
        } else if (flag == 1) {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        } else if (flag == 2) {
            cout << "That word can't be formed on this board." << endl;
        } else if (flag == 3) {
            cout << "You found a new word! " << game.getLastUserInput() << endl;
        }
        cout << game << endl;
        cout << "Your words (" << game.getUserInputs().size() << "): " << game.getUserInputs() << endl;
        cout << "Your score: " << game.getScoreHuman() << endl;
        string userInput = getLine("Type a word (or Enter to stop): ");
        if (userInput == "") {
            break;
        }
        if (!game.checkWord(userInput)) {
            flag = 1;
        } else if (!game.humanWordSearch(userInput)) {
            flag = 2;
        } else {
            flag = 3;
            game.updateUserStatus(userInput);
        }
    }

    // computer's turn.
    cout << endl;
    cout << "It's my turn!" << endl;
    cout << "My words (" << game.computerWordSearch().size() << "): " << game.computerWordSearch() << endl;
    cout << "My score: " << game.getScoreComputer() << endl;

    if (game.getScoreComputer() > game.getScoreHuman()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
}
