#include "Pool.h"

#include <iostream>
#include <Windows.h>

using namespace std;

////function to add letters to the pool
//void Pool::addToPool(std::string toAdd) {
//
//	//loop to add each letter on the toAdd string to the letterPool
//	for (size_t i = 0; i < toAdd.length(); i++) {
//		poolPieces.push_back(toAdd[i]);
//	}
//}

//overload for use with the exchangeLetters function
void Pool::addToPool(char toAdd) {

	poolPieces.push_back(toAdd);
}

//void Pool::addToPool(Matrix& board) {
//	for (auto line : board.board) {
//		for (auto column : line) {
//			if (column != ' ') {
//				poolPieces.push_back(column);
//			}
//		}
//	}
//
//	cout << endl << endl << "Pool pieces: ";
//	for (size_t i = 0; i < poolPieces.size(); i++) {
//		cout << poolPieces[i] << " ";
//	}
//	cout << endl;
//	
//	Sleep(1000000);
//}