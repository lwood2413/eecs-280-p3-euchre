#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>

using namespace std;

//Tests constructor with rank and suit inputs
TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

//Tests default constructor
TEST(test_card_default_ctor) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

//Tests when left bower is considered trump suit
TEST(test_get_suit_trump_left_bower) {
    Card c(JACK, HEARTS);
    ASSERT_EQUAL(DIAMONDS, c.get_suit(DIAMONDS));
}
//Tests with right bower
TEST(test_get_suit_trump_right_bower) {
    Card c(JACK, SPADES);
    ASSERT_EQUAL(SPADES, c.get_suit(SPADES));
}
//Tests with non-Trump
TEST(test_get_suit_trump_non_trump) {
    Card c(NINE, HEARTS);
    ASSERT_EQUAL(HEARTS, c.get_suit(CLUBS));
}
//Tests with non-bower jack
TEST(test_get_suit_trump_non_bower) {
    Card c(JACK, CLUBS);
    ASSERT_EQUAL(CLUBS, c.get_suit(HEARTS));
}

//Tests for each valid euchre card
TEST(test_is_face_or_ace) {
    Card nine(NINE, HEARTS);
    Card ten(TEN, DIAMONDS);
    Card jack(JACK, SPADES);
    Card queen(QUEEN, CLUBS);
    Card king(KING, HEARTS);
    Card ace(ACE, DIAMONDS);

    ASSERT_EQUAL(false, nine.is_face_or_ace());
    ASSERT_EQUAL(false, ten.is_face_or_ace());
    ASSERT_EQUAL(true, jack.is_face_or_ace());
    ASSERT_EQUAL(true, queen.is_face_or_ace());
    ASSERT_EQUAL(true, king.is_face_or_ace());
    ASSERT_EQUAL(true, ace.is_face_or_ace());
}

//Tests for true and false
TEST(test_is_right_bower) {
    Card t(JACK, HEARTS);
    Card f(JACK, DIAMONDS);

    ASSERT_EQUAL(true, t.is_right_bower(HEARTS));
    ASSERT_EQUAL(false, f.is_right_bower(HEARTS));
}

//Tests for true and false;
TEST(test_is_left_bower) {
    Card t(JACK, HEARTS);
    Card f(JACK, DIAMONDS);

    ASSERT_EQUAL(true, t.is_left_bower(DIAMONDS));
    ASSERT_EQUAL(false, f.is_left_bower(DIAMONDS));
}

//Tests for basic trump card
TEST(test_is_trump_basic) {
    Card c(TEN, SPADES);
    ASSERT_EQUAL(true, c.is_trump(SPADES));
}
//Tests for non-trump card
TEST(test_is_trump_non_trump) {
    Card c(TEN, CLUBS);
    ASSERT_EQUAL(false, c.is_trump(SPADES));
}
//Tests for left-bower
TEST(test_is_trump_left_bower) {
    Card c(JACK, SPADES);
    ASSERT_EQUAL(true, c.is_trump(CLUBS));
}

//Tests for every next suit combination
TEST(test_suit_next){
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}

//Tests with both bowers
TEST(test_card_less_bowers){
    Card a(JACK, SPADES);
    Card b(JACK, CLUBS);
    ASSERT_EQUAL(true, Card_less(a, b, CLUBS));
}
//Tests bower with another trump
TEST(test_card_less_bower_v_other_trump){
    Card a(JACK, SPADES);
    Card b(NINE, CLUBS);
    ASSERT_EQUAL(false, Card_less(a, b, CLUBS));
}
//Tests with trump vs. non-trump
TEST(test_card_less_trump_non_trump){
    Card a(KING, HEARTS);
    Card b(KING, CLUBS);
    ASSERT_EQUAL(true, Card_less(a, b, CLUBS));
}
//Tests with two non-trumps
TEST(test_card_less_two_non_trumps){
    Card a(QUEEN, SPADES);
    Card b(NINE, CLUBS);
    ASSERT_EQUAL(false, Card_less(a, b, DIAMONDS));
}


//Tests led suit vs. non-led suit
TEST(test_card_less_led_suit_vs_non_led){
    Card lead(ACE, HEARTS);
    Card a(QUEEN, HEARTS);
    Card b(QUEEN, SPADES);
    ASSERT_EQUAL(false, Card_less(a, b, lead, CLUBS));
}

//Tests with trump suit and led suit
TEST(test_card_less_led_suit_vs_trump){
    Card lead(ACE, HEARTS);
    Card a(QUEEN, HEARTS);
    Card b(QUEEN, SPADES);
    ASSERT_EQUAL(true, Card_less(a, b, lead, SPADES));
}

//Tests with both cards in led suit
TEST(test_card_less_led_suit){
    Card lead(ACE, HEARTS);
    Card a(QUEEN, HEARTS);
    Card b(KING, HEARTS);
    ASSERT_EQUAL(true, Card_less(a, b, lead, CLUBS));
}

//Tests when neither card is led suit
TEST(test_card_less_non_led){
    Card lead(ACE, HEARTS);
    Card a(TEN, CLUBS);
    Card b(NINE, DIAMONDS);
    ASSERT_EQUAL(false, Card_less(a, b, lead, SPADES));
}

//Tests when left bower leads
TEST(test_card_less_left_bower_led){
    Card lead(JACK, CLUBS);
    Card a(QUEEN, CLUBS);
    Card b(QUEEN, SPADES);
    ASSERT_EQUAL(true, Card_less(a, b, lead, SPADES));
}

