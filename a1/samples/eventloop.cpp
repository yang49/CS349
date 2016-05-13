/*
CS 349 Code Examples: X Windows and XLib

    eventloop     Displays text at cursor location on mouse button press

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o eventloop eventloop.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./eventloop

Note: the -L option and -lstdc++ may not be needed on some machines.

*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <vector>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

/*
 * Defines for window geometry and keyboard events
 */

const int Border = 5;
const int BufferSize = 10;


/*
 * Information to draw on the window.
 */
struct XInfo {
    Display*  display;
    Window   window;
    GC       gc;
};


/*
 * An abstract class representing displayable things.
 */
class Displayable {
public:
    virtual void paint(XInfo& xinfo) = 0;
};

/*
 * A text displayable
 */
class Text : public Displayable {
public:
    virtual void paint(XInfo& xinfo) {
        XDrawImageString( xinfo.display, xinfo.window, xinfo.gc,
                          this->x, this->y, this->s.c_str(), this->s.length() );
    }

    // constructor
    Text(int x, int y, string s):x(x), y(y), s(s)  {}

private:
    XPoint p; // a 2D point (see also http://tronche.com/gui/x/xlib/graphics/drawing/)
    int x;
    int y;
    string s; // string to show
};

/*
 * A displayable polyline
 */
class Polyline : public Displayable {
public:
    virtual void paint(XInfo& xinfo) {
        // note the trick to pass a stl vector as an C array
        XDrawLines(xinfo.display, xinfo.window, xinfo.gc,
                   &points[0], points.size(),  // vector of points
                   CoordModeOrigin ); // use absolute coordinates 
    }

    // constructor
    Polyline(int x, int y) {
        add_point(x,y);
    }

    // add another point to the line
    void add_point(int x, int y) {
        XPoint p; // XPoint is a built in struct
        p.x = x;
        p.y = y;
        points.push_back(p);
    }

private:
    // need to use a vector to dynamically add points
    vector < XPoint > points; // XPoint is a built in struct
};

/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}

/*
 * Function to repaint a display list
 */
void repaint( list<Displayable*> dList, XInfo& xinfo) {
    list<Displayable*>::const_iterator begin = dList.begin();
    list<Displayable*>::const_iterator end = dList.end();

    XClearWindow( xinfo.display, xinfo.window );
    while( begin != end ) {
        Displayable* d = *begin;
        d->paint(xinfo);
        begin++;
    }
    XFlush( xinfo.display );
}

/*
 * The loop responding to events from the user.
 */
void eventloop(XInfo& xinfo) {
    XEvent event;
    KeySym key;
    char text[BufferSize];
    list<Displayable*> dList;            // list of Displayables

    Polyline* polyline = NULL;

    while( true ) {
        XNextEvent( xinfo.display, &event );

        switch( event.type ) {

            // Repaint the window on expose events.
        case Expose:
            cout << "Expose count " << event.xexpose.count << endl;
            if ( event.xexpose.count == 0 ) {
                repaint( dList, xinfo);
            }
            break;

            // add item where mouse clicked
        case ButtonPress:
            // output to console
            cout << "ButtonPress at " << event.xbutton.time << endl;
            // add a new text object to the display list
            dList.push_back(new Text(event.xbutton.x, event.xbutton.y, "CLICK"));
            // start a new polyline to draw
            polyline = new Polyline(event.xbutton.x, event.xbutton.y);
            dList.push_back(polyline);
            repaint( dList, xinfo );
            break;

            // add item where mouse moved
        case MotionNotify:

            // add a new text object to the display list
            //dList.push_back(new Text(event.xmotion.x, event.xmotion.y, "C"));
            // add the point to the drawing
            if (polyline != NULL) {
                polyline->add_point(event.xmotion.x, event.xmotion.y);

            }
            repaint( dList, xinfo );

            break;
            /*
             * Exit when 'q' is typed.
             * Arguments for XLookupString :
             *                 event - the keyboard event
             *                 text - buffer into which text will be written
             *                 BufferSize - size of text buffer
             *                 key - workstation independent key symbol
             *                 0 - pointer to a composeStatus structure
             */
        case KeyPress:
            int i = XLookupString( (XKeyEvent*)&event, text, BufferSize, &key, 0 );
            cout << "KeySym " << key 
                 << " text='" << text << "'"
                << " at " << event.xkey.time 

                 << endl;
            if ( i == 1 && text[0] == 'q' ) {
                cout << "Terminated normally." << endl;
                XCloseDisplay(xinfo.display);
                return;
            }
            break;
        }
    }
}

/*
 * Create the window;  initialize X.
 */
void initX(int argc, char* argv[], XInfo& xinfo) {
    XSizeHints hints;
    unsigned long background, foreground;
    int screen;

    /*
     * Display opening uses the DISPLAY  environment variable.
     * It can go wrong if DISPLAY isn't set, or you don't have permission.
     */
    xinfo.display = XOpenDisplay( "" );
    if ( !xinfo.display ) {
        error( "Can't open display." );
    }

    /*
     * Find out some things about the display you're using.
     */
    screen = DefaultScreen( xinfo.display );
    background = WhitePixel( xinfo.display, screen );
    foreground = BlackPixel( xinfo.display, screen );

    /*
     * Set up hints and properties for the window manager, and open a window.
     * Arguments to XCreateSimpleWindow :
     *                 display - the display on which the window is opened
     *                 parent - the parent of the window in the window tree
     *                 x,y - the position of the upper left corner
     *                 width, height - the size of the window
     *                 Border - the width of the window border
     *                 foreground - the colour of the window border
     *                 background - the colour of the window background.
     * Arguments to XSetStandardProperties :
     *                 display - the display on which the window exists
     *                 window - the window whose properties are set
     *                 Hello1 - the title of the window
     *                 Hello2 - the title of the icon
     *                 none - a pixmap for the icon
     *                 argv, argc - a comand to run in the window
     *                 hints - sizes to use for the window.
     */
    hints.x = 100;
    hints.y = 100;
    hints.width = 400;
    hints.height = 300;
    hints.flags = PPosition | PSize;
    xinfo.window = XCreateSimpleWindow( xinfo.display, DefaultRootWindow( xinfo.display ),
                                        hints.x, hints.y, hints.width, hints.height,
                                        Border, foreground, background );
    XSetStandardProperties( xinfo.display, xinfo.window, "eventloop", "eventloop", None,
                            argv, argc, &hints );

    /*
     * Get a graphics context and set the drawing colours.
     * Arguments to XCreateGC
     *           display - which uses this GC
     *           window - which uses this GC
     *           GCflags - flags which determine which parts of the GC are used
     *           GCdata - a struct that fills in GC data at initialization.
     */
    xinfo.gc = XCreateGC (xinfo.display, xinfo.window, 0, 0 );
    XSetBackground( xinfo.display, xinfo.gc, background );
    XSetForeground( xinfo.display, xinfo.gc, foreground );


    // Tell the window manager what input events you want.
    XSelectInput( xinfo.display, xinfo.window,
                  ButtonPressMask | KeyPressMask |
                  ExposureMask | ButtonMotionMask );

    /*
     * Put the window on the screen.
     */
    XMapRaised( xinfo.display, xinfo.window );
}



/*
 * Start executing here.
 *   First initialize window.
 *   Next loop responding to events.
 *   Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char* argv[] ) {
    XInfo xinfo;
    initX(argc, argv, xinfo);
    eventloop(xinfo);
}
