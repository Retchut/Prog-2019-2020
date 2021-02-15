#ifndef _PLAYER_H
#define _PLAYER_H

#include "Pool.h"

#include <map>
#include <string>
#include <vector>

using std::vector;

class Player
{
public:
	int playerPoints = 0;
	vector<char> hand;


	void addToHand(Pool& letterPool);

	void refillHand(Pool& letterPool, int amount);

	void exchangeLetters(Pool& letterPool, int numberToSwap);
};


#endif

