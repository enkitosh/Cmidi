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

when you compile you have to link it against the curses library and coreMidi etc...so you write an executable like this:
     
    g++ -D__MACOSX_CORE__ -o test test.cpp Cmidi.h -lncurses RtMidi.cpp -framework CoreMIDI -framework CoreAudio -framework CoreFoundation


The program takes in couple of options when you load the executable
      
      //so like ./test channel1 Drums

      string str = argv[1]; //Channel1
      string ttl = argv[2]; //Drums

For now you have to either put a number only or a string with a number in the end like here ^^ for the first argument

The first argument is really the important one, it matters because this is the channel your midi messages will
be sent out on. There are other options which you can include , for example:
    
    ./test channel1 Drums BPM= 140 LOOP 4

This will set the tempo to 140 and it will make the sequencer loop only the first 4 bars.
If nothing is declared tempo is set to it's default speed (120 BPM) and the sequencer will only loop when it has reached
the end.

There are couple of other options you can set while the sequencer is running:

    'w' = write mode, this option is set by default
    'e' = erase mode, if you want to erase notes you press 'e' and select the notes you want to erase

Note that after you have erased notes you have to click 'w' to get to write mode again.

Ascii-cast of the latest running process:
    http://ascii.io/a/1460

Picture of the latest running process:
    https://dl.dropbox.com/u/2208202/cmid.png
