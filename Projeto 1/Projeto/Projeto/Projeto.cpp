// Projeto.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <vector>

using std::cout;
using std::cin;
using std::string;
using std::getline;

struct beanRelatedVariables
{
	bool vsCPU = false;						// these two variables keep track of the
	int difficulty;							// type of gameMode selected by the player
	int aBeans, bBeans, opponentBeans;		// playerA's score | playerB's score | beans on the opponent's board
	int pots[12];							// board
};

void rules();																								//function that displays the rules and instructions

int gameModeSelector(bool& opponentIsCPU);																	//function to select the game mode

void displayBoard(beanRelatedVariables& beanVariables);														// function to display the board

int inputManager(bool playerIsA, string turnPlayer, int offset, beanRelatedVariables& beanVariables);		//function to ask for and receive the player's input

int gameController(int beansToMove, int potToSow, beanRelatedVariables& beanVariables);						//function that manages the sowing phase

bool canCollect(bool playerAturn, int currentpot, int offset, beanRelatedVariables& beanVariables);			//function to check if the player is allowed to collect the opponent's beans

void scoreController(bool playerATurn, int currentPot, beanRelatedVariables& beanVariables);				//function that manages the capturing phase

int inputCPU(beanRelatedVariables& beanVariables);															//function to get the CPU's input, if the difficulty is set to hard

int getPlayValue(int boardToBeTested[12], int beansToSow, int currentPot);									//function to get the value of all possible moves the cpu can make. called by the inputCPU function


int main()
{
	//TURN RELATED VARIABLES
	string currentPlayer;		//Whose turn it is
	bool playerATurn = true;	//To check whose turn it is
	int offsetValue;			//Difference between A row and B row
	bool forcedTie = false;				//Forced tie controller

	//BEAN RELATED VARIABLES
	beanRelatedVariables beans;
	beans.aBeans = 0;
	beans.bBeans = 0;

	std::fill_n(beans.pots, 12, 4);

	//INPUT AND COLLECTION RELATED VARIABLES
	bool collectionAllowed;

	string answer = "n";	//Imaginary string to fool the player into thinking he's in control

	rules();		//Displays rules
	beans.difficulty = gameModeSelector(beans.vsCPU);	//Selects game mode and changes the cpu mode bool to true, if needed


	//GAME
	while (beans.aBeans < 25 && beans.bBeans < 25) {

		//PREPARING THE BOARD EVERY "ROUND"
		system("cls");					//Clear console before every turn (to display the board afterwards and "update" the gamestate)
		displayBoard(beans);			//Display starting board/board after every change

		//PREPARING TO RECEIVE INPUT
		switch (playerATurn) {						//Checks whose player's turn it is
													//Also controls offset(for use with the array)
		case true:
			currentPlayer = "Player A";		//Sets current player string to player A
			offsetValue = 0;				//Sets offset to 0 for player A
			break;

		case false:
			currentPlayer = "Player B";		//Same as above but for player B
			offsetValue = 6;				//Sets offset to 6 for player B
			break;
		}

		//calculates the total of beans on the opponent's board
		beans.opponentBeans = 0;
		for (int i = 6; i < 12; i++) {
			beans.opponentBeans += beans.pots[i - offsetValue];
		}

		//GETS INPUT FROM PLAYER
		int inputPot = inputManager(playerATurn, currentPlayer, offsetValue, beans);

		if (inputPot == -1)	//Checks if the player forced a tie
		{
			cout << "The player forced a tie.\n";
			return 0;
		}

		//PREPAIRS TO CHANGE EACH POT'S VALUES (SOWING), BASED ON THE INPUT
		int inputPotInArray = inputPot + offsetValue - 1;					//Gets the inputted pot's position in the array
		int beansInPot = beans.pots[inputPotInArray];							//Gets the number of beans in that pot
		beans.pots[inputPotInArray] = 0;										//Sets that pot's number to 0, after collection of the beans

		system("cls");								//clears the console
		displayBoard(beans);						//and displays the board with the updated value on the selected pot

		inputPotInArray = gameController(beansInPot, inputPotInArray, beans);	//Calls function which sows the pots, and 
																				//returns the value of the last pot to be sowed

		if (((playerATurn && inputPotInArray >= 6) || (!playerATurn && inputPotInArray < 6)) && (beans.pots[inputPotInArray] == 2 || beans.pots[inputPotInArray] == 3))
		{						/*If it's player A's turn and the last bean to be sowed was on PB's side or it's player B's turn and
									the opposite is verified AND if the last pot to be sowed now has exactly 2 or 3 beans*/

			collectionAllowed = canCollect(playerATurn, inputPotInArray, offsetValue, beans);	//checks if the opponent will be left without beans after collecting

			if (collectionAllowed)				//if the opponent isn't left without beans, the game moves to the collecting phase
			{
				scoreController(playerATurn, inputPotInArray, beans);		//Calls function that collects the beans
			}
			else								//if the opponent would be left without beans, displays a message and the game does not collect them
			{
				cout << "As the opponent would have been left without any beans to play with, no beans were collected.\n";
				Sleep(3000);
			}
		}

		//Checks whose player's turn it is, and changes to the opponent's
		switch (playerATurn) {
		case true:
			playerATurn = false;	//Changes to the other player
			break;

		case false:
			playerATurn = true;		//Same as above
			break;
		}

		//in case of a draw 
		if (beans.aBeans == 24 && beans.bBeans == 24)
		{
			break;
		}
	}

	//VICTORY MESSAGE
	if (beans.aBeans >= 25) {
		cout << "Player A wins with " << beans.aBeans << " beans!\n";
	}
	else if (beans.bBeans >= 25) {
		cout << "Player B wins with " << beans.bBeans << " beans!\n";
	}
	else {
		cout << "The game ended on a draw!";
	}

	return 0;
}

