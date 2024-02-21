#include<string>
#include<SFML/Graphics.hpp>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<fstream>
#include<iostream>
#include<algorithm>
#include"gameFunction.h"
#include"menu.h"


using namespace std;

extern struct Board
{
	short row=9;
	short col=9;
	short mines=10;
}board;

extern struct Gameplay
{
	short Posx=0;
	short Posy=0;
	short cnt_ocells=0;
	short cnt_mines=0;
	bool is1st_step=true;
	bool lose=false;
	bool gamewin = false;
	string level="beginner";
}gameplay;

extern struct time_clock
{
	short hours=0;
	short minutes=0;
	short seconds=0;
	string s_time;
	int t;
    int time_elapsed = 0;
}c;

extern string user_name;

extern int table[100][100];
extern int s_table[100][100];
extern int flag_table[100][100];

void play_game()
{
	int display_x = board.col * 70;
	int display_y = board.row * 70;
	sf::RenderWindow game(sf::VideoMode(display_x, display_y), "Minesweeper", sf::Style::Close);
	game.setFramerateLimit(60);
	sf::Texture t[12];
	for (int i = 0; i <= 11; i++)
	{
		string filename ="D:/CS161/23125087_FinalProject/texture/"+to_string(i) + ".png";
		t[i].loadFromFile(filename);
		t[i].setSmooth(true);
	}
	sf::Sprite s[12];
	for (int i = 0; i <= 11; i++)
	{
		s[i].setTexture(t[i]);
	}
	sf::Texture clock;
	clock.loadFromFile("D:/CS161/23125087_FinalProject/texture/clock.png");
	sf::Sprite sprite_clock;
	sprite_clock.setTexture(clock);
	sf::Font font;
	font.loadFromFile("D:/CS161/23125087_FinalProject/font/connection.otf");
	sf::Clock clock_run;
	while (game.isOpen())
	{
			sf::Event e;
			while (game.pollEvent(e))
			{
				if (e.type == e.Closed)
				{
					savegame();
					saveboard();
					game.close();
					menu();
				}
				if(game_win()==false && gameplay.lose == false)
				{ 
					if (e.type == sf::Event::MouseButtonPressed)
					{
						int x_coor;
						int y_coor;
						x_coor = e.mouseButton.x - (display_x - 48 * board.col) / 2;
					    y_coor = e.mouseButton.y - (display_y - 48 * board.row) / 2;
						gameplay.Posx = x_coor / 48;
						gameplay.Posy = y_coor / 48;
						if (e.mouseButton.button == sf::Mouse::Left)
						{
							open_cell();
						}
						if (e.mouseButton.button == sf::Mouse::Right)
						{
							flag();
						}
					}
				}
			}
			sf::Time elapsed = clock_run.getElapsedTime();
			c.time_elapsed = elapsed.asSeconds();
			if (c.time_elapsed == 1)
			{
				time_func();
				clock_run.restart();
			}
			game.clear(sf::Color(176, 196, 222)); 
				for (int y = 0; y < board.row; y++)
					for (int x = 0; x < board.col; x++)
					{
						if (gameplay.lose == true)
						{
							if (s_table[y][x] == 9)
								table[y][x] = s_table[y][x];
						}
						s[table[y][x]].setTextureRect(sf::IntRect(0, 0, 48, 48));
						s[table[y][x]].setPosition(sf::Vector2f(((display_x - board.col * 48)) / 2 + x * 48.f, ((display_y - board.row * 48)) / 2 + y * 48.f));
						game.draw(s[table[y][x]]);
					}
				sprite_clock.setTextureRect(sf::IntRect(0, 0, 60, 60));
				sprite_clock.setPosition(0.f, 0.f);
				game.draw(sprite_clock);
				sf::Text timetext(c.s_time, font, 30);
				timetext.setPosition(sf::Vector2f(70.f,10.f));
				timetext.setFillColor(sf::Color::Black);
				game.draw(timetext);
				if (game_win() || gameplay.lose == true)
				{
					sf::RectangleShape overlay(sf::Vector2f(display_x * 1.f, display_y * 1.f));
					overlay.setFillColor(sf::Color(0, 128, 128, 128));
					game.draw(overlay);
					sf::Text playagaintext("Do you want to play again?\n Press y to play again! \n Press n to return menu !", font, 25);
					playagaintext.setPosition(sf::Vector2f((display_x - 10 * 35.f) / 2, (display_y - 50) / 3*2.f));
					playagaintext.setFillColor(sf::Color(255,215,0));
					playagaintext.setStyle(sf::Text::Italic);
					game.draw(playagaintext);
					if (game_win())
					{
						sf::Text wintext("YOU WIN!", font, 50);
						wintext.setPosition(sf::Vector2f((display_x - 5 * 50) / 2.f, (display_y - 50) / 2.f));
						wintext.setFillColor(sf::Color(255,255,0));
						wintext.setStyle(sf::Text::Bold);
						game.draw(wintext);

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
						{
							c.t = c.hours * 3600 + c.minutes * 60 + c.seconds;
							load_rank(gameplay.level);
							sort_rank(gameplay.level);
							save_rank(gameplay.level);
							reset();
							clock_run.restart();
							gameplay.cnt_mines = board.mines;
							create_mines();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
						{
							c.t = c.hours * 3600 + c.minutes * 60 + c.seconds;
							load_rank(gameplay.level);
							sort_rank(gameplay.level);
							save_rank(gameplay.level);
							reset();
							clock_run.restart();
							create_mines();
							gameplay.cnt_mines = board.mines;
							game.close();
							menu();
						}
					}
					if (gameplay.lose == true)
					{
						sf::Text losetext("GAME OVER!", font, 50);
						losetext.setPosition(sf::Vector2f((display_x - 6 * 50) / 2.f, (display_y - 50) / 2.f));
						losetext.setFillColor(sf::Color(160,160,160));
						losetext.setStyle(sf::Text::Bold);
						game.draw(losetext);
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
						{
							reset();
							gameplay.cnt_mines = board.mines;
							create_mines();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
						{
							reset();
							create_mines();
							gameplay.cnt_mines = board.mines;
							savegame();
							saveboard();
							game.close();
							menu();
						}
					}
				}
			game.display();
	}
}


void createtable()
{
	for (int y = 0; y < board.row; y++)
		for (int x = 0; x < board.col; x++)
		{
			table[y][x] = 10;
			s_table[y][x] = 0;
			flag_table[y][x] = 0;
		}
}

void number_cell(int x, int y)
{
	if (y >= 0 && y < board.row && x >= 0 && x < board.col && s_table[y][x] != 9)
	{
		s_table[y][x]++;
	}
}

void create_mines()
{
	srand(time(NULL));
	while (gameplay.cnt_mines)
	{
		unsigned int y = rand() % board.row;
		unsigned int x = rand() % board.col;
		if (s_table[y][x] != 9)
		{
			s_table[y][x] = 9;
			number_cell(x, y + 1);
			number_cell(x, y - 1);
			number_cell(x + 1, y + 1);
			number_cell(x + 1, y - 1);
			number_cell(x + 1, y);
			number_cell(x - 1, y + 1);
			number_cell(x - 1, y - 1);
			number_cell(x - 1, y);
			gameplay.cnt_mines--;
		}
	}
}

void reveal_cell(int x, int y)
{
	if (y >= 0 && y < board.row && x>=0 && x < board.col&& table[y][x]==10&& s_table[y][x]!=9 && table[y][x]!=11)
	{
		table[y][x] = s_table[y][x];
		if (table[y][x] == 0)
		{
			reveal_cell(x, y + 1);
			reveal_cell(x, y - 1);
			reveal_cell(x+1, y + 1);
			reveal_cell(x+1, y);
			reveal_cell(x+1, y - 1);
			reveal_cell(x-1, y + 1);
			reveal_cell(x-1, y);
			reveal_cell(x-1, y - 1);
		}
		gameplay.cnt_ocells+=1;
	}
}

void reveal_cell_isopen(int x, int y)
{
	if (y >= 0 && y < board.row && x >= 0 && x < board.col && table[y][x] == 10 && s_table[y][x] != 9 && table[y][x] != 11)
	{
		table[y][x] = s_table[y][x];
		if (table[y][x] == 0)
		{
			reveal_cell_isopen(x, y + 1);
			reveal_cell_isopen(x, y - 1);
			reveal_cell_isopen(x + 1, y + 1);
			reveal_cell_isopen(x + 1, y);
			reveal_cell_isopen(x + 1, y - 1);
			reveal_cell_isopen(x - 1, y + 1);
			reveal_cell_isopen(x - 1, y);
			reveal_cell_isopen(x - 1, y - 1);
		}
		gameplay.cnt_ocells += 1;
	}
	if (y >= 0 && y < board.row && x >= 0 && x < board.col && table[y][x] == 10&&s_table[y][x] == 9 && table[y][x] != 11)
	{
		table[y][x] = s_table[y][x];
		gameplay.lose = true;
	}
}

void open_cell()
{
	if (gameplay.is1st_step)
	{
		short y1st = gameplay.Posy;
		short x1st = gameplay.Posx;
		if (s_table[y1st][x1st] == 9)
		{
			gameplay.cnt_mines++;
			create_mines();
			s_table[y1st][x1st] = 0;
			for (int y = -1; y < 2; y++)
				for (int x = -1; x < 2; x++)
				{
					if (x != 0 || y != 0)
					{
						if (s_table[y1st + y][x1st + x] > 0 && s_table[y1st + y][x1st + x] < 9)
						{
							s_table[y1st + y][x1st + x]--;
						}
					}
				}
			for (int y = -1; y < 2; y++)
				for (int x = -1; x < 2; x++)
				{
					if (x != 0 || y != 0)
					{
						if (s_table[y1st + y][x1st + x]==9)
						{
							s_table[y1st][x1st]++;
						}
					}
				}
			reveal_cell(x1st, y1st);
		}
		else
			reveal_cell(x1st, y1st);
		gameplay.is1st_step = false;
	}
	else if (gameplay.is1st_step==false)
	{
		short y = gameplay.Posy;
		short x = gameplay.Posx;
		if (s_table[y][x] == 9 && table[y][x] != 11&&table[y][x]==10)
		{
			table[y][x] = s_table[y][x];
			gameplay.lose = true;
		}
		else
		{
			if (table[y][x] == 10)
				reveal_cell(x, y);
			else if (flag_table[y][x] == s_table[y][x] && flag_table[y][x] != 0&&table[y][x]>0&&table[y][x]<9)
			{
				reveal_cell_isopen(x, y + 1);
				reveal_cell_isopen(x, y - 1);
				reveal_cell_isopen(x + 1, y + 1);
				reveal_cell_isopen(x + 1, y);
				reveal_cell_isopen(x + 1, y - 1);
				reveal_cell_isopen(x - 1, y + 1);
				reveal_cell_isopen(x - 1, y);
				reveal_cell_isopen(x - 1, y - 1);
			}
		}
	}
}

void number_flag_plus(int x, int y)
{
	if (y >= 0 && y < board.row && x >= 0 && x < board.col && s_table[y][x] != 9)
	{
		flag_table[y][x]++;
	}
}

void number_flag_minus(int x, int y)
{
	if (y >= 0 && y < board.row && x >= 0 && x < board.col && s_table[y][x] != 9)
	{
		flag_table[y][x]--;
	}
}

void flag()
{
	int x = gameplay.Posx;
	int y = gameplay.Posy;
	if (x >= 0 && x < board.col && y >= 0 && y < board.row && table[y][x] == 10)
	{
		table[y][x] = 11;
		number_flag_plus(x, y + 1);
		number_flag_plus(x, y - 1);
		number_flag_plus(x + 1, y + 1);
		number_flag_plus(x + 1, y - 1);
		number_flag_plus(x + 1, y);
		number_flag_plus(x - 1, y + 1);
		number_flag_plus(x - 1, y - 1);
		number_flag_plus(x - 1, y);
	}
	else if (x >= 0 && x < board.col && y >= 0 && y < board.row && table[y][x] == 11)
	{
	
		table[y][x] = 10;
		number_flag_minus(x, y + 1);
		number_flag_minus(x, y - 1);
		number_flag_minus(x + 1, y + 1);
		number_flag_minus(x + 1, y - 1);
		number_flag_minus(x + 1, y);
		number_flag_minus(x - 1, y + 1);
		number_flag_minus(x - 1, y - 1);
		number_flag_minus(x - 1, y);
	}
}

bool game_win()
{
	if (gameplay.cnt_ocells== board.row * board.col - board.mines)
	{
		gameplay.gamewin = true;
		return gameplay.gamewin;
	}
	else
	{
		return gameplay.gamewin;
	}
}

void reset()
{
	gameplay.Posx;
	gameplay.Posy;
	gameplay.cnt_ocells = 0;
	gameplay.cnt_mines = 0;
	gameplay.is1st_step = true;
	gameplay.lose = false;
	gameplay.gamewin = false;
	c.seconds = 0;
	c.hours = 0;
	c.minutes = 0;
	for (int y = 0; y < board.row; y++)
		for (int x = 0; x < board.col; x++)
		{
			table[y][x] = 10;
			s_table[y][x] = 0;
			flag_table[y][x] = 0;
		}
}


void time_func()
{
	    clock_func();
		if (game_win() == false && gameplay.lose == false)
		{
			c.seconds += 1;
			c.s_time = to_string(c.hours) + ":" + to_string(c.minutes) + ":" + to_string(c.seconds);
		}
}

void clock_func()
{
	if (c.seconds == 60)
	{
		c.seconds = 0;
		c.minutes += 1;
	}
	if (c.minutes == 60)
	{
		c.minutes = 0;
		c.hours += 1;
	}
}
void savegame()
{
	ofstream file(user_name + "gamesave.txt");
	if (!file.is_open())
	{
		cout << "invalid!";
	}
	else
	{
		file << gameplay.Posx << " ";
		file << gameplay.Posy << " ";
		file << gameplay.cnt_ocells << " ";
		file << gameplay.cnt_mines << " ";
		file << gameplay.is1st_step << " ";
		file << gameplay.lose << " ";
		file << gameplay.gamewin << " ";
		file << gameplay.level << " ";
		file << c.seconds << " ";
		file << c.hours << " ";
		file << c.minutes << " ";
		for (int y = 0; y < board.row; y++)
		{
			for (int x = 0; x < board.col; x++)
			{
				file << table[y][x] << " ";
				file << s_table[y][x] << " ";
				file << flag_table[y][x] << " ";
			}
			file << endl;
		}
	}
	file.close();
}

void loadgame()
{
	ifstream file(user_name + "gamesave.txt");
	if (!file.is_open())
	{
		cout << "invalid!";
	}
	else
	{

		file >> gameplay.Posx;
		file >> gameplay.Posy;
		file >> gameplay.cnt_ocells;
		file >> gameplay.cnt_mines;
		file >> gameplay.is1st_step;
		file >> gameplay.lose;
		file >> gameplay.gamewin;
		file >> gameplay.level;
		file >> c.seconds;
		file >> c.hours;
		file >> c.minutes;
		for (int y = 0; y < board.row; y++)
		{
			for (int x = 0; x < board.col; x++)
			{
				file >> table[y][x];
				file >> s_table[y][x];
				file >> flag_table[y][x];
			}
		}
	}
	file.close();
}

void saveboard()
{
	ofstream file(user_name + "boardsave.txt");
	if (!file.is_open())
	{
		cout << "invalid!";
	}
	else
	{
		file << board.col << " ";
		file << board.row << " ";
		file << board.mines << " ";
	}
	file.close();
}

void loadboard()
{
	ifstream file(user_name + "boardsave.txt");
	if (!file.is_open())
	{
		cout << "invalid!";
	}
	else
	{
		file >> board.col;
		file >> board.row;
		file >> board.mines;
	}
	file.close();
}