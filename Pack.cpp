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