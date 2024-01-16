#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    static void printGrid(const vector<vector<int>>& game);
    void print(BlockFall& game,Block* active_rotation,int col_location);
    static void power_up_check(BlockFall& game);
    static void clear_row(BlockFall& game);
    static void clear_row(BlockFall& game,bool gravity);
    static void drop(BlockFall& game,int col_location,int drop_row);
    static void fall_after_gravity_switch(BlockFall &game);
    static int drop_distance(BlockFall &game,int col_location);
    bool no_more_blocks(BlockFall& game,std::ifstream& file);
    bool could_not_fit(BlockFall& game,std::ifstream& file);
    void no_more_command(BlockFall& game,std::ifstream& file);
};


#endif //PA2_GAMECONTROLLER_H
