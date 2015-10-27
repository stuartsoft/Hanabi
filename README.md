# Hanabi
A knowledge based AI agent to play the Hanabi cardgame with itself

###Logic
This agent works under the principal of knowledge based AI and utilizes a targeted hint system to only hint cards to the opponent that are currently playable. Each player keeps a knowledge base record for each individual card in their own hand as well as a knowledge base record of what their opponent knows. This allows for a more intelligent hint selection and prevents duplicate hinting. Additionally, the agent is able to dynamically switch between making risky moves and making safe moves based on the progress of the game, which allows the ai a wider bredth of choices to make during any given round.

###Setup
Recommended to set Visual Studio for release mode and launch using Ctrl + F5 to halt after running.

Additionally, the number of game iterations can be dictated in Hanabi.cpp

    14  	int iterations = 1000;