//function that displays the rules and instructions
void rules() {
	char leaveInput[50];

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "\t\t\t\t\tOWARE\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	cout << "\tThe goal of this game is to capture more beans than the opponent.\n\n";
	cout << "\tEach player starts with 4 beans on each of his 6 pots.\n";
	cout << "\tPlayers take turns choosing one of their pots, removing the beans on that pot, and distributing them around the board, one by one.\n";
	cout << "\tIf the removed beans are enough to loop around the board, when distributing them, the player must skip the pot they originated from.\n";
	cout << "\tIf the last bean to be 'sown' lands in a pot which now has either 2 or 3 beans, the turn player collects the beans in that pot,";
	cout << " along with the beans inside pots directly before that one, whose beans now equal exactly 2 or 3, so long as those pots belong to the opponent's field.\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "\tIf one of these collections would result in the opponent losing all of his beans, the move is invalid, and cannot be performed.\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "\tSince the game is played with 48 beans, capturing 25 is enough to win the game.\n";
	cout << "\tAs there is an even number of beans, it is also possible for the game to end in a draw, if each player captures exactly 24 beans.\n";
	cout << "\n\tPress Enter to continue...";

	cin.getline(leaveInput, 50, '\n');

	system("cls");

	cin.clear();
}

//function to select the game mode (basically a copy of the inputManagerFunction)
int gameModeSelector(bool& opponentIsCPU) {


	char playerInput[50];		//Player input
	int control = 1;			//Control value of sscanf (only correct if 1)
	int gameMode;

	cout << "Please input the number corresponding to a game mode:\n";
	cout << "0 - Player vs. Player\n";
	cout << "1 - Player vs. CPU - Easy (The CPU's selection is random)\n";
	cout << "2 - Player vs. CPU - Normal (The CPU selects the pot that will give him the maximum score possible, then chooses randomly from the highest scores)\n";

	do {
		if (control != 1) {															//Condition is always false during the first execution of
			cout << "Invalid input. You must input a valid game mode.\n";			//the loop. 
		}																			//If statement to display error message, if necessary

		cout << "\nPlease input a number: ";									//Asking for input

		memset(&playerInput, 0, 50);												//clean buffer

		cin.getline(playerInput, 50, '\n');											//Save input in temporary variable

		control = sscanf(playerInput, "%d\n", &gameMode);							//Get control value from the input. 1 is desirable

		if (control != 1) { continue; }												//in case the input is a letter, to avoid reading pot in the next loop

		if (gameMode < 0 || gameMode > 2)											//If the input is valid, but it does not correspond to any
		{																			//of the game modes
			control = -1;															//Changes control to -1, in order to reenter the loop
			continue;																//So it doesn't access the array with a weird value
		}

		if (control != 1) { continue; }												//in case the input is a letter, to avoid reading pot in the next loop

	} while (control != 1);															//Condition for while

	if (gameMode != 0)
	{
		opponentIsCPU = true;
	}

	return gameMode;
}

