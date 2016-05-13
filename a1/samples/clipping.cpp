/*
CS 349 Code Examples: X Windows and XLib

    clipping.cpp   Demos clipping using a mouse controlled clipping
                   rectangle to reveal a pattern of random lines.

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o clipping clipping.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./clipping

Note: the -L option and -lstdc++ may not be needed on some machines.

*/
#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

// pick a random int between a and b
int my_rand(int a, int b)
{
  return a + (b - a) * (double)rand() / RAND_MAX;

}

// background type
class Pattern {

    vector< XPoint > points;

public:
    void init(int n, int max_x, int max_y) {
        for (int i=0; i < n; i++) {
            XPoint p;
            p.x = my_rand(0, max_x);
            p.y = my_rand(0, max_y);
            cout << p.x << "," << p.y << endl;
            points.push_back(p);


        }
    }

    void paint(Display* display, Window window, GC gc) {
        // note the trick to pass a stl vector as an C array
        XDrawLines(display, window, gc,
                   &points[0], points.size(),  // vector of points
                   CoordModeOrigin ); // use absolute coordinates

    }
};



// the clip window
XRectangle      clip_rect;

// background lines
Pattern pattern;




// my repaint function
void repaint(Display* display, Window window, GC gc) {
    XClearWindow( display, window );
    pattern.paint(display, window, gc);
    XFlush(display);
}



int main ( int argc, char* argv[] ) {
    Display*        display;            /* X display                                */
    int             screen;             /* Will reference the default screen        */
    Window          window;             /* Our simple window                        */
    GC              gc;                 /* Graphics context for drawing             */
    XSizeHints      hints;              /* For specifying size on creation          */
    int             BORDER_WIDTH = 5;   /* Window border width                      */
    unsigned long   background;         /* Background color                         */
    unsigned long   foreground;         /* Foreground color                         */


    /*
     * Display opening uses the DISPLAY  environment variable.
     * It can go wrong if DISPLAY isn't set, or you don't have permission.
     */
    display = XOpenDisplay( "" );
    if ( !display ) {
        printf( "Can't open display.\n" );
        exit(0);
    }

    /*
     * Get default screen
     */
    screen = DefaultScreen( display );

    /*
     * Get the background/foreground colors (black/white)
     */
    background = BlackPixel( display, screen );
    foreground = WhitePixel( display, screen );

    /*
     * Set up the window's size and location
     */
    hints.x = 100;
    hints.y = 100;
    hints.width = 400;
    hints.height = 300;
    hints.flags = PPosition | PSize;

    /*
     * Create a simple window
     */
    window = XCreateSimpleWindow(   display,
                                    DefaultRootWindow( display ),
                                    hints.x,
                                    hints.y,
                                    hints.width,
                                    hints.height,
                                    BORDER_WIDTH,
                                    foreground,
                                    background);

    /*
     * Set the window's basic properties
     */
    XSetStandardProperties( display,
                            window,
                            "Clipping",
                            "Clip",
                            None,           /* Pixmap for icon (none used here) */
                            argv,           /* Commands to run in the window */
                            argc,
                            &hints );       /* Sizes to use for window */

    /*
     * Put the window on the screen.
     */
    XMapRaised( display, window );

    /*
     * Send the commands to the server.
     * Without this, nothing happens!
     */
    XFlush(display);

    /*
     * Draw something on the window
     */

    /*
    * Create a graphics context to hold our drawing state information
    */
    gc = XCreateGC (display,
                    window,
                    0,          /* GCflags for determining which parts of the GC are used */
                    0);         /* A struct for filling in GC data at initialization */

    /*
     * Set the foreground/background colors to draw with
     */
    XSetBackground( display, gc, background );
    XSetForeground( display, gc, foreground );

    /*
     * Indicate what events we are interested in receiving
     */
    XSelectInput(   display,
                    window,
                    ButtonPressMask | KeyPressMask |
                    ExposureMask | PointerMotionMask );

    clip_rect.x = 0;
    clip_rect.y = 0;
    clip_rect.width = 64;
    clip_rect.height = 64;

    // init the background using window size
    pattern.init(200, (int)hints.width, (int)hints.height);

    bool is_clipping = true;

    /*
     * Enter event loop
     */
    while (1) {
        XEvent event;

        /*
         * Grab next event, process it
         */
        XNextEvent( display, &event );

        switch( event.type ) {
            /*
             * Repaint the window on expose events.
             */
        case Expose:
            if ( event.xexpose.count == 0 ) {
                printf("Expose event received. Redrawing\n");
                repaint(display, window, gc);
            }
            break;

        case ButtonPress:
            is_clipping = ! is_clipping; 
            break;

            // key press
        case KeyPress:
            XCloseDisplay(display);
            return 0;
            break;

        case MotionNotify: // mouse movement
            clip_rect.x = event.xmotion.x;
            clip_rect.y = event.xmotion.y;
            break;

        }
        if (!is_clipping)
          XSetClipMask(display, gc, None);
        else
          XSetClipRectangles(display, gc, 0, 0, &clip_rect, 1, Unsorted);
        

        repaint(display, window, gc);
    }
}



