/*
CS 349 Code Examples: X Windows and XLib

    eventloop.min.cpp   Demos events by displaying mouse motion events 
    					to console (minimal version)

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o eventloop.min eventloop.min.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./eventloop.min

Note: the -L option and -lstdc++ may not be needed on some machines.

*/
#include <cstdlib>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
using namespace std;
Display* display;
Window window;
int main( int argc, char *argv[] ){
 display = XOpenDisplay("");
 if (display == NULL) exit (-1);
 int screennum = DefaultScreen(display);
 long background = WhitePixel(display, screennum);
 long foreground = BlackPixel(display, screennum); 
 window = XCreateSimpleWindow(display, DefaultRootWindow(display), 
		      10, 10, 300, 200, 2, foreground, background);
 XSelectInput(display, window, 
 	PointerMotionMask | KeyPressMask); // select events
 XMapRaised(display, window); 
 XFlush(display);
 XEvent event; // save the event here
 while( true ) {  // event loop until 'exit'
	XNextEvent( display, &event ); // wait for next event
	switch( event.type ) {
		case MotionNotify: // mouse movement
			cout << event.xmotion.x << ","
				 << event.xmotion.y << endl;
			break;
		case KeyPress: // any keypress
			exit(0);
			break;
	}
 }
 XCloseDisplay(display);
}
