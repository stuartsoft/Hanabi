#include <vector>

using namespace std;

#ifndef CARDKNOWLEDGEBASE_H
#define CARDKNOWLEDGEBASE_H

class CardKnowledgeBase{
public:
	//default constructor
	CardKnowledgeBase();
	CardKnowledgeBase(const CardKnowledgeBase& CKB);

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

#endif