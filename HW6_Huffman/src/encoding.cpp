// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "HuffmanNode.h"
#include "pqueue.h"
#include "strlib.h"
#include "filelib.h"
// TODO: include any other headers you need

// Step 1: read input from a given istream (which could be a file on disk, a string buffer, etc.),
// count and return a mapping from each character (represented as int here) to the number of times
// that character appears in the file. Also add a single occurrence of the fake character PSEUDO_EOF into the map.
// The file might be empty. An empty file would return a map containing only the 1 occurrence of PSEUDO_EOF.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true) {
        int next = input.get();
        if (next == -1) {
            freqTable.put(PSEUDO_EOF, 1);
            break;
        } else {
            if (freqTable.containsKey(next)) {
                int freq = freqTable.get(next) + 1;
                freqTable.put(next, freq);
            } else {
                freqTable.put(next, 1);
            }
        }
    }
    return freqTable;
}

// Step 2: take a frequency table and use it to create a Huffman encoding tree based on those frequencies.
// Return a pointer to the node representing the root of the tree.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // Use freqTable to build a priority queue of Huffman nodes.
    PriorityQueue<HuffmanNode*> nodeQueue;
    for (const auto it : freqTable) {
        HuffmanNode* node = new HuffmanNode(it, freqTable.get(it), NULL, NULL);
        nodeQueue.enqueue(node, freqTable.get(it));
    }
    // Build an binary encoding tree from priority queue.
    while (nodeQueue.size() >= 2) {
        HuffmanNode* left = nodeQueue.dequeue();
        HuffmanNode* right = nodeQueue.dequeue();
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, left->count + right->count, left, right);
        nodeQueue.enqueue(node, node->count);
    }
    HuffmanNode* root = nodeQueue.dequeue();
    return root;
}

// Step 3: accept a pointer to the root node of a Huffman tree and use it to create and return a Huffman encoding map
// based on the tree's structure. Each key in the map is a character, and each value is the binary encoding for that
// character represented as a string.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string>* encodingMap = new Map<int, string>();
    // Backtracking to build the map.
    backtracking(encodingTree, "", encodingMap);
    return *encodingMap;
}

// Helper function to backtrack a given tree node.
void backtracking(HuffmanNode* treeNode, string path, Map<int, string>* encodingMap) {
    if ((treeNode->zero == NULL) && (treeNode->one == NULL)) {
        encodingMap->put(treeNode->character, path);
        return;
    }
    if (treeNode->zero != NULL) {
        backtracking(treeNode->zero, path + integerToString(0), encodingMap);
    }
    if (treeNode->one != NULL) {
        backtracking(treeNode->one, path + integerToString(1), encodingMap);
    }
}

// Step 4: read one character at a time from a given input file, and use the provided encoding map
// to encode each character to binary, then write the character's encoded binary bits to the given bit output bit stream.
// After writing the file's contents, write a single occurrence of the binary encoding for PSEUDO_EOF into the output
// to identify the end of the data when decompressing the file later.
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    string result;
    while (true) {
        int next = input.get();
        if (next == -1) {
            result += encodingMap.get(PSEUDO_EOF);
            break;
        } else {
            result += encodingMap.get(next);
        }
    }
    for (auto c : result) {
        output.writeBit(stringToInteger(charToString(c)));
    }
}

// Read bits from the given input file one at a time, and recursively walk through the specified decoding tree
// to write the original uncompressed contents of that file to the given output stream.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curNode = encodingTree;
    while (true) {
        int next = input.readBit();
        if (next == -1) {
            break;
        } else {
            if (next == 0) {
                curNode = curNode->zero;
            } if (next == 1) {
                curNode = curNode->one;
            }
            if (curNode->character == PSEUDO_EOF) {
                break;
            } else if (curNode->character == NOT_A_CHAR) {
                continue;
            } else {
                output.put(curNode->character);
                curNode = encodingTree;
            }
        }
    }
}

// This is the overall compression function: compress the given input file into the given output file.
// Take as parameters an input file that should be encoded and an output bit stream to which the compressed bits
// of that input file should be written. Read the input file one character at a time, building an encoding of its contents,
// and write a compressed version of that input file, including a header, to the specified output file.
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    for (char c : freqTable.toString()) {
        for (int i = 7; i >= 0; i--) {
            if (c & (1 << i)) {
                output.writeBit(1);
            } else {
                output.writeBit(0);
            }
        }
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

// This function should do the opposite of compress: it should read the bits from the given input file one at a time,
// including header packed inside the start of the file, to write the original contents of that file to the file specified
// by the output parameter.
void decompress(ibitstream& input, ostream& output) {
    string mapString = "";
//    string byte = "";
    while (true) {
        char c = 0;
        for (int i = 0; i < 8; i++) {
            c <<= 1;
            c += input.readBit();
        }
        mapString += c;
        if (c == '}') {
            break;
        }
    }
    Map<int, int> freqTable;
    bool isKey = true;
    int key = 0;
    int val = 0;
    for (char c : mapString) {
        if (c >= '0' && c <= '9') {
            if (isKey) {
                key *= 10;
                key += c - '0';
            } else {
                val *= 10;
                val += c - '0';
            }
        } else if (c == ',' || c == '}') {
            freqTable.put(key, val);
            key = val = 0;
            isKey = true;
        } else if (c == ':') {
            isKey = false;
        }
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

// This function should free the memory associated with the tree whose root node is represented by the given pointer.
// You must free the root node and all nodes in its subtrees.
void freeTree(HuffmanNode* node) {
    if (node->zero != NULL) {
        freeTree(node->zero);
    }
    if (node->one != NULL) {
        freeTree(node->one);
    }
    delete node;
}
