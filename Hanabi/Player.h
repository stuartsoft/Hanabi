#include "GameConstants.h"
#include "Card.h"
#include "Events.h"
#include <vector>
#include <iostream>
#include "CardKnowledgeBase.h"

using std::vector;
using std::cout;
using std::endl;

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	Player();
	Player(const Player& p);
	void tell(Event* e, vector<int> board, int hints, int fuses, vector<Card> oHand, int deckSize);
	Event* ask();
private:
	int fuses;
	int hints;
	int deckSize;
	vector<Card> discardPile;
	vector<Card> oHand;
	vector<int> board;
	vector<CardKnowledgeBase> myHandKB;
	vector<CardKnowledgeBase> theirHandKB;
};

Player::Player()
{
	for (int i = 0;i<HAND_SIZE;i++){//initialize my knowledgebase for a default hand
		CardKnowledgeBase CKB = CardKnowledgeBase();
		myHandKB.push_back(CKB);
	}
}

Player::Player(const Player& p)
{
}

void Player::tell(Event* e, vector<int> board, int hints, int fuses, vector<Card> oHand, int deckSize)
{
	//store info provided by tell()
	this->hints = hints;
	this->fuses = fuses;
	this->deckSize = deckSize;
	this->oHand = oHand;
	this->board = board;

	int actionType = e->getAction();
	
	if (actionType == DRAW){
		CardKnowledgeBase CKB = CardKnowledgeBase();
		theirHandKB.push_back(CKB);
	}
	else if (actionType == DISCARD){
		DiscardEvent * de = static_cast<DiscardEvent*>(e);
		discardPile.push_back(de->c);//records card in our discard pile
		int pos = de->position;
		if(de->wasItThisPlayer){//we discarded a card
			myHandKB.erase(myHandKB.begin() + pos);
			if (deckSize > 0){
				CardKnowledgeBase CKB = CardKnowledgeBase();
				myHandKB.push_back(CKB);
			}
		}
		else{//the other player discarded
			theirHandKB.erase(theirHandKB.begin() + pos);
		}
	}
	else if (actionType == PLAY){

		for (int i = 0;i<myHandKB.size();i++){//clear all recently hinted flags
			myHandKB[i].wasRecentlyHinted = false;
		}

		PlayEvent * pe = static_cast<PlayEvent *>(e);
		int pos = pe->position;
		if (pe->wasItThisPlayer){//we played
			myHandKB.erase(myHandKB.begin() + pos);
			if (deckSize > 0){
				CardKnowledgeBase CKB = CardKnowledgeBase();
				myHandKB.push_back(CKB);
			}
		}
		else{//other player played
			theirHandKB.erase(theirHandKB.begin() + pos);
		}

		if (!pe->legal){//play was not legal, card goes to discard pile
			discardPile.push_back(pe->c);
		}
	}
	else if (actionType == COLOR_HINT){
		for (int i = 0;i<myHandKB.size();i++){//clear all recently hinted flags
			myHandKB[i].wasRecentlyHinted = false;
		}
		ColorHintEvent * che = static_cast<ColorHintEvent *>(e);
		int color = che->color;
		for (int i = 0;i<myHandKB.size();i++){//update each kb in our hand
			bool cardMatch = false;//card i was hinted
			for (int j = 0;j<che->indices.size();j++){
				if (i==che->indices[j]){
					cardMatch = true;
					break;
				}
			}

			if (cardMatch){//we were given a hint for this card
				myHandKB[i].setColor(color);//remove all possible colors except this color
				myHandKB[i].wasRecentlyHinted = true;
			}
			else{
				myHandKB[i].removePossibleColor(color);//this card cannot be this color because it was not given a hint
			}
		}
	}
	else if (actionType == NUMBER_HINT){
		for (int i = 0;i<myHandKB.size();i++){//clear all recently hinted flags
			myHandKB[i].wasRecentlyHinted = false;
		}
		NumberHintEvent * nhe = static_cast<NumberHintEvent *>(e);
		int number = nhe->number;
		for (int i = 0;i<myHandKB.size();i++){
			bool cardMatch = false;
			for (int j = 0;j<nhe->indices.size();j++){
				if (i==nhe->indices[j]){
					cardMatch = true;
					break;
				}
			}

			if (cardMatch){
				myHandKB[i].setNumber(number);
				myHandKB[i].wasRecentlyHinted = true;
			}
			else{
				myHandKB[i].removePossibleNumber(number);
			}
		}
	}

	/* Possible kinds of event:
		DiscardEvent - can be for us or other player
			c - the card discarded
			wasItThisPlayer - true if we discarded, false otherwise
			position - the index in hand of the discarded card (0 base)
		ColorHintEvent - always for other player
			indices - all indices of the chosen color
			color - the color in question
		NumberHintEvent - always for the other player
			indices - all indices of the chosen number
			color - the number in question
		PlayEvent - can be for us or other player
			position - the index in hand of the discarded card (0 base)
			c - the card played
			legal - whether the card was a legal play
			wasItThisPlayer - true if we discarded, false otherwise
	*/
	
}

