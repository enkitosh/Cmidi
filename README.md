Cmidi
=====

midi sequencer for command line

**NOTE this code is at very early stage, there are no "midi" implementations in this yet, this is mostly graphics and event input from mouse.

These are very basic notes about the test.cpp file in the /include folder.

    #include "CMidi.h"

when you compile you have to link it against the curses library so you write an executable like this:
     
    g++ -o main main.cpp -lncurses

otherwise I think there is nothing more (hopefully). I think the ncurses header is included in your computer by default


}

The first two parameters take input when you load the excecutable , the first is channel number
      
      //so like ./test channel1 Drums

      string str = argv[1]; //Channel1
      string ttl = argv[2]; //Drums


For now you have to either put a number only or a string with a number in the end like here ^^ for the first argument
      

There are two main classes, Generator and MIDI. 
Generator, generates stuff. It draws windows, pattern graphic stuff, but it also holds position of notes and channel info
for example. But although in the Generator the channel info is only used for color.
The Generator has a state. This important mainly because when notes are added to the graphic sequencer they are also being
fed into a 2d vector which stores notes and position. We have to take note of the state once in a while so we can't be
putting in values while we run the sequence. So we kind of lock any input while we play sequence by pressing 'space' and
some kind of counter will take care that state is either on or off 
**Note that I have not implemented all of this yet but the functions are kind of set up

    //***BASIC SETUP***//
    Generator gen; //Make a generator
	  gen.set_defaultColor(value); //place the value from the input
	  gen.setTitle(ttl); //set title (most important!)
	  gen.generate_shit(); //word
	



The MIDI class only takes care of midi messages going from the generator. It stores channel number, last note, but also he stores
some ability to handle outside messages, like take in new valus etc...
    
    MIDI SETUP
    MIDI mid;
    mid.setup(); //set up some channels (probably reserve the first 127 then or so..)

    mid.setChannelNr(value);

       //***//TESTING	
    while(1)
    {
       gen.getMouse();


Finally is this signal handler. It has not been implemented so much but the role of him is to take any process that is going on
or listen to it until it makes a move is a hope for an extended class which could turn his values into some kind of portable
midi device. The thing is I found no easy going libraries for midi in c++ other then with a GUI, so I thought maybe we could
implement the structure and then when we figure out how to connect to an outside source.

    LISTEN TO STATES
    sig.listenGen(gen.sendSignal());
    sig.listenMid(mid.sendSignal());
    }



here is a picture of the running process:
    https://dl.dropbox.com/u/2208202/cmid.png
