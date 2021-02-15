#include "Matrix.h"

#define RED 4
#define WHITE 15

#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

//Matrix class variables
map<char, int> Matrix::CharToInt = { {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9},
							{'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19} };
map<int, char> Matrix::IntToChar = { {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}, {8, 'I'}, {9, 'J'},
							{10, 'K'}, {11, 'L'}, {12, 'M'}, {13, 'N'}, {14, 'O'}, {15, 'P'}, {16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'} };


//Matrix constructor
Matrix::Matrix(int lineNumber, int columnNumber) {

	//assign to the private variables their values
	lines = lineNumber;
	columns = columnNumber;

	//alocate memory for the board vector (each subvector a line of the board)
	board.resize(lines);

	//alocate memory for the board subvectors, and fill them with spaces
	for (int i = 0; i < lines; i++) {
		board[i].resize(columns);
		board[i].assign(columns, ' ');
	}
}

//function to update the colour of the text to be printed on the console
void Matrix::updateColour(int lineIndex, int columnIndex) {

	//generates position pair from the arguments
	char line = IntToChar[lineIndex];
	char column = tolower(IntToChar[columnIndex]);
	pair <char, char> currentLetterPosition(line, column);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE); //default

	for (size_t i = 0; i < playedPositions.size(); i++) {
		//if the position is in the playedPositions vector, changes the text colour to red
		if (currentLetterPosition == playedPositions[i]) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			break;
		}
	}


}

//function to display the game board
void Matrix::displayBoard(int numberOfPlayers, vector<Player> playerObjectVector) {

	//display points
	for (int i = 0; i < numberOfPlayers; i++) {
		cout << "Player " << (i + 1) << "'s points: " << playerObjectVector[i].playerPoints << endl;
	}

	cout << "\n   ";
	
	//display column letters (a, b, c, d, etc.) on the first line
	for (int i = 0; i < columns; i++) {
		cout << " " << char(97 + i);
	}
	cout << endl;
	cout << "  " << char(201);
	for (int c = 0; c < (2 * columns + 1); c++) { cout << char(205); }
	cout << char(187) << endl;

	for (int i = 0; i < lines; i++) {

		cout << char(65 + i) << " " << char(186) << " ";	//displays line letter (A, B, C, etc.)

		for (int j = 0; j < columns; j++) {
			updateColour(i, j);					//changes the colour to the appropriate one
			cout << this->board[i][j] << " ";	//display current position being looped (letter or space)
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);	//changes colour to white after printing a line of the board

		cout << char(186) << " " << endl;
	}

	cout << "  " << char(200);
	for (int c = 0; c < (2 * columns + 1); c++) { cout << char(205); }
	cout << char(188) << endl;
}


//function to fill the empty board with spaces and the words in the board builder file
void Matrix::fillBoard(vector<string> instructions) {

	char lineToPlaceIn = instructions[0][0];
	char columnToPlaceIn = toupper(instructions[0][1]);

	switch (instructions[1][0])	//switch case with char in "H" or "V" string
	{
	case 'H':	//write horizontally - only changes 1 vectors of chars on the board
		for (size_t i = 0; i < instructions[2].size(); i++)
		{
			this->board[CharToInt[lineToPlaceIn]][CharToInt[columnToPlaceIn] + i] = instructions[2][i];
		}
		break;

	case 'V':	//write vertically - changes all vectors, but always on the same column
		for (size_t i = 0; i < instructions[2].size(); i++)
		{
			this->board[CharToInt[lineToPlaceIn] + i][CharToInt[columnToPlaceIn]] = instructions[2][i];
		}
		break;
	}
}


//function to play the chosen letter in the chosen position
void Matrix::playLetter(char letter, pair<char, char> position, int numberOfWordObjects, Player& turnPlayer) {

	//looks for the letter in the player's hand and removes it from that vector
	std::vector<char>::iterator it;
	it = std::find(turnPlayer.hand.begin(), turnPlayer.hand.end(), letter);
	turnPlayer.hand.erase(it);

	//iterates through the objects of the Word Objects Vector
	for(int i = 0; i < numberOfWordObjects; i++){

		Word& currentWord = wordVector[i];

		//edits the word that starts with the letter T, in position
		//this ALWAYS HAPPENS
		if (currentWord.playableLetter == letter && currentWord.playablePosition == position)
		{
			playedPositions.push_back(position);	//pushes that position onto the playedPositions vector

			currentWord.positionsLeft.erase(currentWord.positionsLeft.begin());	//deletes the first element of the positionsLeft and lettersLeft vectors of the
			currentWord.lettersLeft.erase(currentWord.lettersLeft.begin());		//word (since this word starts with that first letter, at that first position

			//if the length is 1, we change these values to something that will never be correct
			if (currentWord.length == 1) {

				currentWord.playablePosition = { ' ', ' ' };	//sets the next playablePosition and playableLetter to the
				currentWord.playableLetter = ' ';				//next letter of the word, and its respective position

				//we also give the turn player one point, for completing this word
				cout << "You have completed one or more words! You have earned one point for each!" << endl;
				turnPlayer.playerPoints++;

				Sleep(2000);
			}
			else {
				currentWord.playablePosition = currentWord.positionsLeft[0];	//sets the next playablePosition and playableLetter to the
				currentWord.playableLetter = currentWord.lettersLeft[0];		//next letter of the word, and its respective position
			}

			currentWord.length -= 1;	//also decrements length

			continue;	//no point in changing this word again
		}

		else {	//edits the other words, in case the word we tested in the first if statement starts in the middle of another word
			for (size_t z = 0; z < currentWord.length; z++) {

				char letterBeingTested = currentWord.lettersLeft[z];
				pair<char, char> positionBeingTested = currentWord.positionsLeft[z];

				//in case of a match
				if (letterBeingTested == letter && positionBeingTested == position) {

					currentWord.positionsLeft.erase(currentWord.positionsLeft.begin() + z);	//deletes that position and letter from that word's
					currentWord.lettersLeft.erase(currentWord.lettersLeft.begin() + z);		//positionsLeft and lettersLeft vectors

					currentWord.length -= 1;	//also decrements the word's length
				}
			}
		}
	}
}