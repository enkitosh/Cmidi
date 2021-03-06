#ifndef CMIDI_H_
#define CMIDI_H_

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <curses.h>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <map>
#include <iterator>
#include "RtMidi.h"
#include "RtError.h"

using namespace std;


enum STATES {STATE_OFF, STATE_ON};


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
int BPM = 200; //Beats per minute
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
const int width = (C / 2);
const int start_y = (L - height) / 2;
const int start_x =  (C - width) / 2;


class Cmidi
{

	    
public:
	Cmidi();
	~Cmidi();
    
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
	void getInput();
    	void getLoop(int loop_seq);
    	bool chooseMidiPort();


			

private:
	string t;
	int dc;
	int c, cord_y, cord_x;
	int currentNote;
	int lp;
	int mode;
	MEVENT event; //For mouse events
	
	WINDOW *win;
   	vector<vector<bool> > N; //for note, to save the position of notes
    	vector<unsigned char> message;
	RtMidiOut *midiout;	

};

Cmidi::Cmidi()
: t(""), dc(0), c(0), cord_y(0),cord_x(0), currentNote(0),
  mode(1), lp(0),  win(NULL) ,
N(height,vector<bool>(width))
{
    
	midiout = new RtMidiOut();

	message.push_back(0);
	message.push_back(0);
	message.push_back(0);

	initscr();
	noecho();
	curs_set(0);
	keypad(win,TRUE);
	cbreak();
	nodelay(win, TRUE);
    
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

Cmidi::~Cmidi()
{
    wgetch(win);
    wrefresh(win);
    endwin();

/*If we have to kill the process we send 
a midi off signal so the reicever is not stuck
with a frozen signal*/
    
	message[0] = 128 + (dc-1);
	message[1] =  currentNote;
	message[2] = 40;
	midiout->sendMessage( &message );


//cleanup connection to port //class RtMidi
        delete midiout;
}

bool Cmidi::chooseMidiPort()
{
    
    std::string portName;
    unsigned int i = 0, nPorts = midiout->getPortCount();
    if ( nPorts == 0 ) {
        mvwaddstr(win,height-1,2,"No output ports available");
    	wrefresh(win);
        return false;
    }
     

    midiout->openPort( 0 );
    
    return true;
}


void Cmidi::setWindow()
{
	win = newwin(height,width,start_y, start_x);
	box(win,0,1);
	wrefresh(win);
    
	mvwprintw(win,0,start_x - (t.length()/2), t.c_str());
	mvwprintw(win,0,width - 9, "%i:BPM" , BPM);
	wrefresh(win);	
}



void Cmidi::drawGrid()
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

string Cmidi::checkForNote(int position)
{
	if(position <= scale)
		return NOTES[position];
}

void Cmidi::generate_shit()
{
	setWindow();
	drawGrid();     
}


void Cmidi::getLoop(int loop_seq)
{
	lp = loop_seq;
}


void Cmidi::play_sequence()
{
	static int cells = 1;
	bool noteIsPlaying = 0;


	while(cells <= lp)
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
			  currentNote = (12 - i) + 60;
 		               noteIsPlaying = 1;

               			 // Note On: 144, 60, 90
                		message[0] = 144 + (dc-1);
               			message[1] = (12 - i) + 60;
                		message[2] = 90;
               			midiout->sendMessage( &message );
                
                	usleep(convertToMilliseconds(100) );
                
                		// Note Off: 128, 60, 40
                		message[0] = 128 + (dc-1);
                		message[1] =  (12 - i) + 60;
               			message[2] = 40;
                		midiout->sendMessage( &message );
				
				/*Print note value on the top of our sequencer*/
				mvwaddstr(win,0,2,checkForNote(0).c_str());
				mvwaddstr(win,0,2,checkForNote(i).c_str());
			}

			noteIsPlaying = 0;

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
			
	wrefresh(win);

	//midiout -> sendMessage( &message);

	usleep(milliTime);

	cells++;


	/*If loop has finished we start from beginning*/
	if(cells == lp+1)
	{

	/*This handles the last bar only, this is to clean up
	  the | bar like before*/
		for(int i =1; i < height-1; i++)
		{
			wattron(win,COLOR_PAIR(dc));
			mvwaddch(win,i,cells-1,'.');
			wattroff(win,COLOR_PAIR(dc));

			if(N[i][cells-1] == TRUE)
			{
				mvwaddch(win,i,cells-1,ACS_DIAMOND);
				wrefresh(win);
			}
		}
		
		//reset loop position
		cells = 1;
	}
	
	//we have to print out this information again
	//so it will not be erased
	box(win,0,1);
	mvwprintw(win,0,start_x - (t.length()/2), t.c_str());
	mvwprintw(win,0,width - 9, "%i:BPM" , BPM);
	wrefresh(win);
   
   }

}



void Cmidi::getInput()
{
	cbreak();
	nodelay(win,TRUE);
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
			
		//SHOW EQUIVALENT NOTE
			if((cord_y > 0 && cord_y  <= scale) && (cord_x > 0 && cord_x < width-1))
			{

			if(mode == 1)
			{
				N[cord_y][cord_x] = 1; //update bool map
				mvwaddch(win,cord_y,cord_x,ACS_DIAMOND); //Add diamonds as notes
			
			/*To get real time feedback from the mouse*/
				message[0] = 144 + (dc-1); //channel (144 is ch 1, 145 ch2 etc...)
				message[1] = (12 - cord_y) + 60; //note to play
                		message[2] = 90;
               			midiout->sendMessage( &message );
                
                		usleep(convertToMilliseconds(100) );
                
                		// Note Off: 128, 60, 40
                		message[0] = 128 + (dc-1);
                		message[1] =  (12 - cord_y) + 60;
               			 message[2] = 40;
				midiout -> sendMessage( &message );
				
				mvwaddstr(win,0,2,checkForNote(0).c_str());
				mvwaddstr(win,0,2,checkForNote(cord_y).c_str());
		
			  }

			else if(mode ==2)
			{
				N[cord_y][cord_x] = 0;
				wattron(win,COLOR_PAIR(dc));	
				mvwaddch(win,cord_y,cord_x,'.');
				wattroff(win,COLOR_PAIR(dc));
			}
			}
			wrefresh(win);
			break;
	//KEYS
		case ' ':
			play_sequence();
			break;
		case 'w': mode = 1; break; //write-mode
		case 'e': mode = 2; break; //erase-mode
    }
	wrefresh(win);
	wgetch(win);
    
}


int convertToInt(char *s)
{
    
	return atoi(s);
    
}


#endif
