#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include<Windows.h>
#include"menu.h"
#include"gamefunction.h"

using namespace std;

struct Board
{
	short row=9;
	short col=9;
	short mines=10;
}board;

struct Gameplay
{
	short Posx = 0;
	short Posy = 0;
	short cnt_ocells = 0;
	short cnt_mines = 0;
	bool is1st_step = true;
	bool lose = false;
    bool gamewin = false;
    string level="beginner";
}gameplay;


struct time_clock
{
	short hours = 0;
	short minutes = 0;
	short seconds = 0;
    string s_time;
    int t=0;
    int time_elapsed = 0;
}c;


string user_name;

int table[100][100];
int s_table[100][100];
int flag_table[100][100];

ranking beginner_lev[10];
ranking intermediate_lev[10];
ranking expert_lev[10];


void menu()
{
    sf::RenderWindow menu(sf::VideoMode(1024, 600), "Minesweeper Menu", sf::Style::Close);
    sf::Texture background;
    background.loadFromFile("D:/CS161/23125087_FinalProject/texture/background.png");
    background.setSmooth(true);
    sf::Sprite s_background;
    s_background.setTexture(background);
    sf::Font font;
    font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
    sf::Text title("MINESWEEPER", font, 100);
    title.setPosition(sf::Vector2f(160.f, 100.f));
    title.setFillColor(sf::Color(160, 160, 160));
    title.setStyle(sf::Text::Bold);
    sf::Text newgame("New Game", font, 50);
    newgame.setPosition(sf::Vector2f(350.f, 230.f));
    newgame.setFillColor(sf::Color(51, 153, 255));
    newgame.setStyle(sf::Text::Bold);
    sf::Text continuegame("Continue Game", font, 50);
    continuegame.setPosition(sf::Vector2f(350.f, 310.f));
    continuegame.setFillColor(sf::Color(51, 153, 255));
    continuegame.setStyle(sf::Text::Bold);
    sf::Text scoreboard("Score Board", font, 50);
    scoreboard.setPosition(sf::Vector2f(350.f, 390.f));
    scoreboard.setFillColor(sf::Color(51, 153, 255));
    scoreboard.setStyle(sf::Text::Bold);
    sf::Text setting("Setting", font, 50);
    setting.setPosition(sf::Vector2f(350.f, 470.f));
    setting.setFillColor(sf::Color(51, 153, 255));
    setting.setStyle(sf::Text::Bold);
    sf::RectangleShape shape[4];
    for (int i = 0; i < 4; i++)
    {
        shape[i].setSize(sf::Vector2f(500.f, 60.f));
        shape[i].setFillColor(sf::Color(192, 192, 192));
        shape[i].setOutlineThickness(5.f);
        shape[i].setOutlineColor(sf::Color(0, 128, 255));
    }
    while (menu.isOpen())
    {
        sf::Event evnt;
        while (menu.pollEvent(evnt))
        {
            if (evnt.type == evnt.Closed)
           {
               menu.close();
                begin();
           }
            
                menu.clear();
                menu.draw(s_background);
                for (int i = 0; i < 4; i++)
                {
                    shape[i].setPosition(sf::Vector2f(260.f, 230.f + i * 80));
                    menu.draw(shape[i]);
                }
                menu.draw(title);
                menu.draw(newgame);
                menu.draw(continuegame);
                menu.draw(scoreboard);
                menu.draw(setting);
                float x_coor;
                float y_coor;
                if (evnt.mouseButton.button == sf::Mouse::Left)
                {
                    x_coor = evnt.mouseButton.x*1.f;
                    y_coor = evnt.mouseButton.y*1.f;
                    if (x_coor > 260.f && x_coor < 800.f)
                    {
                        if (y_coor > 230.f && y_coor < 310.f)
                        {
                            menu.close();
                            loadboard();
                            reset();
                            createtable();
                            gameplay.cnt_mines = board.mines;
                            create_mines();
                            play_game();
                        }
                        else if (y_coor > 310.f && y_coor < 390.f)
                        {
                            loadboard();
                            loadgame();
                            if (game_inited() == true)
                            {
                                menu.close();
                                play_game();
                            }
                            else
                            {
                                sf::Text init("Invalid! ", font, 50);
                                init.setPosition(sf::Vector2f(400.f, 300.f));
                                init.setFillColor(sf::Color::White);
                                init.setStyle(sf::Text::Bold);
                                menu.draw(init);
                                Sleep(300);
                                init.setString("");
                            }
                        }
                        else if (y_coor > 390.f && y_coor < 470.f)
                        {
                            menu.close();
                            Sleep(500);
                            rank_level();
                        }
                        else if (y_coor > 470.f && y_coor < 550.f)
                        {
                            menu.close();
                            Sleep(500);
                            setting_level();
                        }
                    }
                }
            menu.display();
        }
    }
}

