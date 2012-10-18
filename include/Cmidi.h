#ifndef CMIDI_H_
#define CMIDI_H_

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <curses.h>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;

const int scale = 12;

const string NOTES[scale + 2] = 
			{"  ",
			 "B",
	 		 "A#",
	 		 "A",
	 		 "G#",
	 		 "G",
	 		 "F#",
	 		 "F",
	 		 "E",
	 		 "D#",
	 		 "D",
	 		 "C#",
	 		 "C"
			};

/*Change this to suit your needs*/
int L = 24; //TERMINAL LINES
int C = 80; //TERMINAL COLUMNS

const int height = L * 0.60;
const int width = C / 2;
const int start_y = (L - height) / 2;
const int start_x =  (C - width) / 2;

class Generator
{
	WINDOW *win;	
	int dc;
	int c, cord_y, cord_x;
	string t;	
	MEVENT event; //For mouse events
	vector<vector<char> > Grid;
	
	public:
	Generator();
	~Generator();
	void set_graphics();
	void set_defaultColor(int clr) {dc = clr;}
	int get_defaultColor() {return dc;}
	void setTitle(string title) {t = title;}
	void gammagamme();
	void drawGrid();
	string checkForNote(int position);
	void generate_shit();
	void getMouse(/*mousevent &someting???*/);	

};

Generator::Generator()
	: t(""), dc(0), c(0), cord_y(0),cord_x(0), win(NULL) , Grid(height,vector<char>(width))
{

	initscr();
	noecho();
	curs_set(0);
	keypad(win,TRUE);

	mousemask(ALL_MOUSE_EVENTS,NULL);
	
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_BLACK);
	init_pair(7,COLOR_WHITE,COLOR_BLACK);
	init_pair(8,COLOR_BLACK,COLOR_BLACK);	

}

Generator::~Generator()
{
	wgetch(win);
	echo();
	endwin();
}

void Generator::gammagamme()
{

	win = newwin(height,width,start_y, start_x);
	box(win,0,1);
	wrefresh(win);

	mvwprintw(win,0,start_x - (t.length()/2), t.c_str());
	wrefresh(win);
	
}

void Generator::set_graphics()
{


	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			Grid[i][j] = '.';
		}

	}


}

void Generator::drawGrid()
{


	static int x_counter = 0;
	static int y_counter = 0;
	
	int sy = 1;
	int sx = 1;

	for(int i = 0; i < height-2; i++)
	{
		for(int j = 0; j < width-2; j++)
		{
			wattron(win,COLOR_PAIR(dc));
			mvwaddch(win,sy,sx,Grid[i][j]);
			wattroff(win,COLOR_PAIR(dc));
			wrefresh(win);			

			x_counter++;
			if(x_counter >= width - 2)
			{sx = 0;
			 x_counter = 0;}

			sx++;
		}

		y_counter++;
		if(y_counter >= height - 2)
		{
			sy = 0;
		}

		sy++;
	}

}

void Generator::generate_shit()
{
	gammagamme();
	drawGrid();


}

string Generator::checkForNote(int position)
{
	if(position <= scale)
		return NOTES[position];
}

void Generator::getMouse()
{
	keypad(win,1);

	c = wgetch(win);
		switch(c)
		{
			case KEY_MOUSE: 
			getmouse(&event);
			cord_y = event.y;
			cord_x = event.x;
			wmouse_trafo(win,&cord_y,&cord_x,FALSE);
			//ADD DIAMONDS AS NOTES
			mvwaddch(win,cord_y,cord_x,ACS_DIAMOND);
			//SHOW EQUIVALENT NOTE
			if(cord_y <= scale)
			{
				mvwaddstr(win,0,2,checkForNote(0).c_str());
				mvwaddstr(win,0,2,checkForNote(cord_y).c_str());
			}
			wrefresh(win);
			break;
		}
	wrefresh(win);
	wgetch(win);

}

int checkColor(char *s)
{

	return atoi(s);

}

#endif
