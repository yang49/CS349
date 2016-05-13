/*
CS 349 Code Examples: X Windows and XLib

    openwindow     Opens a single blank window

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o openwindow openwindow.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./openwindow

Note: the -L option and -lstdc++ may not be needed on some machines.

*/


#include <cstdlib>
#include <iostream>
#include <list>
#include <unistd.h>


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;


/*
 * Information to draw on the window.
 */
struct XInfo {
    Display* 	display;
    int		 screen;
    Window	 window;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}


/*
 * Create a window
 */
void initX(int argc, char* argv[], XInfo& xInfo) {


    /*
    * Display opening uses the DISPLAY	environment variable.
    * It can go wrong if DISPLAY isn't set, or you don't have permission.
    */
    xInfo.display = XOpenDisplay( "" );
    if ( !xInfo.display )	{
        error( "Can't open display." );
    }

    /*
    * Find out some things about the display you're using.
    */
    xInfo.screen = DefaultScreen( xInfo.display ); // macro to get default screen index

    unsigned long white, black;
    white = XWhitePixel( xInfo.display, xInfo.screen ); 
    black = XBlackPixel( xInfo.display, xInfo.screen );

    xInfo.window = XCreateSimpleWindow(
       xInfo.display,				// display where window appears
       DefaultRootWindow( xInfo.display ), // window's parent in window tree
       10, 10,			           // upper left corner location
       400, 300,	               // size of the window
       5,						     // width of window's border
       black,						// window border colour
       white );					    // window background colour

    // extra window properties like a window title
    XSetStandardProperties(
        xInfo.display,		// display containing the window
        xInfo.window,		// window whose properties are set
        "x1_openWindow",	// window's title
        "OW",				// icon's title
        None,				// pixmap for the icon
        argv, argc,			// applications command line args
        None );			// size hints for the window

    /*
     * Put the window on the screen.
     */
    XMapRaised( xInfo.display, xInfo.window );

    XFlush(xInfo.display);
    sleep(2);	// let server get set up before sending drawing commands
                // this is a hack until we get to events
}



/*
 *   Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char* argv[] ) {

    XInfo xInfo;

    initX(argc, argv, xInfo);

    // wait for user input to quit (a concole event for now)
    cout << "press ENTER to exit";
    cin.get();

    XCloseDisplay(xInfo.display);
}
