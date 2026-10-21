#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_make_trump_round1_order_up_with_left_bower) {
  Player *p = Player_factory("Alice", "Simple");
  p->add_card(Card(JACK, SPADES));    
  p->add_card(Card(JACK, CLUBS));     
  p->add_card(Card(NINE, HEARTS));
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(NINE, DIAMONDS));

  Card upcard(NINE, SPADES);
  Suit order_up = HEARTS;  
  bool result = p->make_trump(upcard, false, 1, order_up);

  ASSERT_TRUE(result);
  ASSERT_EQUAL(order_up, SPADES);
  delete p;
}

TEST(test_make_trump_round1_pass_with_one_trump) {
  Player *p = Player_factory("Lucas", "Simple");
  p->add_card(Card(KING, SPADES));    
  p->add_card(Card(NINE, HEARTS));     
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(NINE, DIAMONDS));
  p->add_card(Card(TEN, DIAMONDS));

  Card upcard(NINE, SPADES);
  Suit order_up = HEARTS;  
  bool result = p->make_trump(upcard, true, 1, order_up);

  ASSERT_FALSE(result);
  ASSERT_EQUAL(order_up, HEARTS);
  delete p;
}

TEST(test_make_trump_round2_left_bower) {
  Player *p = Player_factory("Kevin", "Simple");
  p->add_card(Card(JACK, SPADES));    
  p->add_card(Card(NINE, DIAMONDS));     
  p->add_card(Card(NINE, HEARTS));
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(TEN, DIAMONDS));

  Card upcard(NINE, SPADES);
  Suit order_up = HEARTS;  
  ASSERT_TRUE(p->make_trump(upcard, false, 2, order_up));
  ASSERT_EQUAL(order_up, CLUBS);
  delete p;
}

TEST(test_make_trump_round2_pass_screw_dealer) {
  Player *p = Player_factory("Euchre", "Simple");
  p->add_card(Card(ACE, HEARTS));    
  p->add_card(Card(ACE, DIAMONDS));     
  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(TEN, CLUBS));
  p->add_card(Card(NINE, HEARTS));

  Card upcard(NINE, SPADES);
  Suit order_up = HEARTS;  
  ASSERT_FALSE(p->make_trump(upcard, false, 2, order_up));
  ASSERT_EQUAL(order_up, HEARTS);

  ASSERT_TRUE(p->make_trump(upcard, true, 2, order_up));
  ASSERT_EQUAL(order_up, CLUBS);
  delete p;
}

TEST(test_add_and_discard_removes_lowest) {
  Player *p = Player_factory("EECS", "Simple");
  p->add_card(Card(NINE, SPADES));   
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(QUEEN, HEARTS));
  p->add_card(Card(KING, HEARTS));
  p->add_card(Card(ACE, CLUBS));

  p->add_and_discard(Card(TEN, DIAMONDS));   

  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(ACE, CLUBS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(KING, HEARTS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(QUEEN, HEARTS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(TEN, SPADES));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(TEN, DIAMONDS));  
  delete p;
}

TEST(test_add_and_discard_upcard_is_lowest) {
  Player *p = Player_factory("Pointer", "Simple");
  p->add_card(Card(TEN, DIAMONDS));
  p->add_card(Card(QUEEN, DIAMONDS));
  p->add_card(Card(KING, DIAMONDS));
  p->add_card(Card(ACE, DIAMONDS));
  p->add_card(Card(JACK, DIAMONDS));

  p->add_and_discard(Card(NINE, DIAMONDS));  
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(JACK, DIAMONDS));  
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(ACE, DIAMONDS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(KING, DIAMONDS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(QUEEN, DIAMONDS));
  ASSERT_EQUAL(p->lead_card(DIAMONDS), Card(TEN, DIAMONDS));
  delete p;
}


TEST(test_lead_card_left_bower_is_trump) {
  Player *p = Player_factory("Wolverine", "Simple");
  p->add_card(Card(JACK, CLUBS));    
  p->add_card(Card(NINE, DIAMONDS));
  p->add_card(Card(NINE, SPADES));

  ASSERT_EQUAL(p->lead_card(SPADES), Card(NINE, DIAMONDS));  
  ASSERT_EQUAL(p->lead_card(SPADES), Card(JACK, CLUBS));
  ASSERT_EQUAL(p->lead_card(SPADES), Card(NINE, SPADES));
  delete p;
}

TEST(test_play_card_follow_suit_highest) {
  Player *p = Player_factory("Michigan", "Simple");
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(KING, HEARTS));
  p->add_card(Card(ACE, SPADES));    
  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(TEN, CLUBS));

  Card led(NINE, HEARTS);
  ASSERT_EQUAL(p->play_card(led, SPADES), Card(KING, HEARTS));
  delete p;
}

TEST(test_play_card_led_left_bower) {
  Player *p = Player_factory("Winner", "Simple");
  p->add_card(Card(NINE, SPADES));   
  p->add_card(Card(ACE, CLUBS));    
  p->add_card(Card(KING, HEARTS));

  Card led(JACK, CLUBS);            
  ASSERT_EQUAL(p->play_card(led, SPADES), Card(NINE, SPADES));
  delete p;
}

TEST(test_play_card_cannot_follow_plays_lowest) {
  Player *p = Player_factory("AutoGrader", "Simple");
  p->add_card(Card(JACK, CLUBS));    
  p->add_card(Card(NINE, HEARTS));
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(NINE, DIAMONDS));
  p->add_card(Card(TEN, DIAMONDS));

  Card led(ACE, CLUBS);
  ASSERT_EQUAL(p->play_card(led, SPADES), Card(NINE, HEARTS));
  delete p;
}

// Add more tests here

TEST_MAIN()
