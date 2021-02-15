// ScrabbleJunior.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "AuxiliaryFunctions.h"
#include "Matrix.h"
#include "Player.h"
#include "Pool.h"
#include "Words.h"

#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <Windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

int main()
{
	//GET PLAYERS AND GENERATE PLAYER VECTOR
    int playerNum = getPlayerNum();	//fetches the number of players

	std::vector<Player> playerVector(playerNum);	//create player vector

	for (int i=0; i < playerNum; i++){
		playerVector[i] = Player();		//create player objects in vector
	}
	//-----------------------------------------------------------


	//GET BOARD NAME AND CHECK VALIDITY
	bool isValid = true;
	string fileName;

	do {
		fileName = getFileName();							//get a file name
		isValid = testBoardValidity(fileName, playerNum);	//check if it is a valid board
	} while (isValid == false);	//Repeat until a valid board is inputted
	//-----------------------------------------------------------



	//CREATE EMPTY BOARD
	//some useful variables:
	std::ifstream readBoard;
	readBoard.open(fileName);
	string currentLine;
	int lines = 0;
	int columns = 0;

	std::getline(readBoard, currentLine);							//read first line
	vector<string> splitStrings = splitStringBySpace(currentLine);	//split string - returned vector comes in the form {"line number", "x", "column number"}

	sscanf_s(splitStrings[0].c_str(), "%d\n", &lines);			//save the number of lines
	sscanf_s(splitStrings[2].c_str(), "%d\n", &columns);		//save the number of columns

	Matrix loadedBoard(lines, columns);		//instanciate board object
	//-----------------------------------------------------------


	//POOL AND BOARD FILLING
	Pool gamePool;							//instanciate pool object

	//read beyond the first line
	while (!readBoard.eof()) {
		std::getline(readBoard, currentLine);			//read line

		if (currentLine == "") {
			break;
		}

		vector<string> splitStrings = splitStringBySpace(currentLine);	//split string - returned vector comes in the form {"line and column", "H/V", "word"}

		loadedBoard.fillBoard(splitStrings);										//fill the board vector with the word we just read
		loadedBoard.wordVector.push_back(Word(splitStrings[0], splitStrings[1], splitStrings[2]));	//add the word to the word vector
	}

	readBoard.close();	//closes the file

	int numberOfWords = loadedBoard.wordVector.size();	//sets the number of words
	//-----------------------------------------------------------

	//POOL FILLING
	for (auto line : loadedBoard.board) {
		for (auto column : line) {
			if (column != ' ') {	//if the character in that vector position isn't a space
				gamePool.addToPool(column);	//adds the letter on the board to the pool
			}
		}
	}
	//-----------------------------------------------------------

	//HAND FILLING
	for (auto& x : playerVector) {
		for (int i = 0; i < 7; i++)
		{
			x.addToHand(gamePool);			//adds 7 random letters to each player's hand
		}
	}
	//-----------------------------------------------------------Sleep(5000);

	//GAME
	bool gameOver = false;

	int currentPlayerIndex = 0;	//index of the current player on the player vector
	int currentPlayer;			//corresponding number (index + 1)
	bool canPlayALetter;		//records whether or not playing a letter is possible


	while (gameOver == false) {
		currentPlayerIndex = currentPlayerIndex % playerNum;	//number of the current player, from 0-3
		currentPlayer = currentPlayerIndex + 1;					//number of the current player, from 1-4
		Player& turnPlayer = playerVector[currentPlayerIndex];

		loadedBoard.displayBoard(playerNum, playerVector);		//clear the previous board, and display an updated version of it

		//show the current player's hand
		cout << "Player " << currentPlayer << "'s turn." << endl;
		cout << "Player " << currentPlayer << "'s hand: ";
		for (auto x : turnPlayer.hand) {	cout << x << " ";	}
		cout << endl << endl;


		canPlayALetter = canPlay(turnPlayer, loadedBoard);	//checks if the player can play a letter from his hand

		char playedLetter;
		pair<char,char>  selectedPosition;
		int numberOfLettersPlayed = 0;

		//if the player can play a letter
		if (canPlayALetter) {

			playedLetter = getLetterToPlay(loadedBoard, "Please type the letter you want to play: ", turnPlayer);	//gets the letter to play
			selectedPosition = getPositionToPlayIn(loadedBoard, playedLetter);										//gets the position to play the letter in
			loadedBoard.playLetter(playedLetter, selectedPosition, numberOfWords, turnPlayer);						//play the letter in the selected position

			numberOfLettersPlayed++;	//increments the counter by one

			system("cls");										//clears and
			loadedBoard.displayBoard(playerNum, playerVector);	//displays the board

			//show current player's hand
			cout << "Your hand is now: ";
			for (auto x : turnPlayer.hand) { cout << x << " "; }
			cout << endl;

			//checks if the player can a second letter
			canPlayALetter = canPlay(turnPlayer, loadedBoard);

			if (canPlayALetter) {
				
				playedLetter = getLetterToPlay(loadedBoard, "Please play another letter: ", turnPlayer);	//gets the letter to play
				selectedPosition = getPositionToPlayIn(loadedBoard, playedLetter);							//gets the position to play the letter in
				loadedBoard.playLetter(playedLetter, selectedPosition, numberOfWords, turnPlayer);			//play the letter in the selected position

				numberOfLettersPlayed++;	//increments the counter by one

				system("cls");										//clears and
				loadedBoard.displayBoard(playerNum, playerVector);	//displays the board

				if (turnPlayer.hand.size() > 0) {

					//if the player has letters in his hand, shows the player's hand
					cout << "Your hand is now: ";
					for (auto x : turnPlayer.hand) { cout << x << " "; }
					cout << endl;
				}
				else if (turnPlayer.hand.size() == 0) {

					//if the player does not have letters in his hand, prints out something different
					cout << "Your hand is empty!" << endl;
				}
			}
			else {
				//if the player cannot play a second time, prints out a message
				cout << "You cannot make any more moves this turn." << endl;
			}
		}
		else {

			//if the player cannot even play one letter, prints out something
			cout << "You cannot currently play any letters.";

			//if the player did not play any letters, the player exchanges the appropriate number of letters
			if (gamePool.poolPieces.size() >= 2) {
				turnPlayer.exchangeLetters(gamePool, 2);
			}
			else if (gamePool.poolPieces.size() == 1) {
				turnPlayer.exchangeLetters(gamePool, 1);
			}
			else if (gamePool.poolPieces.size() == 0) {
				cout << "Since the pool is empty, you could not exchange any letters in your hand." << endl;
			}
		}

		Sleep(2000);

		//if the player played at least 1 letter, the program refills the hand with the appropriate number of letters
		if (numberOfLettersPlayed > 0)
		{ 
			if (gamePool.poolPieces.size() >= 2) {
				turnPlayer.refillHand(gamePool, numberOfLettersPlayed);
			}
			else if (gamePool.poolPieces.size() == 1) {
				turnPlayer.refillHand(gamePool, 1);
			}
			else if (gamePool.poolPieces.size() == 0) {
				cout << "Since the pool is empty, you could not refill your hand." << endl;
			}
		
		}

		currentPlayerIndex++;					//gets the index of the next player

		gameOver = testEndGame(loadedBoard);	//checks if the game has ended

		//displays the appropriate message, at the end of the turn
		if (gameOver) {
			cout << "The game has ended. Press enter to see the results.";
		}
		else {
			cout << endl << "Press enter to go to the next player's turn...";
		}

		//passing the turn
		char leaveInput[50];
		cin.getline(leaveInput, 50, '\n');
		system("cls");
		cin.clear();
	}
	//-----------------------------------------------------------


	//DISPLAY THE FINAL SCORE
	pair<bool, vector<int>> results = checkForTie(playerVector);	//gets the results of the game

	if (results.first == false) {	//if the game is not a tie

		cout << "Player " << results.second[0] << " wins with " << results.second[1] << " points!";
	}
	else if (results.first == true)	//if the game is a tie
	{
		cout << "The game ended on a tie between the following players:" << endl;
		for (size_t i = 0; i < results.second.size(); i++) {
			cout << "Player " << results.second[i] + 1 << endl;
		}
	}
	//-----------------------------------------------------------
	
	return 0;
}