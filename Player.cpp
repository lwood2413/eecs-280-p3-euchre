#include "Player.hpp"
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;
class SimplePlayer : public Player {

public:

//Constructor for SimplePlayer class
SimplePlayer(const std::string &name_in) : name(name_in) {}

//Returns name
const std::string & get_name() const {
    return name;
}

void add_card(const Card &c){
    //Asserts that the player has less cards in hand than max
    assert(static_cast<int>(hand.size()) < MAX_HAND_SIZE);
    //Adds card c to player's hand
    hand.push_back(c);
}

bool make_trump(const Card &upcard, bool is_dealer,
int round, Suit &order_up_suit) const {
    //Asserts round is one or two
    assert(round == 1 || round == 2);

    Suit trump;
    //Simple player considers ordering up upcard in round 1
    if(round == 1){
        trump = upcard.get_suit();
    }
    //In round 2, they consider making the next suit trump
    else{
        trump = Suit_next(upcard.get_suit());
    }

    //Increments a count variable to later determine if player should order up
    int count = 0;
    for (size_t i = 0; i < hand.size(); ++i){
        if(hand[i].is_trump(trump) && hand[i].is_face_or_ace()){
            count++;
        }
    }

    bool order_up = false;
    //In round 1, will order up if 2 or more face trump cards
    if(round == 1){
        if(count >= 2){
            order_up = true;
        }
    }

 else{
    //In round 2, orders up if has more than one face trump card
    //Also orders up in the case of screw the dealer
    if(count >= 1 || is_dealer){
        order_up = true;
    }
  }
  //Sets order up suit to trump suit
 if(order_up){
    order_up_suit = trump; 
 }

 return order_up;
}


void add_and_discard(const Card &upcard){
    //Asserts hand is not empty
    assert(!hand.empty());
    //Adds upcard to hand
    hand.push_back(upcard);
    //Sets trump to upcard suit
    Suit trump = upcard.get_suit();

    size_t lowest = 0;
    //Determines which card is the lowest in hand
    for(size_t i = 0; i < hand.size(); ++i){
        if(Card_less(hand[i], hand[lowest], trump)){
            lowest = i;
        }
    }
    //Discards the lowest card
    hand.erase(hand.begin() + lowest);
}

Card lead_card(Suit trump){
    //Asserts hand is not empty
    assert(!hand.empty());

    //Creates variables for bet trump and non trump(-1 b/c not found yet)
    int best_non_trump = -1;
    int best_trump = -1;

    //Loops through hand
    for(size_t i = 0; i < hand.size(); ++i){
        //checks if current card is trump
        if(hand[i].is_trump(trump)){
            /*If no best trump or current card is greater
             than current best trump, sets best trump to current card */
            if(best_trump == -1 || 
                Card_less(hand[best_trump], hand[i], trump)){
                    best_trump = i;
              }
        }
        /*Otherwise, if not trump, checks if current card is
        greater than non trump, and if so, sets current card to best non trump*/
        else{
            if(best_non_trump == -1 ||
            Card_less(hand[best_non_trump], hand[i], trump)){
                best_non_trump = i;
            }
        }
    }

    int chosen;
    //Prefers to choose best non trump
    if(best_non_trump != -1){
        chosen = best_non_trump;
    }
    //If there is no best non trump, plays best trump
    else{
        chosen = best_trump;
    }
    Card c = hand[chosen];
    hand.erase(hand.begin() + chosen);
    return c;
}

Card play_card(const Card &led_card, Suit trump){
    //Asserts hand is not empty
    assert(!hand.empty());

    //Creates suit object for led card
    Suit led_suit = led_card.get_suit(trump);

    //Sets int for best follow card, -1 because does not exist yet
    int best_follow = -1;
    int lowest = 0;

    //Loops through hand
    for(size_t i = 0; i < hand.size(); ++i){
        /*If current card is the same suit of led card and no best follow card exists
        or current card is greater than current best follow, seets best follow
        to current card*/
        if(hand[i].get_suit(trump) == led_suit){
            if(best_follow == -1 || 
            Card_less(hand[best_follow], hand[i], led_card, trump)) {
                best_follow =i;
            }
        }
        /*Otherwise, chacks if current card is less than lowest card, 
        and sets lowest to current card*/
        if(Card_less(hand[i], hand[lowest], led_card, trump)) {
            lowest = i;
        }
    }

    int chosen;
    //If a best follow card exists, set chosen card to best follow
    if(best_follow != -1){
        chosen = best_follow;
    }
    else{
        //Otherwise sets chosen card to the lowest card
        chosen = lowest;
    }

    Card c = hand[chosen];
    //Removes the chosen card from hand
    hand.erase(hand.begin() + chosen);
    return c;
}

private:
string name;
vector <Card> hand;

};




class HumanPlayer : public Player {
public:

HumanPlayer(const std::string &name_in) : name(name_in) {}

const std::string & get_name() const{
    return name;
}

void add_card(const Card &c){
     //Asserts that the player has less cards in hand than max
    assert(static_cast<int>(hand.size()) < MAX_HAND_SIZE);
    //Adds card c to player's hand
    hand.push_back(c);
    //Sorts hand in ascending order
    sort(hand.begin(), hand.end());
}

bool make_trump(const Card &upcard, bool is_dealer,
int round, Suit &order_up_suit) const {
    //Prints player's hand for reference
    print_hand();
    //Prompts player to enter decision
    cout << "Human Player " << name << ", please enter a suit, or \"pass\":\n";

    string decision;
    //Reads in decision
    cin >> decision;

    //If player orders up, sets order up suit to trump and return true
    if(decision != "pass"){
        order_up_suit = string_to_suit(decision);
        return true;
    }
    else{
        return false;
    }
}

void add_and_discard(const Card &upcard){

    //Asserts hand is not empty
    assert(!hand.empty());

    //Prints hand for reference
    print_hand();

    //Prompts user to enter their choice
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n";

    int choice;

    //reads in choice
    cin >> choice;

    //If player makes a choice, adds upcard to hand and discards choice
    if(choice != -1){
        hand.erase(hand.begin() + choice);
        hand.push_back(upcard);

        //Sorts hand
        sort(hand.begin(), hand.end());
    }
    //if -1, upcard is discarded, hand does not chang
}

Card lead_card(Suit trump){

    //Prints player's hand for reference
    print_hand();

    //Prompts player to enter choice
    cout << "Human player " << name << ", please select a card:\n";

    int choice;
    //reads in choice
    cin >> choice;
    Card chosen = hand[choice];

    //Removes choice card from hand
    hand.erase(hand.begin() + choice);

    //Returns chosen card
    return chosen;
}

Card play_card(const Card &led_card, Suit trump){
    //Exact same implementation as lead_card
   return lead_card(trump);
}



private:
string name;
vector <Card > hand;

void print_hand() const {
    //Prints each card in player's hand
  for (size_t i=0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
}


};

Player * Player_factory(const std::string &name, const std::string &strategy){
    //If strategy is simple, creates a Simple player object
    if(strategy == "Simple"){
        return new SimplePlayer(name);
    }
    //If strategy is human, creates a human player object
    else if(strategy == "Human"){
        return new HumanPlayer(name);
    }
    //If strategy is invalid, error
    assert(false);
    return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

