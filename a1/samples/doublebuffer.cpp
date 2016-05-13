/*
CS 349 Code Examples: X Windows and XLib

    doublebuffer.cpp   Demos double buffering (using same 
    				   demo as animation.cpp)

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o doublebuffer doublebuffer.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./doublebuffer

Note: the -L option and -lstdc++ may not be needed on some machines.

*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
const int Border = 5;
const int BufferSize = 10;
const int FPS = 30;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	
	Pixmap	pixmap;		// double buffer
	int		width;		// size of pixmap
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};       


class Eye : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, width, height, 0, 360*64);
			double cx = x + width/2;
			double cy = y + height/2;
						
			double alpha;
			if (look_x > cx ) alpha = atan((look_y - cy) / (look_x - cx) );
			else alpha = M_PI - atan((look_y - cy) / (cx - look_x) );
			XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
				cx + cos(alpha)*width/4 - width/8,
				cy + sin(alpha)*height/4 - height/8,
				width/4, height/4,
				0,
				360*64);
		}
		
		void lookat(int x, int y) {
			look_x = x;
			look_y = y;
		}
      
	// constructor
	Eye(int x, int y, int width, int height):x(x), y(y), width(width), height(height)  {
		look_x = x+width/2;
		look_y = y+height/2;
	}
      
	private:
		int x;
		int y;
		int width;
		int height;
		int look_x;
		int look_y;
};

class Ball : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, diameter, diameter, 0, 360*64);
		}
		
		void move(XInfo &xinfo) {
			x = x + direction;
			if (random() % 100 == 0 || x < 0 || x > xinfo.width - diameter) {
				direction = -direction;
			}
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}
		
		Ball(int x, int y, int diameter): x(x), y(y), diameter(diameter) {
			direction = 4;
		}
	
	private:
		int x;
		int y;
		int diameter;
		int direction;
};


list<Displayable *> dList;           // list of Displayables
Eye left_eye(100, 100, 100, 200);
Eye right_eye(225, 100, 100, 200);
Ball ball(100, 450, 40);


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xinfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xinfo.display = XOpenDisplay( "" );
	if ( !xinfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xinfo.screen = DefaultScreen( xinfo.display );

	white = XWhitePixel( xinfo.display, xinfo.screen );
	black = XBlackPixel( xinfo.display, xinfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 400;
	hints.height = 500;
	hints.flags = PPosition | PSize;

	xinfo.window = XCreateSimpleWindow( 
		xinfo.display,				// display where window appears
		DefaultRootWindow( xinfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xinfo.display,		// display containing the window
		xinfo.window,		// window whose properties are set
		"x3_doublebuffer",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, hints.width, hints.height, depth);
	xinfo.width = hints.width;
	xinfo.height = hints.height;


	XSelectInput(xinfo.display, xinfo.window, 
		ButtonPressMask | KeyPressMask | PointerMotionMask 
		| EnterWindowMask | LeaveWindowMask
		| StructureNotifyMask   // for resize events
		);

	/*
	 * Don't paint the background -- reduce flickering
	 */
	XSetWindowBackgroundPixmap(xinfo.display, xinfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
	
	XFlush(xinfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	// draw into the buffer
	// note that a window and a pixmap are “drawables”
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height);
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo); // the displayables know about the pixmap
		begin++;
	}
	// copy buffer to window
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window

	XFlush( xinfo.display );

}


void handleButtonPress(XInfo &xinfo, XEvent &event) {
	printf("Got button press!\n");
	// dList.push_front(new Text(event.xbutton.x, event.xbutton.y, "Urrp!"));
	// repaint( dList, xinfo );
	
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}
	}
}

void handleMotion(XInfo &xinfo, XEvent &event, int inside) {
	if (inside) {
		left_eye.lookat(event.xbutton.x, event.xbutton.y);
		right_eye.lookat(event.xbutton.x, event.xbutton.y);		
	}
}

void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	if (xce.width > xinfo.width || xce.height > xinfo.height) {
		XFreePixmap(xinfo.display, xinfo.pixmap);
		int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
		xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xce.width, xce.height, depth);
		xinfo.width = xce.width;
		xinfo.height = xce.height;
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
	ball.move(xinfo);
	if (!inside) {
		left_eye.lookat(ball.getX(), ball.getY());
		right_eye.lookat(ball.getX(), ball.getY());
	}
}

unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&ball);
	dList.push_front(&left_eye);
	dList.push_front(&right_eye);
	
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case ButtonPress:
					handleButtonPress(xinfo, event);
					break;
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case MotionNotify:
					handleMotion(xinfo, event, inside);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;
			}
		}
		
		usleep(1000000/FPS);
		handleAnimation(xinfo, inside);
		repaint(xinfo);
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	XInfo xinfo;

	initX(argc, argv, xinfo);
	eventLoop(xinfo);
	XCloseDisplay(xinfo.display);
}
