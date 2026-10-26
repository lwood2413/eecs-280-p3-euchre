#include <iostream>
#include <fstream>
#include <cassert>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"
using namespace std;


const int NUM_PLAYERS = 4;
const int NUM_TRICKS = 5;

void print_error() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
      <<"NAME4 TYPE4" << endl;
}

class Game {
 public:
  Game(const std::vector<Player*> &players_in, const Pack &pack_in,
       bool shuffle_in, int points_in)
       : players(players_in), pack(pack_in), shuffle(shuffle_in),
         points_to_win(points_in) {
            score[0] = 0;
            score[1] = 0;
         }



  void play(){
    int hand = 0; 
    int dealer = 0;

    while(score[0] < points_to_win && score[1] < points_to_win){

        play_hand(hand, dealer);
        hand++;
        dealer = (dealer + 1) % 4;
    }

    if(score[0] >= points_to_win){
        print_team(0);
        cout << " win!" << endl;
    }
    else{
        print_team(1);
        cout << " win!" << endl;
    }
  }


  private:
    void deal(int dealer){
        //Makes vector for batches of cards to deal
        const vector <int> batches = {3, 2, 3, 2, 2, 3, 2, 3};

        //For each batch, goes around to each player dealing respective amount
        for(int i = 0; i < batches.size(); ++i){
            int player = (dealer + 1 + i) % 4;

            //for each card dealt, adds it to player's hand
            for(int c = 0; c < batches[i]; ++c){
                players[player]-> add_card(pack.deal_one());
            }
        }

    }







    int ask_round(int dealer, const Card &upcard, int round, Suit &trump){
    for(int i = 1; i <= NUM_PLAYERS; ++i){
        int p = (dealer + i) % 4;
        if(players[p]->make_trump(upcard, p == dealer, round, trump)){
            cout << players[p]->get_name() << " orders up " << trump << endl;
            return p;
        }
        cout << players[p]->get_name() << " passes" << endl;
    }
    return -1;
}

int make_trump(int dealer, const Card &upcard, Suit &trump){
    for(int round = 1; round <= 2; ++round){
        int maker = ask_round(dealer, upcard, round, trump);
        if(maker != -1){
            if(round == 1){
                players[dealer]->add_and_discard(upcard);
            }
            cout << endl;
            return maker % 2;
        }
    }
    assert(false);
    return -1;
}









    int play_trick(int leader, Suit trump){

        //Sets led_card to the leaders decision when leading
        Card led_card = players[leader]->lead_card(trump);
        cout << led_card << " led by " << players[leader]->get_name() << endl;

        //Sets best and winner accordingly
        Card best = led_card;
        int winner = leader;

        //Continues for each player
        for(int i = 1; i < NUM_PLAYERS; ++i){
            //get current player
            int p = (leader + i) % 4;
            //Set card played to player p's decision
            Card played = players[p]->play_card(led_card, trump);
            cout << played << " played by " << players[p]->get_name() << endl;

            //If the card played is better than best, becomes new best
            if(Card_less(best, played, led_card, trump)){
                best = played;
                winner = p;
            }
        }
        //Prints who took the trick
        cout << players[winner]->get_name() << " takes the trick" << endl << endl;
        return winner;
    }









    void play_hand(int hand_num, int dealer){

        //Prints current hand
        cout << "Hand " << hand_num << endl;
        //prints dealer
        cout << players[dealer]-> get_name() << " deals" << endl;

        //Suffles if input says so, otherwise resets pack
        if(shuffle){
            pack.shuffle();
        }
        else{
            pack.reset();
        }
        //Dealer deals
        deal(dealer);
        Card upcard = pack.deal_one();
        //Prints the upcard that was turned over
        cout << upcard << " turned up" << endl;

        Suit trump;
        //sets maker team to whoever called trump
        int maker_team = make_trump(dealer, upcard, trump);


        int tricks_won[2] = {0, 0};
        int leader = (dealer + 1) % 4;

        //For each round, plays the trick, updates tricks won and winner
        for(int i = 0; i < 5; ++i){
            int winner = play_trick(leader, trump);
            tricks_won[winner % 2]++;
            leader = winner;
        }
        //Scores the hand
        score_hand(maker_team, tricks_won);
    }








    void score_hand(int maker_team, const int tricks_won[2]){
        int winner;

        //Checks for winner
        if(tricks_won[0] > tricks_won[1]){
            winner = 0;
        }
        else{
            winner = 1;
        }

        //Prints the team that won the hand
        print_team(winner);
        cout << " win the hand" << endl;

        //If maker team wins, checks for march
        if(winner == maker_team){
            if(tricks_won[winner] == NUM_TRICKS){
                cout << "march!" << endl;
                //If march, adds 2
                score[winner] += 2;
            }
            else{
                //Otherwise, adds 1
                score[winner] += 1;
            }
        }
        else{
            //If the non-making team wins, prinnt euchred and score 2
            cout << "euchred!" << endl;
            score[winner] += 2;
        }
        //Prints the scores for each team
        for(int i = 0; i < 2; ++i){
            print_team(i);
            cout << " have " << score[i] << " points" << endl;
        }
        cout << endl;
    }








    //Prints team players
    void print_team(int team) const{
        cout << players[team]-> get_name() << " and " <<
        players[team + 2]-> get_name();
    }







    std::vector<Player*> players;
    Pack pack;
    bool shuffle;
    int points_to_win;
    int score[2];
};



int main(int argc, char **argv) {
    //Checks if there are 12 arguments
    if(argc != 12){
        print_error();
        return 1;
    }
    //Checks if shuffle and points to win args are valid
    string shuffle_arg = argv[2];
    int points_to_win = atoi(argv[3]);
    if((shuffle_arg != "shuffle" && shuffle_arg != "noshuffle") ||
    points_to_win < 1 || points_to_win > 100){
        print_error();
        return 1;
    }
    //Checks if type args are valid player types
    for(int i = 0; i < NUM_PLAYERS; ++i) {
        string type = argv[5 + 2 * i];
        if(type != "Simple" && type != "Human"){
            print_error();
            return 1;
        }
    }
    //Checks if fin is open
    ifstream fin(argv[1]);
    if(!fin.is_open()){
        cout << "Error opening " << argv[1] << endl;
        return 1;
    }
    //Creates pack from input
    Pack pack(fin);

    //Prints out all arguments
    for (int i = 0; i < argc; ++i) {
    cout << argv[i] << " ";
    }
    cout << endl;

    //Uses player factory to create player objects for each player
    vector <Player*> players;
    for(int i = 0; i < NUM_PLAYERS; ++i){
        players.push_back(Player_factory(argv[4 + 2 * i], argv[5 + 2 * i]));
    }

    //Creates Game object
  Game game(players, pack, shuffle_arg == "shuffle", points_to_win);
  //Plays the game
  game.play();
    //After game, deletes players
  for(size_t i = 0; i < players.size(); ++i){
    delete players[i];
  }

}
