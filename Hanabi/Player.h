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
		CardKnowledgeBase CKB;
		theirHandKB.push_back(CKB);
	}
	else if (actionType == DISCARD){
		DiscardEvent * de = static_cast<DiscardEvent*>(e);
		discardPile.push_back(de->c);//records card in our discard pile
		int pos = de->position;
		if(de->wasItThisPlayer){//we discarded a card
			myHandKB.erase(myHandKB.begin() + pos);
		}
		else{//the other player discarded
			theirHandKB.erase(theirHandKB.begin() + pos);
		}
	}
	else if (actionType == PLAY){
		PlayEvent * pe = static_cast<PlayEvent *>(e);
		int pos = pe->position;
		if (pe->wasItThisPlayer){//we played
			myHandKB.erase(myHandKB.begin() + pos);
		}
		else{//other player played
			theirHandKB.erase(theirHandKB.begin() + pos);
		}

		if (!pe->legal){//play was not legal, card goes to discard pile
			discardPile.push_back(pe->c);
		}
	}
	else if (actionType == COLOR_HINT){
		ColorHintEvent * che = static_cast<ColorHintEvent *>(e);
		int color = che->color;
		for (int i = 0;i<myHandKB.size;i++){//update each kb in our hand
			
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

Event* Player::ask()
{
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