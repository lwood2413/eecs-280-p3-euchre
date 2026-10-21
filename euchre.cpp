#include <iostream>
#include <fstream>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"
using namespace std;


void print_error() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3"
      <<"NAME4 TYPE4" << endl;
}

class Game {
 public:
  Game(const std::vector<Player*> &players_in, const Pack &pack_in,
       bool shuffle_in, int points_in)
       : players(players_in), pack(pack_in), shuffle(shuffle_in),
         points_to_win(points_in) {
    
            

     }
  void play();


  private:
    void deal(int dealer);
    int make_trump(int dealer, const Card &upcard, Suit &trump);
    std::vector<Player*> players;
    Pack pack;
    bool shuffle;
    int points_to_win;
    int score[2];
};

const int NUM_PLAYERS = 4;
const int NUM_TRICKS = 5;

int main(int argc, char **argv) {
    //Checks if there are 12 arguments
    if(argc != 12){
        print_error();
        return 1;
    }
    //Checks if shuffle and points to win args are valid
    string shuffle_arg = argv[2];
    int points_to_win = atoi(argv[3]);
    if((shuffle_arg != "shuffle" && shuffle_arg != "noshuffle" ||
    points_to_win < 1 || points_to_win > 100)){
        print_error();
        return 1;
    }
    //Checks if type args are valid player types
    for(int i = 0; i < NUM_PLAYERS; ++i) {
        string type = argv[5 + 2 * i];
        if(type != "Simple" || type != "Human"){
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
