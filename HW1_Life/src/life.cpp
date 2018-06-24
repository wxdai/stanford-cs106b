// This is the CPP file for "The Game of Life"

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;


// Grid display: print the current state of the grid.
bool printGridGUI(Grid<char>& matrix, LifeGUI& gui) {
    int n_cols = matrix.numCols();
    int n_rows = matrix.numRows();
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            bool alive;
            if (matrix[i][j] == 'X') {
                alive = true;
            } else {
                alive = false;
            }
            gui.drawCell(i, j, alive);
        }
    }
    return 0;
}

// Grid display: print the current state of the grid.
bool printGrid(Grid<char>& matrix) {
    int n_cols = matrix.numCols();
    int n_rows = matrix.numRows();
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }
    return 0;
}

// Updating to next generation: advance the grid from one generation to the next.
bool nextGen(Grid<char>& matrix, string wrap) {
    int n_cols = matrix.numCols();
    int n_rows = matrix.numRows();
    vector<tuple<int, int>> neighbours{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    Grid<char> temp(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            int n_neighbours = 0;
            for (int k = 0; k < int(neighbours.size()); k++) {
                if (wrap == "n") {
                    int x = i + get<0>(neighbours[k]);
                    int y = j + get<1>(neighbours[k]);
                    if ((x >= 0) && (x < n_rows) && (y >= 0) && (y < n_cols)
                        && matrix[x][y] == 'X') {
                            n_neighbours++;
                    }
                } else {
                    int x = (i + get<0>(neighbours[k]) + n_rows) % n_rows;
                    int y = (j + get<1>(neighbours[k]) + n_cols) % n_cols;
                    if (matrix[x][y] == 'X') {
                            n_neighbours++;
                    }
                }
            }
            if (n_neighbours == 2) {
                temp[i][j] = matrix[i][j];
            } else if (n_neighbours == 3) {
                temp[i][j] = 'X';
            } else {
                temp[i][j] = '-';
            }
        }
    }
    matrix = temp;
    return 0;
}

int main() {
    // introductory welcome message.
    cout << "Welcome to the CS 106B Game of Life,\n"
            "a simulation of the lifecycle of a bacteria colony.\n"
            "Cells (X) live and die by the following rules:\n"
            "- A cell with 1 or fewer neighbors dies.\n"
            "- Locations with 2 neighbors remain stable.\n"
            "- Locations with 3 neighbors will create life.\n"
            "- A cell with 4 or more neighbors dies.\n" << endl;

    // prompt for a filename, and open and read matrix.
    ifstream stream;
    string filename = promptUserForFile(stream, "Grid input file name?");
    string line;

    getline(stream, line);
    int n_rows = stringToInteger(line);
    getline(stream, line);
    int n_cols = stringToInteger(line);

    Grid<char> matrix(n_rows, n_cols);
    for (int i = 0; i < n_rows; i++) {
        getline(stream, line);
        for (int j = 0; j < n_cols; j++) {
            matrix[i][j] = char(line[j]);
        }
    }

    // prompt for whether wrap or not.
    string wrap = getLine("Should the simulation wrap around the grid (y/n)?");

    LifeGUI gui;
    gui.resize(n_rows, n_cols);
    printGridGUI(matrix, gui);

    // prompt main menu and do animation according to choices.
    while (true) {
        string choice = getLine("a)nimate, t)ick, q)uit?");
        if (toLowerCase(choice) == "t") {
            nextGen(matrix, wrap);
            printGridGUI(matrix, gui);
        } else if (toLowerCase(choice) == "q") {
            cout << "Have a nice Life!" << endl;
            break;
        } else if (toLowerCase(choice) == "a") {
            int n;
            while (true) {
                string iter = getLine("How many frames?");
                if (not stringIsInteger(iter)) {
                    cout << "Illegal integer format. Try again." << endl;
                } else {
                    n = stringToInteger(iter);
                    break;
                }
            }
            for (int i = 0; i < n; i++) {
                nextGen(matrix, wrap);
                // clearConsole();
                printGridGUI(matrix, gui);
                // pause(100);
             }
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
    return 0;
}