// function to display the board
void displayBoard(beanRelatedVariables& beanVariables)
{
	cout << "Player A's beans: " << beanVariables.aBeans << "\n";
	cout << "Player B's beans: " << beanVariables.bBeans << "\n";


	//START OF PLAYER B'S PART OF THE BOARD
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);						//changes colour of text to dark grey
	cout << "\n	|-----------------------------|\n	| ";								//separator 1

	//displays upper column numbers
	for (int i = 6; i >= 1; i--)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);			//change colour to white
		cout << '0' << i;																//print out column number
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);			//change back to gray to finish the "structure" of the board
		cout << " | ";															//number separator
	}

	cout << "\n	|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|\n	| ";			//separator 2

	for (int i = 11; i >= 6; i--)		//displays player B's beans on board
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);			//change colour to red
		cout << std::setfill('0') << std::setw(2);
		cout << beanVariables.pots[i];												//print out beans in pot
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);			//change back to gray to finish the "structure" of the board
		cout << " | ";															//number separator
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);				//change colour to red
	cout << " Player B's board";												//print out player B's board identifier
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);				//change back to gray to finish the "structure" of the board
	cout << " |\n";																//separator and new line
	//END OF PLAYER B'S HALF OF THE BOARD

	cout << "	|-----------------------------|-------------------|\n	| ";			//middle separator

	//START OF PLAYER A'S HALF OF THE BOARD
	for (int i = 0; i <= 5; i++)	//displays player A's beans on board
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);			//change colour to blue
		cout << std::setfill('0') << std::setw(2);
		cout << beanVariables.pots[i];												//print out beans in pot
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);			//change back to gray to finish the "structure" of the board
		cout << " | ";															//number separator
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);				//change colour to blue
	cout << " Player A's board";												//print out player A's board identifier
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);				//change back to gray to finish the "structure" of the board
	cout << " |\n";																//separator and new line

	cout << "	|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|<-->|\n	| ";			//separator 3

	//displays upper column numbers
	for (int i = 1; i <= 6; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);			//change colour to white
		cout << '0' << i;																//print out column number
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);			//change back to gray to finish the "structure" of the board
		cout << " | ";															//number separator
	}

	cout << "\n	|-----------------------------|\n\n";									//separator 4
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);				//changes back to white
	//END OF PLAYER A'S HALF OF THE BOARD
}

//function to ask for and receive the player's input
int inputManager(bool playerIsA, string turnPlayer, int offset, beanRelatedVariables& beanVariables) {

	string opponent;

	if (playerIsA) { opponent = "Player B"; }
	else if (!playerIsA) { opponent = "Player A"; }

	char playerInput[50];		//Player input
	int control = 1;			//Control value of sscanf (only correct if 1)
	int pot;					//Input string (player selected pot)

	bool opponentBeansAreZero;	//Bool that signals if the opponent currently has no beans on the board

	switch (beanVariables.opponentBeans)	//gives bool a value according to the number of beans on the opponent's board
	{
	case 0:
		opponentBeansAreZero = true;
		break;
	default:
		opponentBeansAreZero = false;
		break;
	}

	cout << turnPlayer << "'s turn. ";	//Displays whose turn it is

	if (opponentBeansAreZero) {			//Informs player he must allow his opponent to conduct his next move
		cout << "\nSince " << opponent << " has no beans on his side of the board, the current player must select a pot that will";
		cout << " give the opponent at least one seed to play with on his next turn.\n";
	}

	if (beanVariables.vsCPU == true && !playerIsA) {	//if it's pvCPU mode and player B's (the CPU's) turn

		if (beanVariables.difficulty == 1) {			// if the difficulty is set to easy, chooses the input randomly, by trial and error
			do {
				srand(time(0));												//randomizes seed
				pot = 1 + (rand() % 6);										//gets random number from 1 to 6 and assigns it to pot
			} while (beanVariables.pots[6 + pot - 1] == 0);					//repeats until the cpu chooses a pot that is not empty
		}
		else if (beanVariables.difficulty == 2) {	// if the difficulty is set to hard, calls the function to evaluate every possible move

			pot = inputCPU(beanVariables);
		}
		
		cout << "\nThe cpu selected pot number " << pot << "\n";			// displays which pot the cpu selected
		Sleep(3000);														// slight pause
	}
	else		// in case it is playerA's turn or the difficulty is set to player vs player (0)
	{
		do {
			if (control != 1) {															//Condition is always false during the first execution of the loop
				system("cls");															//Clears console										
				displayBoard(beanVariables);											//Redraws the board
				cout << "Invalid input. The input must be an integer between 1 and 6, whose respective pot is not empty.\n";	//Displays error message
			}																			

			cout << "\nIf you wish to revisit the rules screen, you can input 0. If something went wrong and you wish to force a tie, you can do so by inputting - 1.\n";
			cout << "\nPlease choose one of your pots: ";								//Asking for input

			memset(&playerInput, 0, 50);												//clean buffer

			cin.getline(playerInput, 50, '\n');											//Save input in temporary variable

			control = sscanf(playerInput, "%d\n", &pot);								//Get control value from the input. 1 is desirable

			if (control != 1) { continue; }												//in case the input is a letter, to avoid reading pot in the next loop

			if (pot == 0)			//In case the player wishes to revisit the rules
			{
				control = -1;
				system("cls");
				rules();
				continue;
			}

			if (pot == -1)			//In case of a forced draw
			{
				system("cls");
				return -1;			//Returns a control value
			}

			if (pot < 1 || pot > 6) {													//If input is valid, but is >6 or <1,
				control = -1;															//Changes control to -1, in order to reenter the loop
				continue;																//So it doesn't access the array with a weird value
			}

			if (beanVariables.pots[pot + offset - 1] == 0) {							//if the pot is empty
				cout << "This pot is empty. ";
				control = -1;
				continue;
			}

			if (opponentBeansAreZero && (beanVariables.pots[pot + offset - 1] > 6 - (pot - offset)))	//If the beans in the pot are not enough to reach the opponent's
			{																						//board when he has no beans,
				cout << "You must select a pot that will give your opponent at least one bean to play with, on his next turn. \n";
				control = -1;
				continue;
			}

		} while (control != 1);															//Condition for while
	}


	cout << turnPlayer << " chose pot number " << pot << "\n";			//Displays which number was chosen

	return pot;
}

