#ifndef CMIDI_H_
#define CMIDI_H_

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <curses.h>
#include <sstream>
#include <unistd.h>
#include <map>
#include <iterator>

using namespace std;


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

/*Timing and BPM*/
const int BPM = 200; //Beats per minute
const int BPS = BPM / 60; //Beats per second
const int TIME = 1000 / BPS; //calculate time - this is x milliseconds per beat
const int DELAY = TIME / BPM; //calculate Delay time

int convertToMilliseconds(int t)
{
	return t * 1000;
}

const int milliTime = convertToMilliseconds(TIME);

/*Change this to suit your needs*/
int L = 24; //TERMINAL LINES
int C = 80; //TERMINAL COLUMNS

const int height = L * 0.60;
const int width = C / 2;
const int start_y = (L - height) / 2;
const int start_x =  (C - width) / 2;

class Generator
{
private:
	WINDOW *win;
	int dc;
	int c, cord_y, cord_x;
	int signal;
	string t;
	MEVENT event; //For mouse events
	vector<vector<bool> > N; //for note, to save the position of notes
	    
public:
	Generator();
	~Generator();
	
	/*Graphic stuff*/
	void set_defaultColor(int clr) {dc = clr;}
	int get_defaultColor() {return dc;}
	
	/*Window stuff*/
	void setTitle(string title) {t = title;}
	void setWindow();
	void drawGrid();
	
	/*Processes*/
	string checkForNote(int position);
	void generate_shit();
	void play_sequence();	
	void refresh();

	/*Input*/
	void getMouse(/*mousevent &someting???*/);
};

Generator::Generator()
: t(""), dc(0), c(0), cord_y(0),cord_x(0),signal(0), win(NULL) ,
N(height,vector<bool>(width))
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

void Generator::setWindow()
{
    
	win = newwin(height,width,start_y, start_x);
	box(win,0,1);
	wrefresh(win);
    
	mvwprintw(win,0,start_x - (t.length()/2), t.c_str());
	mvwprintw(win,0,width - 9, "%i:BPM" , BPM);
	wrefresh(win);
	
}



void Generator::drawGrid()
{ 
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 1; j < width-1; j++)
		{
			wattron(win,COLOR_PAIR(dc));
			mvwaddch(win,i,j,'.');
			wattroff(win,COLOR_PAIR(dc));
			wrefresh(win); 
            
		}
	}   
}

string Generator::checkForNote(int position)
{
	if(position <= scale)
		return NOTES[position];
}

void Generator::generate_shit()
{
	setWindow();
	drawGrid();
    
    
}


void Generator::play_sequence()
{
	static int cells = 1;

	while(cells < width-1)
	{

		for(int i = 1; i < height-1; i++)	
		{
		/*Draw a sequence line for each bar*/	
			wattron(win,COLOR_PAIR(7));
			mvwaddch(win,i,cells,'|');
			wattroff(win,COLOR_PAIR(7));
			wrefresh(win);

		//if there is a note in current position, show its name	
			if(N[i][cells] == TRUE)
			{
				mvwaddstr(win,0,2,checkForNote(0).c_str());
				mvwaddstr(win,0,2,checkForNote(i).c_str());
			}
		/*We have to clean up the bar before so it doesn't
		  leave behind a white line*/
			   if(cells-1 != 0)
			   {
				wattron(win, COLOR_PAIR(dc));
				mvwaddch(win,i,cells-1,'.');
				wattroff(win, COLOR_PAIR(dc));
				wrefresh(win);
					
			//We have to redraw notes again as well
			//so we check for notes position and compare
			//them with current location - 1
			//if the position contains a note it will
			//replace that position with a diamond note where
			//it was located before
				if(N[i][cells-1] == TRUE)
				{
					mvwaddch(win,i,cells-1,ACS_DIAMOND);
					wrefresh(win);
				}
			
			   }
			
		}	
		
	usleep(milliTime);
	wrefresh(win);
	cells++;

	/*If loop has finished we start from beginning*/
	if(cells == width-1)
	{

	/*This handles the last bar only, this is to clean up
	  the | bar like before*/
		for(int i =1; i < height-1; i++)
		{
			wattron(win,COLOR_PAIR(dc));
			mvwaddch(win,i,cells-1,'.');
			wattroff(win,COLOR_PAIR(dc));

		}
		
		//reset loop position
		cells = 1;
	}
   }
}



void Generator::getMouse()
{
	keypad(win,1);
    
	c = wgetch(win);
    switch(c)
    {
	//MOUSE
	        case KEY_MOUSE:
			getmouse(&event);
			cord_y = event.y;
			cord_x = event.x;
			wmouse_trafo(win,&cord_y,&cord_x,FALSE);
			N[cord_y][cord_x] = 1; //update bool map
			mvwaddch(win,cord_y,cord_x,ACS_DIAMOND); //Add diamonds as notes
			//SHOW EQUIVALENT NOTE
			if(cord_y <= scale)
			{
				mvwaddstr(win,0,2,checkForNote(0).c_str());
				mvwaddstr(win,0,2,checkForNote(cord_y).c_str());
			}
			wrefresh(win);
			break;
	//KEYS
		case ' ':
			play_sequence();
			break;	
    }
	wrefresh(win);
	wgetch(win);
    
}


int convertToInt(char *s)
{
    
	return atoi(s);
    
}



class MIDI
{
	//class for simple I/O operations

};

/*HANDLES SIGNALS FROM BOTH GENERATOR AND MIDI CLASS*/
/*TURNS THEM INTO MESSAGES FOR MIDI CONNECTOR*/
class Signal
{
	public:
	void setup();
	void connect();
	
	/*RETRIVE SIGNAL FROM GENERATOR*/
	void listenGen(bool gen);
	void getStateGen(int g);	


	/*SIGNAL FROM MIDI*/
	void listenMid(bool mid);
	void getStateMid(int m);


	/*PROCESSING*/
	int output();	

};


void Signal::setup()
{}
void Signal::connect()
{}

void Signal::listenGen(bool gen)
{

}
void Signal::getStateGen(int g)
{}
void Signal::listenMid(bool mid)
{}
void Signal::getStateMid(int m)
{}



#endif