void begin()
{
    sf::RenderWindow begin(sf::VideoMode(1024, 600), "MINESWEEPER", sf::Style::Close);
    sf::Texture background;
    background.loadFromFile("D:/CS161/23125087_FinalProject/texture/background.png");
    background.setSmooth(true);
    sf::Sprite s_background;
    s_background.setTexture(background);
    sf::Font font;
    font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
    sf::Text title("MINESWEEPER", font, 100);
    title.setPosition(sf::Vector2f(160.f, 100.f));
    title.setFillColor(sf::Color(160, 160, 160));
    title.setStyle(sf::Text::Bold);
    sf::Text username;
    username.setFont(font);
    username.setCharacterSize(32);
    username.setFillColor(sf::Color::White);
    username.setPosition(sf::Vector2f(200.f, 250.f));
    username.setString("Enter your username: ");
    user_name = "";
    while (begin.isOpen())
    {
        sf::Event e_begin;
        while (begin.pollEvent(e_begin))
        {
            if (e_begin.type == e_begin.Closed)
            {
                begin.close();
            }
            begin.clear();
            begin.draw(s_background);
            begin.draw(title);
            begin.draw(username);
            begin.display();
            if (e_begin.type == sf::Event::TextEntered)
            {
                sf::Uint32 unicode = e_begin.text.unicode;
                if (unicode >= 32 && unicode <= 126)
                {
                    user_name += static_cast<char>(unicode);
                    username.setString("Enter your username: " + user_name);
                }
                if (unicode == 8 && !user_name.empty())
                {
                    user_name.pop_back();
                    username.setString("Enter your username: " + user_name);
                }
                if (unicode == 13)
                {
                    username.setString("Your username is: " + user_name + "\n Press left/right mouse to continue!");
                }
            }
            if (user_name != "")
            {
                if (e_begin.type == sf::Event::MouseButtonPressed)
                {
                    begin.close();
                    Sleep(300);
                    menu();
                }
            }
        }
    }
}

