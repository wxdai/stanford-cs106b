// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>

#include "grid.h"
#include "lexicon.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    // TODO: add any other member functions/variables necessary
    Set<string> getUserInputs();
    friend ostream& operator<<(ostream& out, Boggle& boggle);
    void updateUserStatus(string word);
    string getLastUserInput();

private:
    string randomInitialize();
    bool isDuplicated(string word);
    bool humanWordSearchHelper(int i, int j, const string& word, int k);
    void computerWordSearchHelper(int i, int j, string word);
    Grid<char> _board;
    Lexicon& _dictionary;
    Set<string> _validUserInputs;
    Set<string> _computerWords;
    string _lastUserInput;
    int _humanScore = 0;
    Grid<bool> _visited;
};


#endif // _boggle_h
