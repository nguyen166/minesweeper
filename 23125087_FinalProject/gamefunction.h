#pragma once
void createtable();
void create_mines(); 
void number_cell(int x, int y);
void play_game(); 
void reveal_cell(int x, int y);
void reveal_cell_isopen(int x, int y);
void number_flag_plus(int x, int y);
void number_flag_minus(int x, int y);
void open_cell();
void flag();
bool game_win();
void reset();
void time_func();
void clock_func();
void savegame();
void loadgame();
void saveboard();
void loadboard();
