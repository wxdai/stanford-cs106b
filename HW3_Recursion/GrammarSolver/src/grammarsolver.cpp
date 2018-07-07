/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"
#include "map.h"

using namespace std;

void buildMap(istream& input, Map<string, Vector<string>>& grammar) {
    string line;
    while (getline(input, line)) {
        Vector<string> splitLine = stringSplit(line, "::=");
        string nonTermimal = splitLine[0];
        Vector<string> rules = stringSplit(splitLine[1], "|");
        grammar.put(nonTermimal, rules);
    }
}

string joinString(Vector<string> v, string delimiter) {
    string s;
    int i;
    for (i = 0; i < v.size()-1; i++) {
        s += v[i];
        s += delimiter;
    }
    s += v[i];
    return s;
}

string generateSymbol(const Map<string, Vector<string>>& grammar, const string& symbol) {
    if (!grammar.containsKey(symbol)) {
        return symbol;
    }

    Vector<string> result;
    Vector<string> rules = grammar.get(symbol);
    int x = randomInteger(0, rules.size()-1);
    Vector<string> randomRule = stringSplit(rules[x], " ");
    for (int i = 0; i < randomRule.size(); i++) {
        result.add(generateSymbol(grammar, randomRule[i]));
    }
    return joinString(result, " ");
}

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    // build map from input file.
    Map<string, Vector<string>> grammar;
    buildMap(input, grammar);

    // generate given symbols according to input gramma.
    Vector<string> result;
    for (int i = 0; i < times; i++) {
        result.add(generateSymbol(grammar, symbol));
    }

    return result;
}