//function that manages the sowing phase
int gameController(int beansToMove, int currentPot, beanRelatedVariables& beanVariables) {

	int originalPot = currentPot;					//temp variable to control where to skip the sowing, if needed
	currentPot++;									//move to the first pot to be sowed

	//CHANGING THE POTS' VALUES. AKA. "THE GAME"
	while (beansToMove > 0) {						//While the player still has beans to sow

		if (currentPot == 12) {								//If the pot being sown is supposed to be
			currentPot = 0;									//the one in the position 0 of the array,
		}													//set potInArray to the correct value

		if (currentPot == originalPot)				//If the pot being sown is the starting pot
		{
			currentPot++;							//sows the bean in the next one
		}

		beanVariables.pots[currentPot]++;								//sows one bean into the pot
		currentPot++;												//moves on to the next pot
		beansToMove--;												//removes one bean from the player's beans to sow

		Sleep(500);												//slight pause for the sake of aesthetics
		system("cls");												//Clear console before every turn (to display the board afterwards and "update" the gamestate)
		displayBoard(beanVariables);	//Display starting board/board after every change
	}
	currentPot--;											//subtract 1, to get the value of the last pot to get
	return currentPot;										//sowed (the variable increases by one at the end of the while loop)
}

//function to check if the player is allowed to collect the opponent's beans
bool canCollect(bool playerAturn, int currentpot, int offset, beanRelatedVariables& beanVariables) {

	int collectedBeans = 0;

	do {	//this function is only called if the last pot to be sown has either 2 or 3 beans

		if (playerAturn && currentpot > 5) {			//If it's pA's turn and the pot belongs to pB
			collectedBeans += beanVariables.pots[currentpot];

		}
		else if (!playerAturn && currentpot < 6) {		//If it's pB's turn and the pot belongs to pA
			collectedBeans += beanVariables.pots[currentpot];
		}
		currentpot--;																		//gets the index of the previous pot
	} while (beanVariables.pots[currentpot] == 2 || beanVariables.pots[currentpot] == 3);	//in case the player can still score

	if (collectedBeans == beanVariables.opponentBeans) {
		return false;
	}
	else
	{
		return true;
	}
}

