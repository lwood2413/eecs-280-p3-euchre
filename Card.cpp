#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////

//Sets card to two of spades
Card::Card():rank(TWO), suit(SPADES) {}

//Sets rank and suit to respective inputs
Card::Card(Rank rank_in, Suit suit_in):
rank(rank_in), suit(suit_in) {}

//Gets the rank of the card
Rank Card::get_rank() const {
  return rank;
}

//Gets the suit of the card
Suit Card::get_suit() const{
  return suit;
}

//Get the suit of the card, accounts for trump
Suit Card::get_suit(Suit trump) const{
  //Checks if card is left bower, since it becomes trump
  if(is_left_bower(trump)){
    return trump;
  }
  else{
    return suit;
  }
}
  

bool Card::is_face_or_ace() const{
  //Checks if card is a face card
  if(rank == JACK || rank == QUEEN || rank == KING
  || rank == ACE){
    return true;
  }
  return false;
}

bool Card::is_left_bower(Suit trump) const{
  /*Checks if card is a jack and the next suit is trump,
  meaning the card is the left bower*/
  if(rank == JACK && Suit_next(suit) == trump){
    return true;
  }
  return false;
}

bool Card::is_right_bower(Suit trump) const{
  /* checks if the card is a jack in the trump suit,
  meaning it is a right bower */
  if(rank == JACK && suit == trump){
    return true;
  }
  return false;
}

bool Card::is_trump(Suit trump) const{
  //Checks if the suit of the card is the trump suit
  //Uses get suit accounting for trump
  return get_suit(trump) == trump;

}

std::ostream & operator<<(std::ostream &os, const Card &card){
  //Prints out card to os in form of "Rank of Suit"
  os << card.get_rank() << " of " << card.get_suit();
  
  return os;
}

std::istream & operator>>(std::istream &is, Card &card){
  //Declares a string to read "of" into
  std::string of;
  //Reads "Rank of "suit" into stream
  is >> card.rank >> of >> card.suit;

  return is;

}

bool operator<(const Card &lhs, const Card &rhs){
  //Checks which card has a higher rank(value)
  if(lhs.get_rank() < rhs.get_rank()){
    return true;
  }
  else{
    return false;
  }
}

bool operator<=(const Card &lhs, const Card &rhs){
  //Checks if lhs rank is less than or equal to rhs rank
  if((lhs < rhs) || (lhs.get_rank() == rhs.get_rank())){
    return true;
  }
  else{
    return false;
  }
}

bool operator>(const Card &lhs, const Card &rhs){
  //Since <= is already implemented, just checks if lhs is not <= rhs
  if(!(lhs <= rhs)){
    return true;
  }
  else{
    return false;
  }
}

bool operator>=(const Card &lhs, const Card &rhs){
  //Uses < to check if lhs is not < rhs, so must be >=
  if(!(lhs < rhs)){
    return true;
  }
  else{
    return false;
  }
}

bool operator==(const Card &lhs, const Card &rhs){
  //Checks if card ranks are equal
  if((lhs.get_rank() == rhs.get_rank()) && (lhs.get_suit() == rhs.get_suit())){
    return true;
  }
  else{
    return false;
  }
}

bool operator!=(const Card &lhs, const Card &rhs){
  //Uses == to check !=
  if(!(lhs == rhs)){
    return true;
  }
  else{
    return false;
  }
}

Suit Suit_next(Suit suit){
  //Checks each suit and returns next suit depending on suit input
  if(suit == CLUBS){
    return SPADES;
  }
  else if(suit == SPADES){
    return CLUBS;
  }
  else if(suit == HEARTS){
    return DIAMONDS;
  }
  else{
    return HEARTS;
  }

}

bool Card_less(const Card &a, const Card &b, Suit trump){
  //Checks if either card is right bower
  if(a.is_right_bower(trump)){
    return false;
  }
  if(b.is_right_bower(trump)){
    return true;
  }
  //Checks if either card is left bower
  if(a.is_left_bower(trump)){
    return false;
  }
  if(b.is_left_bower(trump)){
    return true;
  }
  //Tests if one card is trump and the other is not
  if(a.is_trump(trump) && !b.is_trump(trump)){
    return false;
  }
  if(!a.is_trump(trump) && b.is_trump(trump)){
    return true;
  }
  //If none above apply, resorts to oveloaded < operator
  return a < b;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump){
  //If one card is trump, calls card_less(card a, card b, trump)
  if(a.is_trump(trump) || b.is_trump(trump)){
    return Card_less(a, b, trump);
  }
  //Checks if either card is the led suit
  if(a.get_suit() == led_card.get_suit() && b.get_suit() != led_card.get_suit()){
    return false;
  }
  if(a.get_suit() != led_card.get_suit() && b.get_suit() == led_card.get_suit()){
    return true;
  }
  //If none above apply, resorts to overloaded < operator
  return a < b;
}
// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