void setting_level()
{
    sf::RenderWindow setting_level(sf::VideoMode(1024, 600), "Minesweeper Setting Level", sf::Style::Close);
    sf::Texture background;
    background.loadFromFile("D:/CS161/23125087_FinalProject/texture/background.png");
    background.setSmooth(true);
    sf::Sprite s_background;
    s_background.setTexture(background);
    sf::Font font;
    font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
    sf::Text title("MINESWEEPER", font, 100);
    title.setPosition(sf::Vector2f(160.f, 100.f));
    title.setFillColor(sf::Color(160, 160, 160));
    title.setStyle(sf::Text::Bold);
    sf::Text beginner("Beginner", font, 50);
    beginner.setPosition(sf::Vector2f(350.f, 230.f));
    beginner.setFillColor(sf::Color(51, 153, 255));
    beginner.setStyle(sf::Text::Bold);
    sf::Text intermediate("Intermidiate", font, 50);
    intermediate.setPosition(sf::Vector2f(350.f, 310.f));
    intermediate.setFillColor(sf::Color(51, 153, 255));
    intermediate.setStyle(sf::Text::Bold);
    sf::Text expert("Expert", font, 50);
    expert.setPosition(sf::Vector2f(350.f, 390.f));
    expert.setFillColor(sf::Color(51, 153, 255));
    expert.setStyle(sf::Text::Bold);
    sf::Text re_menu("Return Menu", font, 50);
    re_menu.setPosition(sf::Vector2f(350.f, 470.f));
    re_menu.setFillColor(sf::Color(51, 153, 255));
    re_menu.setStyle(sf::Text::Bold);
    sf::RectangleShape shape_lev[4];
    for (int i = 0; i < 4; i++)
    {
        shape_lev[i].setSize(sf::Vector2f(500.f, 60.f));
        shape_lev[i].setFillColor(sf::Color(192, 192, 192));
        shape_lev[i].setOutlineThickness(5.f);
        shape_lev[i].setOutlineColor(sf::Color(0, 128, 255));
    }
    sf::Text levelchanged("Level Changed ", font, 50);
    levelchanged.setPosition(sf::Vector2f(350.f, 350.f));
    levelchanged.setFillColor(sf::Color::White);
    levelchanged.setStyle(sf::Text::Bold);
    while (setting_level .isOpen())
    {
        sf::Event e_lev;
        while (setting_level.pollEvent(e_lev))
        {
            if (e_lev.type == e_lev.Closed)
            {
                setting_level.close();
            }

            setting_level.clear();
            setting_level.draw(s_background);
            for (int i = 0; i < 4; i++)
            {
                shape_lev[i].setPosition(sf::Vector2f(260.f, 230.f + i * 80));
                setting_level.draw(shape_lev[i]);
            }
            setting_level.draw(title);
            setting_level.draw(beginner);
            setting_level.draw(intermediate);
            setting_level.draw(expert);
            setting_level.draw(re_menu);
            float x_coor;
            float y_coor;
            if (e_lev.mouseButton.button == sf::Mouse::Left)
            {
                x_coor = e_lev.mouseButton.x * 1.f;
                y_coor = e_lev.mouseButton.y * 1.f;
                if (x_coor > 260.f && x_coor < 800.f)
                {
                    if (y_coor > 230.f && y_coor < 310.f)
                    {
                        board.row = 9;
                        board.col = 9;
                        board.mines = 10;
                        reset();
                        gameplay.level = "beginner";
                        savegame();
                        saveboard();
                        setting_level.draw(levelchanged);
                        Sleep(500);
                        levelchanged.setString("");
                    }
                    else if (y_coor > 310.f && y_coor < 390.f)
                    {
                        board.row = 16;
                        board.col = 16;
                        board.mines = 40;
                        reset();
                        gameplay.level ="intermediate";
                        savegame();
                        saveboard();
                        setting_level.draw(levelchanged);
                        Sleep(500);
                        levelchanged.setString("");
                    }
                    else if (y_coor > 390.f && y_coor < 470.f)
                    {
                        board.row = 16;
                        board.col = 30;
                        board.mines = 99;
                        reset();
                        gameplay.level = "expert";
                        savegame();
                        saveboard();
                        setting_level.draw(levelchanged);
                        Sleep(500);
                        levelchanged.setString("");
                    }
                   else if (y_coor > 470.f && y_coor < 550.f)
                   {
                        setting_level.close();
                        Sleep(500);
                        menu();
                   }
                }
            }
            setting_level.display();
        }
    }
}

