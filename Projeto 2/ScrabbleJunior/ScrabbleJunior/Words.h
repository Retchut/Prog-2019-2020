#ifndef _WORDS_H
#define _WORDS_H

#include <string>
#include <vector>

using std::pair;
using std::string;
using std::vector;

class Word {
public:

	Word(string position, string orientation, string word);
	
	int length = 0;

	//positions of the words letters, which haven't been played in
	vector<pair <char,char>> positionsLeft;
	//letters of the word that have not been yet played
	vector<char> lettersLeft;

	//char of the next letter of the word that can be played
	char playableLetter;
	//pair containing the position where the playableLetter can be played in
	pair<char, char> playablePosition;
};

#endif
