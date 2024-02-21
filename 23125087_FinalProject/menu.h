#pragma once
#include<string>

struct ranking
{
    std::string name="#";
    int t_score=0;
};
void begin();
void menu();
void setting_level();
void sort_rank(std::string level);
void save_rank(std::string level);
void load_rank(std::string level);
void rank_level();
void rank_board(std::string level, ranking game_lev[10]);
bool game_inited();

