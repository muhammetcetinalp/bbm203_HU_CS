#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cout <<"dosya yok"<<std::endl;
    }
    std::string line;
    std::vector<vector<bool>> current_shape;
    Block* first_block = nullptr;
    bool in_matrix = false;
    bool end = false;
    while (std::getline(file,line)){
        std::vector<bool> current_row;
        for (char c : line){
            if (line.find('[') != std::string::npos){
                in_matrix = true;
                end = false;
            }
            if (line.find(']') != std::string::npos){
                end = true;
            }
            if (in_matrix){
                if (c == '0') {
                    current_row.push_back(false);
                } else if (c == '1') {
                    current_row.push_back(true);
                }
                else
                    continue;
            }
        }
        if(!current_row.empty()){
            current_shape.push_back(current_row);
        }
        if (end){
            auto* node = new Block;
            node->shape = current_shape;
            current_shape.clear();
            if (active_rotation){
                active_rotation->next_block = node;
                active_rotation->right_rotation->next_block = node;
                active_rotation->left_rotation->next_block = node;
                active_rotation->right_rotation->right_rotation->next_block = node;
            }
            active_rotation = node;

            auto* left = new Block;
            left->shape = left_rotate(node->shape);
            active_rotation = left;

            auto* right = new Block;
            right->shape = right_rotate(node->shape);
            active_rotation = right;

            auto* right_right = new Block;
            right_right->shape = right_rotate(right->shape);
            active_rotation = right_right;

            node->right_rotation = right;
            node->left_rotation = left;
            right->left_rotation = node;
            right->right_rotation = right_right;
            left->right_rotation = node;
            left->left_rotation = right_right;
            right_right->left_rotation = right;
            right_right->right_rotation = left;

            if (!initial_block){
                initial_block = node;
                first_block = node;
            }
            in_matrix = false;
            end = false;
            active_rotation =node;
        }
    }
    power_up = active_rotation->shape;

    while(first_block->next_block->next_block != nullptr){
        first_block = first_block->next_block;

    }


    first_block->next_block = nullptr;
    first_block->right_rotation->right_rotation->next_block = nullptr;
    first_block->left_rotation->next_block = nullptr;
    first_block->right_rotation->next_block = nullptr;

    active_rotation = initial_block;

    file.close();

}
void BlockFall::initialize_grid(const string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cout <<"dosya yok"<<std::endl;
    }
    std::string line;
    rows = 0;
    cols = 0;
    bool column = true;
    while (std::getline(file,line)){
        std::vector<int> matrix_row;
        rows++;
        if (column){
            for (char c : line){
                if (c == '0' || c == '1'){
                    cols ++;
                }
            }
            column = false;
        }
        for (char j : line){
            if(j == '1')
                matrix_row.push_back(1);
            if(j == '0')
                matrix_row.push_back(0);
        }
        grid.push_back(matrix_row);
    }
    file.close();
}


BlockFall::~BlockFall() {
    Block* current_block = initial_block;
    while (current_block != nullptr) {
        Block* next_block = current_block->next_block;
        delete current_block->right_rotation->right_rotation;
        delete current_block->right_rotation;
        delete current_block->left_rotation;
        delete current_block;
        current_block = next_block;
    }

}
vector<vector<bool>> BlockFall::right_rotate(const std::vector<vector<bool>>& original_shape) {
    vector<vector<bool>> rotated_shape(original_shape[0].size(), vector<bool>(original_shape.size(), false));
    for (size_t i = 0; i < original_shape.size(); ++i) {
        for (size_t j = 0; j < original_shape[0].size(); ++j) {
            rotated_shape[j][original_shape.size() - 1 - i] = original_shape[i][j];
        }
    }
    return rotated_shape;

}
vector<vector<bool>>  BlockFall::left_rotate(const std::vector<vector<bool>>& original_shape) {
    vector<vector<bool>> rotated_shape(original_shape[0].size(), vector<bool>(original_shape.size(), false));
    for (size_t i = 0; i < original_shape.size(); ++i) {
        for (size_t j = 0; j < original_shape[0].size(); ++j) {
            rotated_shape[original_shape[0].size() - 1 - j][i] = original_shape[i][j];
        }
    }
    return rotated_shape;

}