Event* Player::ask()//actual AI
{
	int hintTally = 0;
	int hintIndex = 0;
	for (int i = 0;i<myHandKB.size();i++){
		if (myHandKB[i].wasRecentlyHinted){
			hintTally++;
			hintIndex = i;
		}
	}
	if (hintTally == 1){//a single card was hinted, therefore we know it's safe to play, so play it
		PlayEvent * PE = new PlayEvent();
		PE->position = hintIndex;
		return PE;//play this card if we know color and num and it is playable
	}

	vector<Card> neededCards;
	for (int i = 0;i<board.size();i++){
		if (board[i]<NUM_NUMBERS){
			Card neededCard(i,board[i]+1);
			neededCards.push_back(neededCard);
		}		
	}

	//play actions
	for (int i = 0;i<myHandKB.size();i++){
		if (myHandKB[i].knowsColor() && myHandKB[i].knowsNumber()){
			for (int j = 0;j<neededCards.size();j++){
				if (myHandKB[i].possibleColors[0] == neededCards[j].color && myHandKB[i].possibleNumbers[0] == neededCards[j].number){
					PlayEvent * PE = new PlayEvent();
					PE->position = i;
					//cout<< "!!!Playing card index ("<<i<<") with color ("<<myHandKB[i].possibleColors[0]<<") and num ("<<myHandKB[i].possibleNumbers[0]<<")"<<endl;
					return PE;//play this card if we know color and num and it is playable
				}
			}
		}
	}

	if (hints > 0){//we have hints available. Only hint cards that are currently playable

		//we now have a list of the cards we could play
		vector<int> oHandPlayableCards;//indicies of playable cards in the opponent's hand
		for (int i = 0;i<oHand.size();i++){
			for (int j = 0;j<neededCards.size();j++){
				if (oHand[i] == neededCards[j]){//a card that can be played is in the opponent's hand.
					oHandPlayableCards.push_back(i);
					break;
				}
			}
		}

		for (int i = 0;i<oHandPlayableCards.size();i++){
			if (theirHandKB[oHandPlayableCards[i]].knowsColor() && theirHandKB[oHandPlayableCards[i]].knowsNumber()){
				oHandPlayableCards.erase(oHandPlayableCards.begin()+i);
			}
			else if (theirHandKB[oHandPlayableCards[i]].knowsColor()){//then hint the number
				NumberHintEvent* NHE = new NumberHintEvent();
				NHE->number = oHand[oHandPlayableCards[i]].number;

				for (int j = 0;j<theirHandKB.size();j++){//update our copy of the opponent's kb
					if (oHand[j].number == NHE->number){
						theirHandKB[j].setNumber(NHE->number);
					}
					else{
						theirHandKB[j].removePossibleNumber(NHE->number);
					}
				}

				return NHE;//return the number hint event
			}
			else if (theirHandKB[oHandPlayableCards[i]].knowsNumber()){//then hint the color
				ColorHintEvent* CHE = new ColorHintEvent();
				CHE->color = oHand[oHandPlayableCards[i]].color;

				for (int j = 0;j<theirHandKB.size();j++){//update our copy of the opponent's kb
					if (oHand[j].color == CHE->color){
						theirHandKB[j].setColor(CHE->color);
					}
					else{
						theirHandKB[j].removePossibleColor(CHE->color);
					}
				}

				return CHE;
			}
		}

		//if no cards were yet hinted at...
		vector<int> possibleColorHints;
		vector<int> possibleNumberHints;
		for (int i = 0;i<oHandPlayableCards.size();i++){
			int colorTally = 0;
			int numTally = 0;
			for (int j = 0;j<oHand.size();j++){
				if (oHand[j].color == oHand[oHandPlayableCards[i]].color)
					colorTally++;
				if (oHand[j].number == oHand[oHandPlayableCards[i]].number)
					numTally++;
			}
			possibleColorHints.push_back(colorTally);
			possibleNumberHints.push_back(numTally);
		}

		int minColorIndex = 0;
		for (int i = 0;i< possibleColorHints.size();i++){
			if (possibleColorHints[i] < possibleColorHints[minColorIndex])
				minColorIndex = i;
		}

		int minNumIndex = 0;
		for (int i = 0;i<possibleNumberHints.size();i++){
			if (possibleNumberHints[i] < possibleNumberHints[minNumIndex])
				minNumIndex = i;
		}

		//we now have the min values for both vectors

		if (possibleColorHints.size() >0 && possibleNumberHints.size() > 0){
			if (possibleColorHints[minColorIndex] < possibleNumberHints[minNumIndex]){
				ColorHintEvent* CHE = new ColorHintEvent();
				CHE->color = oHand[oHandPlayableCards[minColorIndex]].color;

				for (int j = 0;j< theirHandKB.size();j++){//update our copy of the opponent's kb
					if (oHand[j].color == CHE->color){
						theirHandKB[j].setColor(CHE->color);
					}
					else{
						theirHandKB[j].removePossibleColor(CHE->color);
					}
				}

				return CHE;
			}
			else{
				NumberHintEvent* NHE = new NumberHintEvent();
				NHE->number = oHand[oHandPlayableCards[minNumIndex]].number;

				for (int j = 0;j< theirHandKB.size();j++){//update our copy of the opponent's kb
					if (oHand[j].number == NHE->number){
						theirHandKB[j].setNumber(NHE->number);
					}
					else{
						theirHandKB[j].removePossibleNumber(NHE->number);
					}
				}

				return NHE;
			}
		}
	}

	//no hints were given

	//attempt to discard an unuseful card
	vector<int> cardKnowledgeScore;
	for (int i = 0;i< myHandKB.size();i++){
		int score = 0;
		score = myHandKB[i].possibleColors.size() * myHandKB[i].possibleColors.size();
		cardKnowledgeScore.push_back(score);

		if (myHandKB[i].knowsColor() && myHandKB[i].knowsNumber()){
			int color = myHandKB[i].possibleColors[0];
			int val = myHandKB[i].possibleNumbers[0];
			if (board[color] > val){
				//card already exists on the field. Discard
				DiscardEvent * DE = new DiscardEvent();
				DE->position = i;
				return DE;//discard the card we know the least about
			}
		}

	}

	int CKS_HighScore = 0;//index of highest score
	for (int i = 1;i<cardKnowledgeScore.size();i++){
		if (cardKnowledgeScore[i] > cardKnowledgeScore[CKS_HighScore])
			CKS_HighScore = i;		
	}

	//CKS_HighScore is the index of the card in our hand with the worst knowledge score, discard it
	DiscardEvent * DE = new DiscardEvent();
	DE->position = CKS_HighScore;
	return DE;//discard the card we know the least about

	/* You must produce an event of the appropriate type. Not all member
		variables of a given event type need to be filled in; some will be
		ignored even if they are. Summary follows.
	Options:
		ColorHintEvent - you must declare a color; no other member variables
			necessary.
		NumberHintEvent - you must declare a number; no other member variables
			necessary.
		PlayEvent - you must declare the index to be played; no other member
			variables necessary.
		DiscardEvent - you must declare the index to be discarded; no other
			member variables necessary.
	*/
}
#endif