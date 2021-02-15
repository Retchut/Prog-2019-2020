#include "AuxiliaryFunctions.h"
#include "Matrix.h"
#include "Player.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


//function to receive string input from the player(s)
string getInput(string message) {
	string input;
	cout << message;
	getline(cin, input);
	cin.clear();
	return input;
}


//function to receive the number of players
int getPlayerNum() {
	int playerNum;
	while (true) {
		string inputString = getInput("Please input the number of players (2-4): ");	//get string input

		sscanf_s(inputString.c_str(), "%d\n", &playerNum);								//convert string to integer

		//the only valid answers are 2, 3 and 4
		if (inputString.length() > 1) {
			cout << "That input is invalid." << endl;
			continue;
		}

		//check validity of the input
		if (playerNum >= 2 && playerNum <= 4)
		{
			return playerNum;
		}
		else
		{
			cout << "That input is invalid." << endl;
		}
	}
}


//function to receive the file name from the player
string getFileName() {
	while (true)
	{
		string inputString = getInput("Please input the name of the board you wish to open (in the format \"<name>.txt\"): ");	//get string input

		string strTest = inputString.substr(inputString.size() - 4);	//test string, to check the validity of the input

		//check validity (name must contain .txt, and it can't be just ".txt")
		if (strTest == ".txt" && inputString.size() >= 5)
		{
			return inputString;
		}
		else {
			cout << "That is not a valid name." << endl;
		}
	}
}


//string splitter function (by space only)
vector<string> splitStringBySpace(string toSplit) {
	std::istringstream splitStream(toSplit);
	vector<string> results((std::istream_iterator<string>(splitStream)), std::istream_iterator<string>());
	return results;
}


//function to test a board's validity
bool testBoardValidity(string fileName, int playerNumber) {

	std::ifstream reading;
	reading.open(fileName);		//open file

	if (!reading) {
		cout << "That file does not exist." << endl;
		return false;
	}


	std::string currentLine;			//line we're currently reading
	std::string currentWord;			//word whose letters we're counting
	std::getline(reading, currentLine);	//read the first line

	std::vector<std::string> splitStrings = splitStringBySpace(currentLine);	//split the line

	int lines = 0;
	int columns = 0;
	sscanf_s(splitStrings[0].c_str(), "%d\n", &lines);			//get number of lines
	sscanf_s(splitStrings[2].c_str(), "%d\n", &columns);		//get number of columns

	Matrix testBoard(lines, columns);		//instanciate test board object

	int letterNumber = 0;	//total letter number variable

	while (!reading.eof()) {
		std::getline(reading, currentLine);					//get each line

		if (currentLine == "") {
			break;
		}

		vector<string> splitStrings = splitStringBySpace(currentLine);	//split string - returned vector comes in the form {"line and column", "H/V", "word"}
		testBoard.fillBoard(splitStrings);
	}

	reading.close();	//close file

	for (auto line : testBoard.board) {
		for (auto column : line) {
			if (column != ' ') {
				letterNumber++;		//counts letters on the board
			}
		}
	}

	//check the validity of that board, and return the appropriate boolean
	if (letterNumber < (playerNumber * 7))
	{
		cout << "That board does not have enough letters for all the players." << endl;
		return false;
	}
	else {
		return true;
	}
}


//function that checks if a letter can be played
bool canPlay(Player turnPlayer, Matrix board) {

	//if the turn player's hand is empty, the player can't play
	if (turnPlayer.hand.size() == 0) {
		cout << "Your hand is empty! Sadly, you cannot play this turn." << endl;
		return false;
	}

	for (auto letter : turnPlayer.hand) {					//for each letter in the player's hand
		for (auto word : board.wordVector) {				//compares that letter to the playable letter
			if (word.playableLetter == letter) {			//of each word in the wordVector
				return true;								//if they're equal, that letter can be played
			}												//in one word's playable position, at the very least
		}
	}
	return false;
}

