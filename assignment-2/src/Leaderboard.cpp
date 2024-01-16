#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    if(head_leaderboard_entry== nullptr){
        head_leaderboard_entry = new_entry;
    }
    else if (new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    }
    else{
        bool enter = false;
        LeaderboardEntry* current_entry = head_leaderboard_entry;
        while(current_entry->next_leaderboard_entry != nullptr){
            if(new_entry->score > current_entry->next_leaderboard_entry->score){
                new_entry->next_leaderboard_entry = current_entry->next_leaderboard_entry;
                current_entry->next_leaderboard_entry = new_entry;
                enter = true;
                break;
            }
            current_entry = current_entry->next_leaderboard_entry;
        }
        if(!enter){
            current_entry->next_leaderboard_entry = new_entry;
        }
    }
    int number_of_list = 0;
    LeaderboardEntry* current = head_leaderboard_entry;
    while(current != nullptr){
        number_of_list++;
        if (number_of_list == MAX_LEADERBOARD_SIZE){
            current-> next_leaderboard_entry = nullptr;
        }
        current = current->next_leaderboard_entry;

    }







}

void Leaderboard::write_to_file(const string& filename) {
    std::ofstream output(filename);
    if(output.is_open()){
        LeaderboardEntry *current = head_leaderboard_entry;
        while(current != nullptr){
            output << current->score << " ";
            output << current->last_played << " ";
            output << current->player_name << std::endl;
            current = current->next_leaderboard_entry;
        }
    }
}

void Leaderboard::read_from_file(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout <<"dosya yok"<<std::endl;
    }
    std::string line;
    auto* current_entry = new LeaderboardEntry();
    while (std::getline(file,line)){
        std::istringstream iss(line);
        std::vector<std::string> words;

        do {
            std::string word;
            iss >> word;
            words.push_back(word);
        } while (iss);

        unsigned long time = std::stoul(words[1]);
        auto* new_member = new LeaderboardEntry(std::stoul(words[0]), static_cast<time_t>(time), words[2]);

        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_member;
        } else {
            current_entry->next_leaderboard_entry = new_member;
        }
        current_entry = new_member;
    }
}


void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard" << std::endl;
    std::cout << "-----------" << std::endl;
    int number = 1;
    LeaderboardEntry* current_entry = head_leaderboard_entry;
    while(current_entry != nullptr){
        std::cout <<number++ << ". " <<current_entry->player_name << " ";
        std::cout << current_entry->score << " ";
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(current_entry->last_played);
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S/%d.%m.%Y");
        std::cout << oss.str() << std::endl;
        current_entry = current_entry->next_leaderboard_entry;
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current_entry = head_leaderboard_entry;
    while (current_entry != nullptr) {
        LeaderboardEntry* next_entry = current_entry->next_leaderboard_entry;

        delete current_entry;

        current_entry = next_entry;
    }
}