//function that manages the capturing phase
void scoreController(bool playerATurn, int currentPot, beanRelatedVariables& beanVariables) {

	do {	//if the player can score

		if (playerATurn && currentPot > 5) {	//If it's pA's turn and the pot belongs to pB

			beanVariables.aBeans += beanVariables.pots[currentPot];		//adds beans on pot to the player's score
			beanVariables.pots[currentPot] = 0;						//removes all beans from that pot
		}
		else if (!playerATurn && currentPot < 6) {					//
			beanVariables.bBeans += beanVariables.pots[currentPot];	//read above, but for player B
			beanVariables.pots[currentPot] = 0;						//
		}

		currentPot--;									//gets the index of the previous pot

		Sleep(500);										//slight pause for the sake of aesthetics
		system("cls");									//Clear console before every turn (to display the board afterwards and "update" the gamestate)
		displayBoard(beanVariables);

	} while (beanVariables.pots[currentPot] == 2 || beanVariables.pots[currentPot] == 3);	//in case the player can still score
}

//function to get the CPU's input, if the difficulty is set to hard
int inputCPU(beanRelatedVariables& beanVariables) {

	int testBoard[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	//Copy of the board array, for testing. Only the first 6 values are relevant

	int playValues[6];		/*This array saves the immediate value of the moves the CPU can make.
							-1 means the respective pot is empty,
							0 means the move will give the cpu no immediate advantage,
							and any other values represent the advantage gained*/

	// For loop to evaluate each possible move's values
	for (int i = 6; i < 12; i++) {

		if (beanVariables.pots[i] == 0) {	// If the pot is empty
			playValues[i - 6] = -1;			// Value is -1 (Prevents the CPU from choosing a pot with no beans)
			continue;
		}
		else if (beanVariables.pots[i] < (36 - i)%12)	// If the seeds in the pot make it land on one of the CPU's pots (36 because it's a large
		{												// number, and so, it's practically impossible to reach that number of beans in a single pot)

			playValues[i - 6] = 0;						// The value is 0
		}
		else		// If the CPU has enough seeds to land on one of player A's pots
		{

			// Loop to copy the current board to the test board:
			for (int k = 0; k < 12; k++)	
			{
				testBoard[k] = beanVariables.pots[k];
			}

			//Calls function which calculates the value of choosing the pot of index i
			playValues[i - 6] = getPlayValue(testBoard, beanVariables.pots[i], i); // Inserts that value into the playValues array
		}
	}

	std::vector<int> largestValueIndexes;
	int currentLargestValue = -1;

	//Loop to get the largest value in the playValues array
	for (int l = 0; l < 6; l++) {
		if (playValues[l] > currentLargestValue)
		{
			currentLargestValue = playValues[l];
		}
	}

	//Inserts the highest value(s)'s indices into the vector
	for (int m = 0; m < 6; m++) {
		if (playValues[m] == currentLargestValue) {
			largestValueIndexes.push_back(m);
		}
	}

	srand(time(0));	//Randomizes the seed

	int randomVectorIndex = rand() % largestValueIndexes.size();	//Randomly selects one of the indexes of the vector
	int randomCPUSelection = 1 + largestValueIndexes[randomVectorIndex];

	return randomCPUSelection;
}

int getPlayValue(int boardToBeTested[12], int beansToSow, int startingPot) {

	//PREPARING TO CHECK POINTS
	boardToBeTested[startingPot] = 0;					//set the starting pot to 0
	int currentPot = startingPot;						//temp variable to control where the sowing is happening
	currentPot++;										//move to the first pot to be sowed

	do {		//the player has at least 1 bean to sow

		if (currentPot == 12) {					//If the pot being sown is supposed to be
			currentPot = 0;						//the one in the position 0 of the array,
		}										//set potInArray to the correct value

		if (currentPot == startingPot)				//If the pot being sown is the starting pot
		{
			currentPot++;							//skips that one, and moves on to the next
		}

		boardToBeTested[currentPot]++;			//Sows one bean
		beansToSow--;							//Removes one bean from the player's beans to sow
		currentPot++;							//Moves on to the next pot
	} while (beansToSow > 0);		//While the player still has beans to sow

	currentPot--;											//subtract 1, to get the value of the last pot to get sown

	//CHECKING POINTS:
	int playScore = 0;

	while (boardToBeTested[currentPot] == 2 || boardToBeTested[currentPot] == 3) {

		if (currentPot == -1)
		{
			currentPot = 11;
		}

		if (currentPot < 6) {								// if the move can score
			playScore += boardToBeTested[currentPot];		// 
			boardToBeTested[currentPot] = 0;				//
		}

		currentPot--;									//gets the index of the previous pot
	}

	return playScore;
}

//function to get the value of all possible moves the cpu can make. called by the inputCPU function

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
