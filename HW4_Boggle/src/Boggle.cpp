// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include <stdexcept>

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static int kCubeRow = 4;
static int kCubeColumn = 4;
static int kCubeSize = kCubeRow * kCubeColumn;
static int kNumFaces = 6;
static vector<vector<int>> direction {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

Boggle::Boggle(Lexicon& dictionary, string boardText) : _board(kCubeRow, kCubeColumn),
    _dictionary(dictionary),  _visited(kCubeRow, kCubeColumn, false) {
    // use the given 16-letter string to initialize the 16 board cubes from top-left to bottom-right.
    // If the string is empty, generate a random shuffled board.
    if (boardText.empty()) {
        boardText = randomInitialize();
    }

    int k = 0;
    for (int i = 0; i < kCubeRow; i++) {
        for (int j = 0; j < kCubeColumn; j++) {
            _board[i][j] = boardText[k];
            k++;
        }
    }
}

string Boggle::randomInitialize() {
    // randomly shuffle the cubes and pick one of the letter facing up.
    string boardText;
    shuffle(CUBES, kCubeSize);
    for (int i = 0; i < kCubeSize; i++) {
        boardText += CUBES[i][randomInteger(0, kNumFaces-1)];
    }
    return boardText;
}

bool Boggle::checkWord(string word) {
    // check whether the given word string is suitable to search for:
    // that is, whether it is in the dictionary, long enough to be a valid Boggle word,
    // and has not already been found.
    // You should return a boolean result of true if the word is suitable, and otherwise you should return false.
    // method is case-insensitive; you should properly verify the word whether it is passed in upper, lower, or mixed case.
    return ((word.size() >= 4) && (_dictionary.contains(word)) && (!_validUserInputs.contains(toUpperCase(word))));
}

Set<string> Boggle::getUserInputs() {
    return _validUserInputs;
}

string Boggle::getLastUserInput() {
    return _lastUserInput;
}

void Boggle::updateUserStatus(string word) {
    _validUserInputs.add(toUpperCase(word));
    _lastUserInput = word;
    _humanScore += word.length() - 3;
}

bool Boggle::humanWordSearch(string word) {
    // perform a search on the board for an individual word.
    // If the word can be formed, you should add it to the human's set of found words.
    // function returns a boolean result of whether the word can be formed.
    // If the word is unsuitable, should not perform the recursive search.
    word = toUpperCase(word);
    for (int i = 0; i < kCubeRow; i++) {
        for (int j = 0; j < kCubeColumn; j++) {
            _visited[i][j] = true;
            if (humanWordSearchHelper(i, j, word, 0)) {
                _visited[i][j] = false;
                return true;
            }
            _visited[i][j] = false;
        }
    }
    return false;
}

bool Boggle::humanWordSearchHelper(int i, int j, const string& word, int k) {
    if (_board[i][j] != word[k]) {
        return false;
    } else if (k == int(word.length()) - 1) {
        return true;
    }

    for (const vector<int>& d : direction) {
        int ni = i + d[0];
        int nj = j + d[1];
        if ((ni >= 0) && (ni < kCubeRow) && (nj >= 0) && (nj < kCubeColumn) && !_visited[ni][nj]) {
            _visited[ni][nj] = true;
            if (humanWordSearchHelper(ni, nj, word, k+1)) {
                _visited[ni][nj] = false;
                return true;
            }
            _visited[ni][nj] = false;
        }
    }
    return false;
}


int Boggle::getScoreHuman() {
    return _humanScore;
}

Set<string> Boggle::computerWordSearch() {
    // perform a search on the board for all words that can be formed
    // (that have not already been found by the human player), and return them as a Set of strings.
    for (int i = 0; i < kCubeRow; i++) {
        for (int j = 0; j < kCubeColumn; j++) {
            _visited[i][j] = 1;
            computerWordSearchHelper(i, j, "");
            _visited[i][j] = 0;
        }
    }
    return _computerWords;
}

void Boggle::computerWordSearchHelper(int i, int j, string word) {
    string newWord = word + _board[i][j];
    if ((newWord.length() >= 4) && (_dictionary.contains(newWord)) && (!_validUserInputs.contains(newWord))) {
        _computerWords.add(newWord);
    }

    if (!_dictionary.containsPrefix(newWord)) {
        return;
    }

    for (const vector<int>& d : direction) {
        int ni = i + d[0];
        int nj = j + d[1];
        if ((ni >= 0) && (ni < kCubeRow) && (nj >= 0) && (nj < kCubeColumn) && (_visited[ni][nj] == 0)) {
            _visited[ni][nj] = 1;
            computerWordSearchHelper(ni, nj, newWord);
            _visited[ni][nj] = 0;
        }
    }
}


int Boggle::getScoreComputer() {
    int score = 0;
    for (auto w : _computerWords) {
        score += w.length() - 3;
    }
    return score;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < kCubeRow; i++) {
        for (int j = 0; j < kCubeColumn; j++) {
            out << boggle._board[i][j] << " ";
        }
        out << endl;
    }
    return out;
}
