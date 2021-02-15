#include "Player.h"
#include "AuxiliaryFunctions.h"

#include <iostream>
#include <time.h>

using std::cout;
using std::endl;
using std::map;
using std::string;


//function to add letters to a player's hand
void Player::addToHand(Pool& letterPool) {


	srand((unsigned int)time(0));												//randomizes seed
	int randomIndex = (rand() % letterPool.poolPieces.size());					//gets an index at random

	//pushes that letter into the player hand
	this->hand.push_back(letterPool.poolPieces[randomIndex]);

	//removes the letter from the pool
	letterPool.poolPieces.erase(letterPool.poolPieces.begin() + randomIndex);
}

//function to refill a player's hand with a number of letters
void Player::refillHand(Pool& letterPool, int amount) {
	cout << "Your hand has been refilled." << endl;

	//calls the addToHand function amount times
	for (int i = 0; i < amount; i++) {
		this->addToHand(letterPool);
	}

	//shows updated hand
	cout << endl << "It is now: ";
	for (auto x : (*this).hand) { cout << x << " "; }
	cout << endl;

	//if the pool is emptied by this, a message is displayed
	if (letterPool.poolPieces.size() == 0) {
		cout << "The pool is now empty." << endl;
	}
}

//function to allow the player to exchange his letters for others in the pile
void Player::exchangeLetters(Pool& letterPool, int numberToSwap) {

	cout << "Since you could not play a letter this turn, you must swap " << numberToSwap << " of your letters with letters from the pool." << endl;

	Player& turnPlayer = *this;

	//the loop happens numberToSwap times
	for (int i = 0; i < numberToSwap; i++) {
		//gets input of the letter to remove from the hand
		char input = getLetterToExchange("Please input the letter to swap: ", turnPlayer);

		//adds the letter to be removed from the hand to the pool
		letterPool.addToPool(input);

		//looks for the letter in the player's hand and removes it from the hand
		std::vector<char>::iterator it;
		it = std::find(turnPlayer.hand.begin(), turnPlayer.hand.end(), input);
		turnPlayer.hand.erase(it);

		//fetches a new random letter from the pool, and adds it to the hand
		turnPlayer.addToHand(letterPool);

		cout << "Your hand is now: ";
		for (auto x : (*this).hand) { cout << x << " "; }	//show current player's hand
		cout << endl;
	}
}
