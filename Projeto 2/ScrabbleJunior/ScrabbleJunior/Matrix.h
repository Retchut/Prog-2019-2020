#ifndef _MATRIX_H
#define _MATRIX_H

#include "Player.h"
#include "Words.h"


#include <map>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::vector;

class Matrix {
private:
	int lines, columns;

public:
	Matrix(int lineNumber, int columnNumber);

	static map<char, int> CharToInt;
	static map<int, char> IntToChar;

	vector <vector<char>> board;

	vector <pair<char, char>> playedPositions;

	vector <Word> wordVector;


	void updateColour(int lineIndex, int columnIndex);

	void displayBoard(int numberOfPlayers, vector<Player> playerObjectVector);

	void fillBoard(vector<string> instructions);

	void playLetter(char letter, pair<char, char> position, int numberOfWordObjects, Player& turnPlayer);
};

#endif