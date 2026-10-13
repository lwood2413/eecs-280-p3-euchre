#include "Pack.hpp"
#include <iostream>
#include <string>
Pack::Pack(): next(0) { //Next() helps with dealing cards, so don't have to delete
int i = 0;
//Loops throuh suits by enum values
for(int s = SPADES; s <= DIAMONDS; ++s){
    //Loops through rank by enum values
    for(int r = NINE; r <= ACE; ++r){
        //Used to change r and s from ints to rank and suit types for constructor
        //Sets current index to that card
       cards[i] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
       ++i;
    }
}
}

Pack::Pack(std::istream& pack_input): next(0) {
//Loops through size of pack, reading in from file the card for each index
for(int i = 0; i < PACK_SIZE; ++i){
    pack_input >> cards[i];
}
}

Card Pack::deal_one(){
    //Sets temp card object
    Card dealt = cards[next];
    //Increments next
    next++;
    return dealt;
}

void Pack::reset(){
    //Sets next to first index of pack
    next = 0;
}

void Pack::shuffle(){
//Constants for number of shuffles and half of deck
const int SHUFFLE_COUNTER = 7;
const int HALF = PACK_SIZE / 2;

//Performs 7 in-shuffles
for(int round = 0; round < SHUFFLE_COUNTER; ++round){
    //New array to store shuffled cards
    std::array <Card, PACK_SIZE> shuffled;

    for(int i = 0; i < HALF; i++){
        //Bottom half of cards
        shuffled[2 * i] = cards[HALF + i];
        //Top half of cards
        shuffled[2 * i + 1] = cards[i];
    }
    //Sets cards equal to the shuffle array
    cards = shuffled;
}
//Resets next index to new first card
reset();
}

bool Pack::empty() const{
    // Checks if the next card is outside of pack, meaning no cards remain
    if(next >= PACK_SIZE){
        return true;
    }
    else{
        return false;
    }
}