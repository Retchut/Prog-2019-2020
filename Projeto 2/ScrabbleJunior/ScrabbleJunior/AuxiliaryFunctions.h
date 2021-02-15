#ifndef _AUXILIARYFUNCTIONS_H
#define _AUXILIARYFUNCTIONS_H

#include "Player.h"
#include "Matrix.h"

#include <string>
#include <vector>

using std::string;
using std::vector;


string getInput(string message);

int getPlayerNum();

string getFileName();

vector<string> splitStringBySpace(string toSplit);

bool testBoardValidity(string fileName, int playerNumber);

bool canPlay(Player turnPlayer, Matrix board);
bool canPlay(char selectedLetter, Matrix board, Player turnPlayer);

char getLetterToPlay(Matrix board, string message, Player turnPlayer);

std::pair<char,char> getPositionToPlayIn(Matrix board, char selectedLetter);

char getLetterToExchange(string message, Player turnPlayer);

bool testEndGame(Matrix board);

pair<bool, vector<int>> checkForTie(vector<Player> playerVector);
#endif


