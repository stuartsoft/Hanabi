/*	Mike Boom & Stuart Bowman 
This is the Card Knowledgebase class. It represents the knowledgebase of an individual card in either the player or the opponent's hand. It also contains a set of helper functions to reduce clutter and make the ai logic a bit easier to read
*/

#include <vector>

using namespace std;

#ifndef CARDKNOWLEDGEBASE_H
#define CARDKNOWLEDGEBASE_H

class CardKnowledgeBase{
public:
	//default constructor
	CardKnowledgeBase();

	void setColor(int c);
	void setNumber(int n);

	void removePossibleColor(int c);
	void removePossibleNumber(int n);

	bool couldBeColor(int c);
	bool couldBeNumber(int n);

	int mustBeColor();
	int mustBeNumber();

	bool knowsColor();
	bool knowsNumber();

	vector<int> possibleColors;
	vector<int> possibleNumbers;

	bool wasRecentlyHinted;
};

CardKnowledgeBase::CardKnowledgeBase(){
	for (int i = 0;i< NUM_COLORS;i++){
		possibleColors.push_back(i);
	}
	for (int i = 0; i< NUM_NUMBERS;i++){
		possibleNumbers.push_back(i+1);
	}

	wasRecentlyHinted = false;
}

void CardKnowledgeBase::setColor(int c){
	possibleColors.clear();
	possibleColors.push_back(c);
}

void CardKnowledgeBase::setNumber(int n){
	possibleNumbers.clear();
	possibleNumbers.push_back(n);
}

void CardKnowledgeBase::removePossibleColor(int c){
	for (int i = 0;i< possibleColors.size();i++){
		if (possibleColors[i] == c)
			possibleColors.erase(possibleColors.begin() + i);
	}
}

void CardKnowledgeBase::removePossibleNumber(int n){
		for (int i = 0;i< possibleNumbers.size();i++){
		if (possibleNumbers[i] == n)
			possibleNumbers.erase(possibleNumbers.begin() + i);
	}
}

bool CardKnowledgeBase::couldBeColor(int c){
	for (int i = 0;i<possibleColors.size();i++){
		if (possibleColors[i] == c)
			return true;
	}

	return false;
}

bool CardKnowledgeBase::couldBeNumber(int n){
	for (int i = 0;i<possibleNumbers.size();i++){
		if (possibleNumbers[i] == n)
			return true;
	}

	return false;
}

int CardKnowledgeBase::mustBeColor(){
	if (possibleColors.size() ==1 )
		return possibleColors[0];
	return -1;
}

int CardKnowledgeBase::mustBeNumber(){
	if (possibleNumbers.size() == 1)
		return possibleNumbers[0];
	return -1;
}

bool CardKnowledgeBase::knowsColor(){
	return (possibleColors.size() ==1);
}

bool CardKnowledgeBase::knowsNumber(){
	return (possibleNumbers.size() ==1);
}

#endif