// This is the CPP file for N-gram.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "hashmap.h"
#include "vector.h"
#include "set.h"
#include "queue.h"
#include "random.h"
#include "strlib.h"

using namespace std;


void copy(Vector<string>& sentence, Queue<string> window) {
    while (!window.isEmpty()) {
        string word = window.dequeue();
        sentence.add(word);
    }
}

string stringJoin(Vector<string> v, string delimiter) {
    string result = "";
    for (int i = 0; i < v.size(); i++) {
        result += v[i];
        result += delimiter;
    }
    return result;
}

void getNAndFile(ifstream& stream, int& N) {
    string file_name = promptUserForFile(stream, "Input file name?");
    while (true) {
        string input_N = getLine("Value of N?");
        if (!stringIsInteger(input_N)) {
            cout << "Illegal integer format. Try again." << endl;
        } else {
            N = stringToInteger(input_N);
            if (N < 2) {
                cout << "N must be 2 or greater." << endl;
            } else {
                break;
            }
        }
    }
}

void update(HashMap<Queue<string>, Vector<string>>& map, Queue<string>& key, string& word) {
    Vector<string> value;
    if (map.containsKey(key)) {
        value = map.get(key);
    } else {
        value.clear();
    }
    value.add(word);
    map.put(key, value);
    key.dequeue();
    key.enqueue(word);
}

void buildMap(HashMap<Queue<string>, Vector<string>>& map, ifstream& stream, int N) {
    string word;
    Queue<string> key;
    Queue<string> head;
    int cnt = 0;
    while (stream >> word) {
        if (cnt < N-1) {
            key.enqueue(word);
            cnt += 1;
        } else {
            if (cnt == N-1) {
                head = key;
            }
            update(map, key, word);
            cnt++;
        }
    }

    while (!head.isEmpty()) {
        word = head.dequeue();
        update(map, key, word);
    }
}

void generate(Vector<string>& sentence, int num_words, int N, HashMap<Queue<string>, Vector<string>>& map) {
    int cnt = N-1;
    int rand = randomInteger(0, map.size()-1);
    Queue<string> window = map.keys()[rand];

    Vector<string> value;
    copy(sentence, window);
    while (cnt < num_words) {
        value = map.get(window);
        rand = randomInteger(0, value.size()-1);
        string next = value[rand];
        sentence.add(next);
        window.dequeue();
        window.enqueue(next);
        cnt++;
    }
}

void generateFullSentence(Vector<string>& sentence, int num_words, int N, HashMap<Queue<string>, Vector<string>>& map) {
    Queue<string> window;
    int rand;
    Vector<Queue<string>> keys = map.keys();
    while (true) {
        rand = randomInteger(0, map.size() - 1);
        window = keys[rand];
        if (isupper(window.front()[0])) {
            break;
        }
    }

    int cnt = N - 1;
    Vector<string> value;
    copy(sentence, window);
    string next;
    while (true) {
        value = map.get(window);
        rand = randomInteger(0, value.size()-1);
        next = value[rand];
        sentence.add(next);
        if (((next.back() == '.') || (next.back() == '?') || (next.back() == '!')) && (cnt >= num_words)) {
            break;
        }
        window.dequeue();
        window.enqueue(next);
        cnt++;
    }
}

int main() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n"
         << endl;

    // Prompt for user input file and N.
    ifstream stream;
    int N;
    getNAndFile(stream, N);

    // Build N-gram graph for input file.
    HashMap<Queue<string>, Vector<string>> map;
    buildMap(map, stream, N);

    // generate sentence based on N-gram graph.
    while (true) {
        cout << endl;
        int num_words;
        string num_words_str = getLine("# of random words to generate (0 to quit)?");
        if (!stringIsInteger(num_words_str)) {
            cout << "Illegal integer format. Try again." << endl;
            continue;
        } else {
            num_words = stringToInteger(num_words_str);
            if (num_words == 0) {
                break;
            } else if (num_words < N) {
                cout << "Must be at least " << integerToString(N) << " words." << endl;
                continue;
            }
        }

        Vector<string> sentence;
        // generates full sentence.
        generateFullSentence(sentence, num_words, N, map);
        cout << stringJoin(sentence, " ") << endl;

        // randomly generates a section of sentence.
//        generate(sentence, num_words, N, map);
//        cout << "... " << stringJoin(sentence, " ") << "..." << endl;
    }

    cout << "Exiting." << endl;
    return 0;
}