//Tests operator overloading for rank output
TEST(test_rank_output) {
  std::ostringstream oss;
  oss << NINE;
  ASSERT_EQUAL(oss.str(), "Nine");
}
//Tests operator overloading for suit output
TEST(test_suit_output) {
  std::ostringstream oss;
  oss << DIAMONDS;
  ASSERT_EQUAL(oss.str(), "Diamonds");
}
//Tests operator overloading for card output
TEST(test_card_output) {
  std::ostringstream oss;
  oss << Card(JACK, HEARTS);
  ASSERT_EQUAL(oss.str(), "Jack of Hearts");
}
//Tests operator overloading for rank input
TEST(test_rank_input) {
  std::istringstream iss("Queen");
  Rank r = TWO;
  iss >> r;
  ASSERT_EQUAL(r, QUEEN);
}
//Tests operator overloading for suit input
TEST(test_suit_input) {
  std::istringstream iss("Clubs");
  Suit s = SPADES;
  iss >> s;
  ASSERT_EQUAL(s, CLUBS);
}
//Tests operator overloading for card input
TEST(test_card_input) {
  std::istringstream iss("Ace of Spades");
  Card c;
  iss >> c;
  ASSERT_EQUAL(c, Card(ACE, SPADES));
}

TEST(test_less_rank_dominates) {
    ASSERT_TRUE(Card(NINE, DIAMONDS) < Card(TEN, SPADES));
    ASSERT_FALSE(Card(TEN, SPADES) < Card(NINE, DIAMONDS));
    ASSERT_TRUE(Card(KING, DIAMONDS) < Card(ACE, SPADES));
    ASSERT_FALSE(Card(ACE, SPADES) < Card(KING, DIAMONDS));
}


TEST(test_less_equal_cards) {
    ASSERT_FALSE(Card(JACK, CLUBS) < Card(JACK, CLUBS));
}

TEST(test_less_equal_rank_dominates) {
    ASSERT_TRUE(Card(NINE, DIAMONDS) <= Card(TEN, SPADES));
    ASSERT_FALSE(Card(TEN, SPADES) <= Card(NINE, DIAMONDS));
    ASSERT_TRUE(Card(KING, DIAMONDS) <= Card(ACE, SPADES));
    ASSERT_FALSE(Card(ACE, SPADES) <= Card(KING, DIAMONDS));
}


TEST(test_less_equal_equal_cards) {
    ASSERT_TRUE(Card(JACK, CLUBS) <= Card(JACK, CLUBS));
    ASSERT_TRUE(Card(ACE, SPADES) <= Card(ACE, SPADES));
}

TEST(test_greater_rank_dominates) {
    ASSERT_TRUE(Card(TEN, SPADES) > Card(NINE, DIAMONDS));
    ASSERT_FALSE(Card(NINE, DIAMONDS) > Card(TEN, SPADES));
    ASSERT_TRUE(Card(ACE, SPADES) > Card(KING, DIAMONDS));
    ASSERT_FALSE(Card(KING, DIAMONDS) > Card(ACE, SPADES));
}


TEST(test_greater_equal_cards) {
    ASSERT_FALSE(Card(JACK, CLUBS) > Card(JACK, CLUBS));
}

TEST(test_greater_equal_rank_dominates) {
    ASSERT_TRUE(Card(TEN, SPADES) >= Card(NINE, DIAMONDS));
    ASSERT_FALSE(Card(NINE, DIAMONDS) >= Card(TEN, SPADES));
    ASSERT_TRUE(Card(ACE, SPADES) >= Card(KING, DIAMONDS));
    ASSERT_FALSE(Card(KING, DIAMONDS) >= Card(ACE, SPADES));
}


TEST(test_greater_equal_equal_cards) {
    ASSERT_TRUE(Card(JACK, CLUBS) >= Card(JACK, CLUBS));
    ASSERT_TRUE(Card(ACE, SPADES) >= Card(ACE, SPADES));
}

TEST(test_equal_same_cards) {
    ASSERT_TRUE(Card(JACK, CLUBS) == Card(JACK, CLUBS));
    ASSERT_TRUE(Card(NINE, SPADES) == Card(NINE, SPADES));
    ASSERT_TRUE(Card(ACE, DIAMONDS) == Card(ACE, DIAMONDS));
}

TEST(test_equal_different_rank) {
    ASSERT_FALSE(Card(NINE, SPADES) == Card(TEN, SPADES));
    ASSERT_FALSE(Card(ACE, HEARTS) == Card(KING, HEARTS));
}

TEST(test_equal_different_suit) {
    ASSERT_FALSE(Card(NINE, SPADES) == Card(NINE, HEARTS));
    ASSERT_FALSE(Card(JACK, CLUBS) == Card(JACK, DIAMONDS));
}

TEST(test_equal_different_rank_and_suit) {
    ASSERT_FALSE(Card(NINE, SPADES) == Card(ACE, DIAMONDS));
}

TEST(test_not_equal_same_cards) {
    ASSERT_FALSE(Card(JACK, CLUBS) != Card(JACK, CLUBS));
    ASSERT_FALSE(Card(ACE, DIAMONDS) != Card(ACE, DIAMONDS));
}

TEST(test_not_equal_different_rank) {
    ASSERT_TRUE(Card(NINE, SPADES) != Card(TEN, SPADES));
    ASSERT_TRUE(Card(ACE, HEARTS) != Card(KING, HEARTS));
}

TEST(test_not_equal_different_suit) {
    ASSERT_TRUE(Card(NINE, SPADES) != Card(NINE, HEARTS));
    ASSERT_TRUE(Card(JACK, CLUBS) != Card(JACK, DIAMONDS));
}

TEST(test_not_equal_different_rank_and_suit) {
    ASSERT_TRUE(Card(NINE, SPADES) != Card(ACE, DIAMONDS));
}


TEST_MAIN()