//same as above, but this overload is used only inside the getLetterToPlay function
bool canPlay(char selectedLetter, Matrix board, Player turnPlayer) {
	for (auto word : board.wordVector) {					//compares that inputted letter
		if (word.playableLetter == selectedLetter) {		//to the playable letter of each word
			return true;									//if it is equal to any, that letter can be played
		}													//in one word's playable position, at the very least
	}
	return false;
}


//function to receive the input of the letter to be played
char getLetterToPlay(Matrix board, string message, Player turnPlayer) {
	while (true) {

		string inputString = getInput(message);	//get string input

		//check validity of the input
		if (inputString.length() > 1) {
			cout << "That input is too long." << endl << endl;
			continue;
		}
		else {
			char relevantChar = inputString[0];

			bool playable = canPlay(relevantChar, board, turnPlayer);

			switch (playable)
			{
			case(true):
				return relevantChar;
				break;
			case(false):
				cout << "You can't play that character. " << endl << endl;
				break;
			}
		}
	}
}


//function that receives the position the chosen letter will be played in
std::pair<char, char> getPositionToPlayIn(Matrix board, char selectedLetter) {
	while (true) {

		string inputString = getInput("Please input the position where you wish to play the letter (e.g. Hf or Nj): ");	//get string input

		//check validity of the input
		if (inputString.length() > 2) {
			cout << "That input is too long." << endl << endl;
			continue;
		}
		else if (inputString.length() < 2) {
			cout << "That input is too short." << endl << endl;
			continue;
		}
		else if (islower(inputString[0]) || isupper(inputString[1])){
			cout << "The input must be in the format <capital letter><lower case letter> (e.g. Hf or Nj)" << endl << endl;
			continue;
		}
		else {	//if the input is correct so far
			for (auto wordObject : board.wordVector) {	//iterates through each word object

				pair <char, char> inputPosition(inputString[0], inputString[1]);		//creates position pair, to compare with the words' playable positions
																						
				if (inputPosition == wordObject.playablePosition && wordObject.playableLetter == selectedLetter) {		//if both the position and the letter are correct
					return inputPosition;																				//that position is valid
				}
			}
			cout << "You can't play that letter there." << endl << endl;
		}
	}
}


//function that receives the letter to be exchanged input
char getLetterToExchange(string message, Player turnPlayer) {
	while (true) {

		string inputString = getInput(message);	//get string input

		//check validity of the input (the input must be a char, and the char must be in the player's hand)
		if (inputString.length() > 1) {
			cout << "That input is too long." << endl;
			continue;
		}
		else {
			char relevantChar = inputString[0];

			bool valid = false;

			std::vector<char>::iterator it;
			it = std::find(turnPlayer.hand.begin(), turnPlayer.hand.end(), relevantChar);

			if (it != turnPlayer.hand.end()) {
				valid = true;
			}

			switch (valid)
			{
			case(true):
				return relevantChar;
				break;
			case(false):
				cout << "You don't own that letter.";
				break;
			}
		}
	}
}

//function that checks if the game has ended (all words have been completed)
bool testEndGame(Matrix board) {
	for (auto x : board.wordVector) {
		if (x.length > 0) {
			return false;
		}
	}
	return true;
}

//function that checks for a tie
pair<bool, vector<int>> checkForTie(vector<Player> playerVector) {

	size_t playerNum = playerVector.size();

	//check the highest score
	int highestScore = 0;
	for (size_t i = 0; i < playerNum; i++)
	{
		if (playerVector[i].playerPoints > highestScore) {
			highestScore = playerVector[i].playerPoints;
		}
	}

	//check for a tie
	vector<int> highestValueIndexes;
	for (size_t i = 0; i < playerNum; i++)
	{
		if (playerVector[i].playerPoints == highestScore) {
			highestValueIndexes.push_back(i);
		}
	}

	//highestValueIndexes.push_back(highestScore);

	if (highestValueIndexes.size() == 1) {
		return pair<bool, vector<int>>(false, highestValueIndexes);
	}
	else {
		return pair<bool, vector<int>>(true, highestValueIndexes);
	}
}