Cmidi
=====

midi sequencer for command line

**NOTE this code is at very early stage.

UPDATE: midi now working, sending response on mouse click etc...
There are still some improvements to be made.
Keyboard inputs gets stuck in the loop of playing the sequence leaving you
no other option then to kill the process with ctrl+c.

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
for example.

The midi class is unused at the moment. I think it's purpose will mainly be to get all midi information and include operations
such as saving a sequence to midi file.

    //***BASIC SETUP***//
    Generator gen; //Make a generator
	  gen.set_defaultColor(value); //place the value from the input
	  gen.setTitle(ttl); //set title (most important!)
	  gen.generate_shit(); //word
	


       //***//TESTING	
    while(1)
    {
       gen.getMouse();


Ascii-cast of the latest running process:
    http://ascii.io/a/1460

Picture of the latest running process:
    https://dl.dropbox.com/u/2208202/cmid.png