void rank_level()
{
        sf::RenderWindow rank_level(sf::VideoMode(1024, 600), "Minesweeper Setting Level", sf::Style::Close);
        sf::Texture background;
        background.loadFromFile("D:/CS161/23125087_FinalProject/texture/background.png");
        background.setSmooth(true);
        sf::Sprite s_background;
        s_background.setTexture(background);
        sf::Font font;
        font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
        sf::Text title("MINESWEEPER", font, 100);
        title.setPosition(sf::Vector2f(160.f, 100.f));
        title.setFillColor(sf::Color(160, 160, 160));
        title.setStyle(sf::Text::Bold);
        sf::Text beginner("Beginner", font, 50);
        beginner.setPosition(sf::Vector2f(350.f, 230.f));
        beginner.setFillColor(sf::Color(51, 153, 255));
        beginner.setStyle(sf::Text::Bold);
        sf::Text intermediate("Intermidiate", font, 50);
        intermediate.setPosition(sf::Vector2f(350.f, 310.f));
        intermediate.setFillColor(sf::Color(51, 153, 255));
        intermediate.setStyle(sf::Text::Bold);
        sf::Text expert("Expert", font, 50);
        expert.setPosition(sf::Vector2f(350.f, 390.f));
        expert.setFillColor(sf::Color(51, 153, 255));
        expert.setStyle(sf::Text::Bold);
        sf::Text re_menu("Return Menu", font, 50);
        re_menu.setPosition(sf::Vector2f(350.f, 470.f));
        re_menu.setFillColor(sf::Color(51, 153, 255));
        re_menu.setStyle(sf::Text::Bold);
        sf::RectangleShape shape_lev[4];
        for (int i = 0; i < 4; i++)
        {
            shape_lev[i].setSize(sf::Vector2f(500.f, 60.f));
            shape_lev[i].setFillColor(sf::Color(192, 192, 192));
            shape_lev[i].setOutlineThickness(5.f);
            shape_lev[i].setOutlineColor(sf::Color(0, 128, 255));
        }
        sf::Text levelchanged("Level Changed ", font, 50);
        levelchanged.setPosition(sf::Vector2f(350.f, 350.f));
        levelchanged.setFillColor(sf::Color::White);
        levelchanged.setStyle(sf::Text::Bold);
        while (rank_level.isOpen())
        {
            sf::Event e_lev;
            while (rank_level.pollEvent(e_lev))
            {
                if (e_lev.type == e_lev.Closed)
                {
                    rank_level.close();
                }

                rank_level.clear();
                rank_level.draw(s_background);
                for (int i = 0; i < 4; i++)
                {
                    shape_lev[i].setPosition(sf::Vector2f(260.f, 230.f + i * 80));
                    rank_level.draw(shape_lev[i]);
                }
                rank_level.draw(title);
                rank_level.draw(beginner);
                rank_level.draw(intermediate);
                rank_level.draw(expert);
                rank_level.draw(re_menu);
                float x_coor;
                float y_coor;
                if (e_lev.mouseButton.button == sf::Mouse::Left)
                {
                    x_coor = e_lev.mouseButton.x * 1.f;
                    y_coor = e_lev.mouseButton.y * 1.f;
                    if (x_coor > 260.f && x_coor < 800.f)
                    {
                        if (y_coor > 230.f && y_coor < 310.f)
                        {
                            rank_level.close();
                            load_rank("beginner");
                            rank_board("BEGINNER", beginner_lev);
                        }
                        else if (y_coor > 310.f && y_coor < 390.f)
                        {
                            rank_level.close();
                            load_rank("intermediate");
                            rank_board("INTERMEDIATE", intermediate_lev);
                        }
                        else if (y_coor > 390.f && y_coor < 470.f)
                        {
                            rank_level.close();
                            load_rank("expert");
                            rank_board("EXPERT", expert_lev);

                        }
                        else if (y_coor > 470.f && y_coor < 550.f)
                        {
                            rank_level.close();
                            Sleep(500);
                            menu();
                        }
                    }
                }
                rank_level.display();
            }
        }
}
void sort_rank(string level)
{
    if (level == "beginner" && c.t < beginner_lev[9].t_score)
    {
        beginner_lev[9].t_score = c.t;
        beginner_lev[9].name = user_name;
        sort(beginner_lev, beginner_lev + 10, [](const ranking& a, const ranking& b) {return a.t_score < b.t_score; });
    }
    else if (level == "intermediate" && c.t < intermediate_lev[9].t_score)
    {
        intermediate_lev[9].t_score = c.t;
        intermediate_lev[9].name = user_name;
        sort(intermediate_lev, intermediate_lev + 10, [](const ranking& a, const ranking& b) {return a.t_score < b.t_score; });
    }
    else if (level == "expert" && c.t < expert_lev[9].t_score)
    {
        expert_lev[9].t_score = c.t;
        expert_lev[9].name = user_name;
;       sort(expert_lev, expert_lev + 10, [](const ranking& a, const ranking& b) {return a.t_score < b.t_score; });
    }
}

