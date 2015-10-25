#include <vector>

using namespace std;

#ifndef CARDKNOWLEDGEBASE_H
#define CARDKNOWLEDGEBASE_H

class CardKnowledgeBase{
public:
	//default constructor
	CardKnowledgeBase();
	CardKnowledgeBase(const CardKnowledgeBase& CKB);

	void setColor(int c);
	void setNumber(int n);

	void removePossibleColor(int c);
	void removePossibleNumber(int n);

	bool couldBeColor(int c);
	bool couldBeNumber(int n);

	vector<int> possibleColors;
	vector<int> possibleNumbers;
};

CardKnowledgeBase::CardKnowledgeBase(){
		for (int i = 0;i< 5;i++){
			possibleColors.push_back(i);
			possibleNumbers.push_back(i+1);
		}
}

CardKnowledgeBase::CardKnowledgeBase(const CardKnowledgeBase& CKB){
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
	for (int i = 0;i< possibleColors.size;i++){
		if (possibleColors[i] == c)
			possibleColors.erase(possibleColors.begin + i);
	}
}

void CardKnowledgeBase::removePossibleNumber(int n){
		for (int i = 0;i< possibleNumbers.size;i++){
		if (possibleNumbers[i] == n)
			possibleNumbers.erase(possibleNumbers.begin + i);
	}
}

bool CardKnowledgeBase::couldBeColor(int c){
	for (int i = 0;i<possibleColors.size;i++){
		if (possibleColors[i] == c)
			return true;
	}

	return false;
}

bool CardKnowledgeBase::couldBeNumber(int n){
	for (int i = 0;i<possibleNumbers.size;i++){
		if (possibleNumbers[i] == n)
			return true;
	}

	return false;
}

#endif