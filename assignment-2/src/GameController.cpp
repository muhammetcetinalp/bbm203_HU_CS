#include "GameController.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>


bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    std::ifstream file(commands_file);
    if (!file.is_open()) {
        std::cout <<"dosya yok"<<std::endl;
    }

    std::string line;
    int col_location = 0;
    while (std::getline(file,line)){
        if (line.find("PRINT_GRID") != std::string::npos) {
            print(game,game.active_rotation,col_location);
        } else if (line.find("DROP") != std::string::npos) {

            //calculate drop distance
            int drop_row = drop_distance(game,col_location);
            //dropping
            drop(game,col_location,drop_row);
            //power_up check
            power_up_check(game);

            // cleaning the row
            clear_row(game);
            col_location = 0;
            game.active_rotation = game.active_rotation->next_block;
            //last block
            if (game.active_rotation == nullptr){
                bool finish = no_more_blocks(game,file);
                if (!finish){
                    return true;
                }
            }
            //block could not fit check
            bool finish = could_not_fit(game,file);
            if (!finish){
                return false;
            }
        } else if (line.find("MOVE_RIGHT") != std::string::npos) {
            int collision = 0;
            if (col_location <= game.cols - game.active_rotation->shape[0].size() - 1) {
                for(int i = 0 ; i<game.active_rotation->shape.size();++i){
                    for (int j = 0 ; j < game.active_rotation->shape[i].size();++j) {
                        if (game.active_rotation->shape[i][j == 1]){
                            if (game.grid[i][j + 1 + col_location] == 1) {
                                collision++;
                                break;
                            }
                        }
                    }
                }
                if (collision == 0){
                    col_location++;
                }
                //col_location ++;
            }
        } else if (line.find("MOVE_LEFT") != std::string::npos) {
            if (col_location > 0) {
                col_location--;
            }
        } else if (line.find("ROTATE_RIGHT") != std::string::npos) {
            bool collision = false;
            for(int i = 0 ; i < game.active_rotation->right_rotation->shape.size();++i){
                for(int j = 0 ; j < game.active_rotation->right_rotation->shape[i].size();++j){
                    if((j + col_location) > (game.cols - 1)){
                        collision = true;
                        break;
                    }
                    if (game.active_rotation->right_rotation->shape[i][j == 1]){
                        if (game.grid[i][j + col_location] == 1) {
                            collision= true;
                            break;
                        }
                    }
                }
            }
            if(!collision){
                game.active_rotation = game.active_rotation->right_rotation;
            }
            //game.active_rotation = game.active_rotation->right_rotation;
        } else if (line.find("ROTATE_LEFT") != std::string::npos) {
            bool collision = false;
            for (int i = 0 ; i < game.active_rotation->left_rotation->shape.size();++i){
                for(int j = 0 ; j < game.active_rotation->left_rotation->shape[i].size();++j){
                    if(j + col_location < 0){
                        collision = true;
                        break;
                    }
                }
            }
            if(!collision){
                game.active_rotation = game.active_rotation->left_rotation;
            }
            //game.active_rotation = game.active_rotation->left_rotation;
        } else if (line.find("GRAVITY_SWITCH") != std::string::npos) {
            if (game.gravity_mode_on) {
                game.gravity_mode_on = false;
            } else {
                game.gravity_mode_on = true;
                // fall
                fall_after_gravity_switch(game);

            }
        }
        else {
            std::cout << "Unknown command: "<< line;
        }
    }
    no_more_command(game,file);
    return true;
}
void GameController::printGrid(const vector<vector<int>>& game) {
    for (const auto & i : game) {
        for (int j : i) {
            if (j == 1) {
                std::cout << occupiedCellChar <<"";
            } else {
                std::cout << unoccupiedCellChar<< "";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout<<std::endl;

}

bool GameController::no_more_blocks(BlockFall& game,std::ifstream& file) {

    cout << "YOU WIN!" <<endl;
    cout << "No more blocks." <<endl;
    cout << "Final grid and score:" << endl;
    std::cout<<std::endl;
    std::cout <<"Score: "  << game.current_score << endl;
    int score;
    if (game.leaderboard.head_leaderboard_entry == nullptr){
        std::cout << "High Score: 0";
    }
    else
        std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    printGrid(game.grid);
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_time = std::chrono::system_clock::to_time_t(current_time);
    auto current_times_tamp = static_cast<long long>(current_time_time);
    auto* new_entry = new LeaderboardEntry(game.current_score,current_times_tamp,game.player_name);
    game.leaderboard.insert_new_entry(new_entry);
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    game.leaderboard.print_leaderboard();
    file.close();
    return false;
}
bool GameController::could_not_fit(BlockFall& game,std::ifstream& file) {
    //block could not fit check
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (game.active_rotation->shape[i][j] == 1) {
                if(game.grid[i][j] == 1){
                    cout << "GAME OVER!" << endl;
                    cout << "Next block that couldn't fit:" <<endl;
                    for (auto & row : game.active_rotation->shape) {
                        for (auto && col : row) {
                            if (col == 1) {
                                std::cout << occupiedCellChar <<"";
                            } else {
                                std::cout << unoccupiedCellChar<< "";
                            }
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                    std::cout << "Final grid and score:"<< std::endl;
                    std::cout << std::endl;
                    std::cout <<"Score: "  << game.current_score << endl;
                    int score;
                    if (game.leaderboard.head_leaderboard_entry == nullptr){
                        std::cout << "High Score: 0";
                    }
                    else
                        std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                    std::cout << std::endl;
                    printGrid(game.grid);
                    auto current_time = std::chrono::system_clock::now();
                    std::time_t current_time_time = std::chrono::system_clock::to_time_t(current_time);
                    auto current_times_tamp = static_cast<long long>(current_time_time);
                    auto* new_entry = new LeaderboardEntry(game.current_score,current_times_tamp,game.player_name);
                    game.leaderboard.insert_new_entry(new_entry);
                    game.leaderboard.write_to_file(game.leaderboard_file_name);
                    game.leaderboard.print_leaderboard();
                    file.close();
                    return false;
                }
            }
        }
    }
    return true;
}
void GameController::no_more_command(BlockFall& game,std::ifstream &file) {
    cout << "GAME FINISHED!" <<endl;
    cout << "No more commands." <<endl;
    cout << "Final grid and score:" << endl;
    std::cout<<std::endl;
    std::cout <<"Score: "  << game.current_score << endl;
    int score;
    if (game.leaderboard.head_leaderboard_entry == nullptr){
        std::cout << "High Score: 0";
    }
    else
        std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    printGrid(game.grid);
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_time = std::chrono::system_clock::to_time_t(current_time);
    auto current_times_tamp = static_cast<long long>(current_time_time);
    auto* new_entry = new LeaderboardEntry(game.current_score,current_times_tamp,game.player_name);
    game.leaderboard.insert_new_entry(new_entry);
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    game.leaderboard.print_leaderboard();
    file.close();
}
void GameController::print(BlockFall& game,Block *rotation,int col_location) {
    std::cout << "Score: " << game.current_score << endl;
    std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << endl;

    for (int i = 0; i < rotation->shape.size(); ++i) {
        for (int j = 0; j < rotation->shape[i].size(); ++j) {
            if (rotation->shape[i][j] == 1) {
                game.grid[i][j + col_location] = 1;
            }
        }
    }

    printGrid(game.grid);

    for (int i = 0; i < rotation->shape.size(); ++i) {
        for (int j = 0; j < rotation->shape[i].size(); ++j) {
            if (rotation->shape[i][j] == 1) {
                game.grid[i][j + col_location] = 0;
            }
        }
    }
}
void GameController::power_up_check(BlockFall &game) {
    bool power_up2 = false;
    for (int k = 0; k<=game.rows-game.power_up.size();++k){
        for (int l = 0;l <= game.cols-game.power_up[0].size();++l){
            bool power_up = false;
            bool power_up3 = false;
            for(int i = 0 ; i < game.power_up.size();++i){
                for(int j = 0; j < game.power_up[i].size(); ++j){
                    if(game.grid[i+k][j+l] == game.power_up[i][j]){
                        power_up = true;
                        continue;
                    }
                    else{
                        power_up3 = true;
                    }
                }
            }
            if(power_up && !power_up3){
                power_up2 = true;
            }
        }
    }
    if(power_up2){
        cout << "Before clearing:"<< endl;
        printGrid(game.grid);
        game.current_score +=1000;
        for (int k = 0; k<game.rows;++k){
            for (int l = 0;l < game.cols;++l){
                if (game.grid[k][l] == 1){
                    game.grid[k][l] = 0;
                    game.current_score++;
                }
            }
        }

    }
}
void GameController::clear_row(BlockFall &game) {
    // cleaning the row
    bool row_bool = false;
    int number_of_row_has1 = 0;
    for (int i = game.rows - 1; i >= 0; --i) {
        for (int j = game.cols - 1; j >= 0; --j) {
            if (game.grid[i][j] == 1) {
                number_of_row_has1++;
            }
        }
        if (number_of_row_has1 == game.cols) {
            if(!row_bool){
                cout << "Before clearing:" << endl;
                printGrid(game.grid);
                row_bool = true;
            }
            for (int k = 0; k < game.cols; ++k) {
                game.grid[i][k] = 0;
                game.current_score++;
            }

            for (int k = i; k > 0; --k) {
                for (int l = 0; l < game.cols; ++l) {
                    if (game.grid[k][l] == 1) {
                        game.grid[k + 1][l] = 1;
                        game.grid[k][l] = 0;
                    }
                }
            }
            i = i+1;
        }
        number_of_row_has1 = 0;
    }
}
void GameController::clear_row(BlockFall &game,bool gravity) {
    // cleaning the row
    int number_of_row_has1 = 0;
    for (int i = game.rows - 1; i >= 0; --i) {
        for (int j = game.cols - 1; j >= 0; --j) {
            if (game.grid[i][j] == 1) {
                number_of_row_has1++;
            }
        }
        if (number_of_row_has1 == game.cols) {
            for (int k = 0; k < game.cols; ++k) {
                game.grid[i][k] = 0;
                game.current_score++;
            }

            for (int k = i; k > 0; --k) {
                for (int l = 0; l < game.cols; ++l) {
                    if (game.grid[k][l] == 1) {
                        game.grid[k + 1][l] = 1;
                        game.grid[k][l] = 0;
                    }
                }
            }
            i = i+1;
        }
        number_of_row_has1 = 0;
    }
}
void GameController::drop(BlockFall &game,int col_location,int drop_row) {
    //dropping
    int half_score = 0;
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (game.active_rotation->shape[i][j] == 1) {
                half_score++;
                game.grid[drop_row + i][j + col_location] = 1;
            }
        }
    }
    if (game.gravity_mode_on) {
        for (int i = game.rows - 2; i >= 0; --i) {
            for (int j = game.cols - 1; j >= 0; --j) {
                if (game.grid[i][j] == 1) {
                    int fall = 0;
                    for (int k = i; k < game.rows; k++) {
                        if (game.grid[k][j] == 0) {
                            fall++;
                        }
                    }
                    if (fall > 0) {
                        game.grid[i + fall][j] = 1;
                        game.grid[i][j] = 0;
                    }
                }
            }
        }
    }
    game.current_score += half_score * drop_row;
}
int GameController::drop_distance(BlockFall &game,int col_location) {
    int drop_row = game.rows - game.active_rotation->shape.size();
    int temporary = -1;
    // calculating drop distances
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
            if (game.active_rotation->shape[i][j] == 1) {
                for (int k = i; k < game.rows; ++k) {
                    if (game.grid[k][j + col_location] == 1) {
                        temporary = k - i - 1;
                        break;
                    }
                }
                if (temporary < 0) {
                    temporary = drop_row;
                }
                if (temporary < drop_row) {
                    drop_row = temporary;
                }
            }
        }
    }
    return drop_row;
}
void GameController::fall_after_gravity_switch(BlockFall &game) {
    for (int i = game.rows - 2; i >= 0; --i) {
        for (int j = game.cols - 1; j >= 0; --j) {
            if (game.grid[i][j] == 1) {
                int fall = 0;
                for (int k = i; k < game.rows; k++) {
                    if (game.grid[k][j] == 0) {
                        fall++;
                    }
                }
                if (fall > 0) {
                    game.grid[i + fall][j] = 1;
                    game.grid[i][j] = 0;
                }
            }
        }
    }
    clear_row(game,true);
}



