/*
CS 349 Code Examples: X Windows and XLib

    animation.cpp   Demos animation using non-blocking
    				event loop.

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o animation animation.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./animation

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
 
const int Border = 10;
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
	int		width;		// size of window
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
			XFillArc(xinfo.display, xinfo.window, xinfo.gc[1], x, y, width, height, 0, 360*64);
			double cx = x + width/2;
			double cy = y + height/2;
						
			double alpha;
			if (look_x > cx ) alpha = atan((look_y - cy) / (look_x - cx) );
			else alpha = M_PI - atan((look_y - cy) / (cx - look_x) );
			XFillArc(xinfo.display, xinfo.window, xinfo.gc[0], 
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
			XFillArc(xinfo.display, xinfo.window, xinfo.gc[1], x, y, diameter, diameter, 0, 360*64);
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
void initX(int argc, char *argv[], XInfo &xInfo) {
	XSizeHints hints;
	unsigned long white, black;

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
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 400;
	hints.height = 500;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);


	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	//XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	// XWindowAttributes windowInfo;
	// XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	// unsigned int height = windowInfo.height;
	// unsigned int width = windowInfo.width;

	// big black rectangle to clear background
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, xinfo.width, xinfo.height);
	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}
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

void handleAnimation(XInfo &xinfo, int inside) {
	ball.move(xinfo);
	if (!inside) {
		left_eye.lookat(ball.getX(), ball.getY());
		right_eye.lookat(ball.getX(), ball.getY());
	}
}

// update width and height when window is resized
void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	if (xce.width != xinfo.width || xce.height != xinfo.height) {
		xinfo.width = xce.width;
		xinfo.height = xce.height;
	}
}

// get microseconds
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
			cout << "event.type=" << event.type << "\n";
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
	XInfo xInfo;

	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