void save_rank(string level)
{
    ofstream file_rank(level + "ranking.txt");
    if (!file_rank.is_open())
    {
        cout << "invalid!";
    }
    else
    {
        if (level == "beginner")
            for (int i = 0; i < 10; i++)
            {
                file_rank << beginner_lev[i].name << " ";
                file_rank << beginner_lev[i].t_score << " ";
            }
        else if (level == "intermediate")
            for (int i = 0; i < 10; i++)
            {
                file_rank << intermediate_lev[i].name << " ";
                file_rank << intermediate_lev[i].t_score << " ";
            }
        else if (level == "expert")
            for (int i = 0; i < 10; i++)
            {
                file_rank << expert_lev[i].name << " ";
                file_rank << expert_lev[i].t_score << " ";
            }
    }
    file_rank.close();
}
void load_rank(string level)
{
    ifstream file_rank(level + "ranking.txt");
    if (!file_rank.is_open())
    {
        cout << "invalid!";
    }
    else
    {
        if (level == "beginner")
            for (int i = 0; i < 10; i++)
            {
                file_rank >> beginner_lev[i].name;
                file_rank >> beginner_lev[i].t_score;
            }
        else if (level == "intermediate")
            for (int i = 0; i < 10; i++)
            {
                file_rank >> intermediate_lev[i].name;
                file_rank >> intermediate_lev[i].t_score;
            }
        else if (level == "expert")
            for (int i = 0; i < 10; i++)
            {
                file_rank >> expert_lev[i].name;
                file_rank >> expert_lev[i].t_score;
            }
    }
    file_rank.close();
}


void rank_board(string level, ranking game_lev[10])
{
    sf::RenderWindow board(sf::VideoMode(800, 1000), "MINESWEEPER RANK", sf::Style::Close);
    sf::Font font;
    font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
    sf::Text text_lev(level + " RANK", font, 70);
    text_lev.setFillColor(sf::Color(160, 160, 160));
    text_lev.setPosition(sf::Vector2f(100.f, 1.f));
    text_lev.setStyle(sf::Text::Bold);
    sf::Text top[11];
    sf::Text name_txt[11];
    sf::Text score_txt[11];
    top[0].setString("TOP");
    top[0].setFont(font);
    top[0].setCharacterSize(40);
    top[0].setFillColor(sf::Color::White);
    top[0].setPosition(sf::Vector2f(0.f, 80.f));
    name_txt[0].setString("USERNAME");
    name_txt[0].setFont(font);
    name_txt[0].setCharacterSize(40);
    name_txt[0].setFillColor(sf::Color::White);
    name_txt[0].setPosition(sf::Vector2f(250.f, 80.f));
    score_txt[0].setString("SCORE(s)");
    score_txt[0].setFont(font);
    score_txt[0].setCharacterSize(40);
    score_txt[0].setFillColor(sf::Color::White);
    score_txt[0].setPosition(sf::Vector2f(600.f, 80.f));
    for (int i = 1; i < 11; i++)
    {
        string r = to_string(i);
        string score = to_string(game_lev[i-1].t_score);
        top[i].setString(r);
        top[i].setFont(font);
        top[i].setCharacterSize(40);
        top[i].setFillColor(sf::Color::White);
        top[i].setPosition(sf::Vector2f(0.f, (i+1)*80.f));
        name_txt[i].setString(game_lev[i-1].name);
        name_txt[i].setFont(font);
        name_txt[i].setCharacterSize(40);
        name_txt[i].setFillColor(sf::Color::White);
        name_txt[i].setPosition(sf::Vector2f(250.f, (i+1)*80.f));
        score_txt[i].setString(score);
        score_txt[i].setFont(font);
        score_txt[i].setCharacterSize(40);
        score_txt[i].setFillColor(sf::Color::White);
        score_txt[i].setPosition(sf::Vector2f(600.f, (i+1)*80.f));
    }
    while (board.isOpen())
    {
        sf::Event e;
        while (board.pollEvent(e))
        {
            if (e.type == e.Closed)
            {
                board.close();
                rank_level();
            }
            board.clear(sf::Color(176, 196, 222));
            board.draw(text_lev);
            for (int i = 0; i < 11; i++)
            {
                board.draw(top[i]);
                board.draw(name_txt[i]);
                board.draw(score_txt[i]);
            }
            board.display();
        }
    }
}

bool game_inited()
{
    bool game_init=false;
    for (int y = 0; y < board.row; y++)
        for (int x = 0; x < board.col; x++)
            if (s_table[y][x] != 0)
            {
                game_init = true;
                break;
            }
    return game_init;
}