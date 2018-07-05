// This program finds the shortest path to change
// one word to the other one letter at a time.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "strlib.h"
#include "filelib.h"

using namespace std;

Vector<string> gen_neighbors(const string word, const Lexicon& dict) {
    Vector<string> neighbors;
    for (int i = 0; i < int(word.length()); i++) {
        for (int c = 'a'; c <= 'z'; c++) {
            string s;
            s = word.substr(0, i) + char(c) + word.substr(i+1, word.length() - i - 1);
            if (dict.contains(s)) {
                neighbors.add(s);
            }
        }
    }
    return neighbors;
}

bool used(const string word, Stack<string> ladder) {
    while (!ladder.isEmpty()) {
        string popped_word = ladder.pop();
        if (word == popped_word) {
            return true;
        }
    }
    return false;
}

Stack<string> word_ladder(const string word1, const string word2, const Lexicon& dict) {
    Stack<string> result;
    Queue<Stack<string>> q;
    Stack<string> s{word1};
    q.enqueue(s);
    Stack<string> ladder;
    string curr_word;
    Vector<string> neighbors;
    while (!q.isEmpty()) {
        ladder = q.dequeue();
        curr_word = ladder.peek();
        neighbors = gen_neighbors(curr_word, dict);
        for (int i = 0; i < neighbors.size(); i++) {
            if (!used(neighbors[i], ladder)) {
                if (neighbors[i] == word2) {
                    ladder.push(neighbors[i]);
                    result = ladder;
                    return result;
                } else {
                    Stack<string> new_ladder = ladder;
                    new_ladder.push(neighbors[i]);
                    cout << new_ladder << endl;
                    q.enqueue(new_ladder);
                }
            }
        }
    }
    return result;
}

int main() {
    // print welcome words.
    cout <<  "Welcome to CS 106B Word Ladder.\n"
             "Please give me two English words, and I will change the\n"
             "first into the second by changing one letter at a time.\n"
         << endl;

    // prompt for user input file.
    ifstream stream;
    string dict_file = promptUserForFile(stream, "Dictionary file name?");

    // build a dictionary.
    Lexicon dict(dict_file);

    string word1, word2;

    while (true) {
        while (true) {
            // get two words from user inputs, enter to quit.
            word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
            if (word1 == "") {
                cout << "Have a nice day." << endl;
                return 0;
            }
            word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
            if (word2 == "") {
                cout << "Have a nice day." << endl;
                return 0;
            }

            // sanity check
            // 1. both words typed by the user are valid words found in the dictionary.
            // 2. they are the same length.
            // 3. they are not the same word.
            if (!(dict.contains(word1) && dict.contains(word2))) {
               cout << "The two words must be found in the dictionary.\n" << endl;
            } else if (word1.length() != word2.length()) {
                cout << "The two words must be the same length.\n" << endl;
            } else if (word1 == word2) {
                cout << "The two words must be different.\n" << endl;
            } else {
                break;
            }
        }
        Stack<string> ladder = word_ladder(word2, word1, dict);
        if (ladder.isEmpty()) {
            cout << "No word ladder found from azure back to metal.\n" << endl;
        } else {
            cout << "A ladder from cat back to dog:" << endl;
            cout << ladder.toString() << '\n' << endl;
        }

    }
    return 0;
}
