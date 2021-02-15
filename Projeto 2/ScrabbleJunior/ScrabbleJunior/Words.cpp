#include "Matrix.h"
#include "Words.h"

#include <iostream>
#include <Windows.h>

using std::string;

//class constructor
Word::Word(string position, string orientation, string word) {

	//initializes length
	length = word.length();

	//gets line and column from the position string
	char line = position[0];
	char column = position[1];

	//initializes the playableLetter and playablePosition
	playableLetter = word[0];
	playablePosition = { line, column };

	//index and new values for the columns, if the orientation is Horizontal, or for the lines, if the orientation is Vertical
	int lineOrColumnIndex;
	char changedLineOrColumn;

	for (size_t i = 0; i < word.size(); i++) {

		lettersLeft.push_back(word[i]);	//pushes each letter of the word into the lettersLeft vector

		switch (orientation[0])	//letter is written horizontally ('H') or vertically('V')
		{
		case('H'):

			//gets the column index (on the board vector) of the letter of each iteration
			lineOrColumnIndex = Matrix::CharToInt[toupper(column)];
			
			//gets the index's corresponding lower case letter
			changedLineOrColumn = tolower(Matrix::IntToChar[lineOrColumnIndex + i]);

			//pushes the position into the positionsLeft vector
			positionsLeft.push_back({ line, changedLineOrColumn });
			break;

		case('V'):

			//gets the line index (on the board vector) of the letter of each iteration
			lineOrColumnIndex = Matrix::CharToInt[line];

			//gets the index's corresponding upper case letter
			changedLineOrColumn = Matrix::IntToChar[lineOrColumnIndex + i];

			//pushes the position into the positionsLeft vector
			positionsLeft.push_back({ changedLineOrColumn, column });
			break;
		}
	}
}