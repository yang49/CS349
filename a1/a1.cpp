/*
 * NAME: Yang Li
 * USERID: 20525269
 *
 * Fall 2015 cs349 Assigment 1: An implementation of Breakout in C/C++ and Xlib.
 *
 *
 *
 * Command to compile and run:
 *
 *  g++ -o a1.cpp -L/usr/X11R6/lib -lX11 -lstdc++
 *  ./a1
 * Note: the -L option and -lstdc++ may not be needed on the same machines.
 */

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <string>
#include <sstream>


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
const int Border = 5;
const int BufferSize = 10;
int FPS = 60;
const int SPEED = 1;

const int row = 7;
const int num = 10;

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

/////
class Paddle : public Displayable {
   public:
      float x;
      float y;
      float width;
      float height;

   public:
      //constructor
      Paddle(float x, float y, float width, float height):x(x),y(y),width(width),height(height){}
  
      void setX(int x) {this->x = x;}

      virtual void paint(XInfo &xinfo) {
      	if(x + width > xinfo.width) {
         	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1],
         	               xinfo.width - width, y, width, height);
         }
         else {
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1],
                           x, y, width, height);
         }
      }
      
      void update(int width, int height, float old_width, int old_height) {
         double rX = (double) width / (double) old_width;
         double rY = (double) height / (double) old_height;
         x = x * rX;
         y = y * rY;
         this->width = this->width * rX;
         this->height = this->height * rY;
      }
};

bool OVER = false;

class Ball : public Displayable {
   public:
      float x;
      float y;
      float diameter;
      float dirX;
      float dirY;
      
      Ball(float x, float y, int diameter, float dirX, float dirY):x(x),y(y),diameter(diameter), dirX(dirX),
                                                           dirY(dirY) {}
      
      virtual void paint(XInfo &xinfo) {
          XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, diameter, diameter, 0, 360*64);
      }

      void move(XInfo &xinfo, Paddle &paddle) {
          x = x + dirX;
          y = y + dirY;
          //printf("moveing to %f  %f\n", x, y);
          //When the ball hits the paddle
          if(((float)x > (float)paddle.x) && ((float) x < (float)paddle.x + paddle.width) && (y >= paddle.y - 6)) {
             dirY = -dirY;
          }
          if(x < 0) {
             dirX = -dirX;
          }
          if((float)x > (float)xinfo.width - 7) {
              dirX = -dirX;
          }
          if((float) y < 0) {
              dirY = -dirY;
          }
          if((float)y >= (float)xinfo.height - 7) {
              dirY = -dirY;
	      OVER = true;
              x = 130;
              y = 310;
              throw(1);
          } 
      }
      void update(int width, int height, int old_width, int old_height) {
         double rX = (double) width / (double) old_width;
         double rY = (double) height / (double) old_height;
         x = x * rX;
         y = y * rY;
         dirX = dirX * rX;
         dirY = dirY * rY;
      }
};

class Block : public Displayable {
   public:
      float x;
      float y;
      float width;
      float height;
      int color;
      int score;
      bool vis;

      void setBlock(float x,float y,float width,float height,int score,int color, bool vis) {
         this->x = x;
         this->y = y;
         this->width = width;
         this->height = height;
         this->score = score;
         this->color = color;
         this->vis = vis;
      }

      virtual void paint(XInfo &xinfo) {
         if(this->vis) {
            XSetForeground(xinfo.display, xinfo.gc[2], color);
            XSetBackground(xinfo.display, xinfo.gc[2], XWhitePixel(xinfo.display, xinfo.screen));
            XFillRectangle(xinfo.display, xinfo.pixmap,xinfo.gc[1], x, y, width, height);
            XFillRectangle(xinfo.display, xinfo.pixmap,xinfo.gc[2], x+1, y+1,
                           width-2, height-2);
        }
      }

      void Block_Destroy() {
         x = 1;
         y = 1;
         width = 1;
         height = 1;
         //color = 0xFFFFFF;
         vis = false;
      }

      bool checkCollision(Ball &ball) {
         if((ball.x > (float)this->x - ball.diameter) and (ball.x < (float)this->x + (float)this->width) and
            (ball.y > (float)this->y - ball.diameter) and (ball.y < (float)this->y + (float)this->height)) {
            if((ball.x > (float)this->x - ball.diameter) && (ball.x <= (float)this->x + (float)this->width)
             && (!(ball.y > (float)this->y - ball.diameter)) && (!(ball.y< (float)this->y + (float)this->height))) {
               ball.dirX = -ball.dirX;
               x = 0;
           		y = 0;
            	width = 0;
            	height = 0;
            	return true;
            }
            else if((ball.y > (float)this->y - ball.diameter) && (ball.y< (float)this->y + (float)this->height)) {
               ball.dirY = -ball.dirY;
               x = 0;
           		y = 0;
            	width = 0;
            	height = 0;
            	return true;
            }
         }
         else {
            return false;
         }
      }
      
      void update(int width, int height, int old_width, int old_height) {
         double rX = (double) width / (double) old_width;
         double rY = (double) height / (double) old_height;
         x = x * rX;
         y = y * rY;
         this->width = this->width * rX;
         this->height = this->height * rY;
      }
};

class Item : public Displayable {
   public:
      int x;
      int y;
      int speed;
      int width;
      int color;
      bool vis;
      bool created;

      //constructor
      Item():x(0), y(0), speed(1), width(0), color(0), vis(false), created(false) {
      }
      Item(XInfo &xinfo) {
         created = true;
         this->width = 6;
         this->x = rand() % xinfo.width + 0;
         this->speed = rand() % 5 + 1;
         int tmp = rand() % 2 + 1;
         if(tmp == 1) {
            this->color = 0x20B2AA;   //Light Sea Green
         }
         if(tmp == 2) {
            this->color = 0x00008B;   //Dark Blue
         }
         y = 0;
         vis = true;
      }

      void move(XInfo &xinfo, Paddle &paddle) {
         y = y + speed;
         //printf("item moving\n");
         if((x >= paddle.x) and (x <= paddle.x + paddle.width) and (y >= paddle.y)) {
            //printf("item hits the paddle\n");
            if(color == 0x20B2AA) {
               paddle.width = paddle.width / 3 + paddle.width;
            }
            if(color == 0x00008B) {
               paddle.width = paddle.width - paddle.width / 3;
            }
            x = 0;
            y = 0;
            speed = 0;
            vis = false;
            created = false;
         }
         if(y >= xinfo.height) {
            x = 0;
            y = 0;
            vis = false;
            speed = 0;
            created = false;
         }
      }

      virtual void paint(XInfo &xinfo) {
         //printf("calling paint\n");
         if(this->vis) {
           // printf("Painting  -> ( %d, %d);\n", x, y);
            XSetForeground(xinfo.display, xinfo.gc[2], color);
            XSetBackground(xinfo.display, xinfo.gc[2], XWhitePixel(xinfo.display, xinfo.screen));
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x, y, width, width);
         }
      }
   
};

class Grid : public Displayable {
   public:
      Block my_G[row][num];
      Item item;
      int score;
      int lives;
      //constructor
      Grid() {
         score = 0;
         lives = 3;
         item = Item();
         for(int i = 0; i < num; i++) {
            my_G[0][i].setBlock(10 + (20 + 30) * i, 30,      30, 20,10, 0xFF0000, true);  //RED
            my_G[1][i].setBlock(10 + (20 + 30) * i, 30 + 30, 30, 20, 9, 0xFFA500, true);  //ORANGE
            my_G[2][i].setBlock(10 + (20 + 30) * i, 30 + 60, 30, 20, 8, 0xFFFF00, true);  //YELLOW
            my_G[3][i].setBlock(10 + (20 + 30) * i, 30 + 90, 30, 20, 7, 0x00FF00, true);  //GREEN
            my_G[4][i].setBlock(10 + (20 + 30) * i, 30 + 120, 30, 20, 6, 0x0000FF, true);  //BLUE
            my_G[5][i].setBlock(10 + (20 + 30) * i, 30 + 150, 30, 20, 5, 0xA020F0, true);  //PURPLE
            my_G[6][i].setBlock(10 + (20 + 30) * i, 30 + 180, 30, 20, 4, 0x4B0082, true);  //INDIGO
         }

      } // Grid() end
      virtual void paint(XInfo &xinfo) {
         for(int i = 0; i < lives; i++) {
            XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], xinfo.width - 30 - i * 10, 5, 7, 7, 0, 360*64);
         }
         for(int i = 0; i < num; i++) {
            my_G[0][i].paint(xinfo);
            my_G[1][i].paint(xinfo);
            my_G[2][i].paint(xinfo);
            my_G[3][i].paint(xinfo);
            my_G[4][i].paint(xinfo);
            my_G[5][i].paint(xinfo);
            my_G[6][i].paint(xinfo);
         }
         stringstream ss;
         ss << score;
         string text;
         ss >> text;
         XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 3, 20, text.c_str(), text.length());
         if(item.created) {
            item.paint(xinfo);
         }
      }

      void move(XInfo &xinfo,Paddle &paddle) {
         item.move(xinfo, paddle);
      }

      void collision(Ball &ball, XInfo &xinfo, Paddle &paddle) {
         for(int i = 0; i < row; i++) {
            for(int j = 0; j < num; j++) {
               if(my_G[i][j].checkCollision(ball)) {
                  my_G[i][j].Block_Destroy();
                  score = score + my_G[i][j].score;
                  //if(score >= 10) throw 3;    // You have won
                  if(!item.created) {
                     item = Item(xinfo);
                  }
               } // if end
            }// inner for end
         }// outer for end
      } // collision end
      
      void update(int width, int height, int old_width, int old_height) {
         for(int i = 0; i < row; i++) {
            for(int j = 0; j < num; j++) {
               my_G[i][j].update(width, height, old_width, old_height);
            }// inner loop end
         }//outer loop end
      }
};


////

list<Displayable *> dList;           // list of Displayables


Paddle paddle = Paddle(0, 395, 50, 4);
Ball ball = Ball(120,300, 7, -0.3 * SPEED, -0.7 * SPEED);
Grid grid = Grid();


void setSpeed(Ball &ball, float speed) {
   //cout << "Setting " << speed << endl;
	ball.dirX *= speed;
	ball.dirY *= speed;
}

/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xinfo) {

	if (argc == 1) {
		setSpeed(ball, SPEED);
	}
   if (argc == 2) {
   	istringstream ss(argv[1]);
   	ss >> FPS;
   	setSpeed(ball, SPEED);
   }
   if(argc == 3) {
      int mys;
   	istringstream ss(argv[1]);
   	ss >> FPS;
   	istringstream ss1(argv[2]);
   	ss1 >> mys;
      setSpeed(ball, mys);
     // cout << "zeor args"  << mys << " --- "  << FPS << endl;
      ball.dirX *= SPEED;
      ball.dirY *= SPEED;
   }

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
	hints.width = 500;
	hints.height = 400;
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
		"Break-out",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 2;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
        XSetLineAttributes(xinfo.display, xinfo.gc[i],
                             1, LineSolid, CapButt, JoinRound);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, hints.width, hints.height, depth);
	xinfo.width = hints.width;
	xinfo.height = hints.height;

	XSelectInput(xinfo.display, xinfo.window, 
	   KeyPressMask | PointerMotionMask 
		| StructureNotifyMask   // for resize events
		);

	/*
	 * Don't paint the background -- reduce flickering
	 */
	//XSetWindowBackgroundPixmap(xinfo.display, xinfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
	
	XFlush(xinfo.display);
        sleep(1);
	
/////////////////////////////////////////////////////////////////
//This is for splash window
	XFontStruct *font;
	font = XLoadQueryFont(xinfo.display, "7x14");
	XSetFont(xinfo.display, xinfo.gc[1], font->fid);

	string text("Name: Yang Li, ID: 20525269");
	XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[1], 2, 20, text.c_str(), text.length());
	string line1 = "   You move the mouse to control how the paddle moves,";
	XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 50, line1.c_str(), line1.length());
	string line2 = " neither left or right.";
	XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 70, line2.c_str(), line2.length());
        string line3 = "   When you're resizing the window, the size of the";
	XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 90, line3.c_str(), line3.length());
	string line4 = " window can't be less that 100 pixels.";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 110, line4.c_str(), line4.length());
        string line5 = "   You have 3 lives, everytime the ball touches, you will,";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 130, line5.c_str(), line5.length());
        string line6 = " loose one life. When you have no lives, the game is over.";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 150, line6.c_str(), line6.length());
	     string line7 = "   To quit the game, please press 'Q' on the keyboard.";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 170, line7.c_str(), line7.length());
        string line8 = "   When the ball hits a brick, there will be an item ";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 190, line8.c_str(), line8.length());
        string line9 = " dropped. If you pick it up, and its color is dark blue,";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 210, line9.c_str(), line9.length());
        string line10 = " the width of the paddle will decrease, if the item is";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 230, line10.c_str(), line10.length());
        string line11 = " sea green, the width of the paddle will increase.";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 0, 250, line11.c_str(), line11.length());

        font = XLoadQueryFont(xinfo.display, "10x20");
        XSetFont(xinfo.display, xinfo.gc[2], font->fid);
        XSetForeground(xinfo.display, xinfo.gc[2], 0xFF0000);
        XSetBackground(xinfo.display, xinfo.gc[2], XWhitePixel(xinfo.display, xinfo.screen));
        string line12 = "GOOD LUCK, HAVE FUN";
        XDrawString(xinfo.display, xinfo.window, xinfo.gc[2], 90, 275, line12.c_str(), line12.length());

	font = XLoadQueryFont(xinfo.display, "7x14");
	XSetFont(xinfo.display, xinfo.gc[1], font->fid);
	string end = "Press Any Key to Enter the Game.";
	XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 80.f / 400.0 * (float) xinfo.width,
	                                                      295.0 / 300.f * (float) xinfo.height, end.c_str(), end.length());
	XFlush(xinfo.display);

	XEvent event;
	while (true) {
		XNextEvent(xinfo.display, &event);
		if(event.type == KeyPress) break;
	}
/////////////////////////////////////////////////////////////////
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
               // if(d == NULL) printf("aaa\n");
                if(d != NULL) {
			d->paint(xinfo); // the displayables know about the pixmap
		}
		begin++;
	}
	// copy buffer to window
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window

	XFlush( xinfo.display );

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
		text, 				// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 				// workstation-independent key symbol
		NULL );				// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}
	}
}

void handleMotion(XInfo &xinfo, XEvent &event) {
   paddle.setX(event.xmotion.x);
   paddle.paint(xinfo);
}

void handleResize(XInfo &xinfo, XEvent &event, Paddle &paddle, Grid &grid) {
	int old_width = xinfo.width;
	int old_height = xinfo.height;
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	if (xce.width > xinfo.width || xce.height > xinfo.height) {
		XFreePixmap(xinfo.display, xinfo.pixmap);
		int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
		xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xce.width, xce.height, depth);
		xinfo.width = xce.width;
		xinfo.height = xce.height;
                //xinfo.height = xinfo.width / 3 * 4;
	}
	else {
		xinfo.width = xce.width;
                xinfo.height = xce.height;
                //xinfo.height = xinfo.width / 3 * 4;
	}
	grid.update(xinfo.width, xinfo.height, old_width, old_height);     //make the grid fit to the "new" window
        paddle.update(xinfo.width, xinfo.height, old_width, old_height);   //make the paddle fit to the "new" window
        ball.update(xinfo.width, xinfo.height, old_width, old_height);     //make the ball fit to the "new" window
}

void handleAnimation(XInfo &xinfo, Paddle &paddle) {
    try {
       ball.move(xinfo, paddle);
       grid.move(xinfo, paddle);
       grid.collision(ball, xinfo, paddle);
    }catch(int tmp) {
	if(tmp = 1) {
           grid.lives--;
           if(grid.lives == 0) throw 1;
           if (tmp == 3) throw 3;
        }
	else throw 0;
    }
}

void handleGameOver(XInfo &xinfo, Grid &grid, int &tmp) {
   XClearWindow(xinfo.display, xinfo.window);
   XFontStruct *font;
   font = XLoadQueryFont(xinfo.display, "12x24");
   XSetFont(xinfo.display, xinfo.gc[1], font->fid);

   string text;
   if(tmp == 1) {
      text = "GAME OVER";
   }
   else {
      text = " YOU WIN";
   }
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 150.0f / 400.f * (float) xinfo.width, 
                                                         100.0f / 300.0f * (float) xinfo.height , text.c_str(), text.length());


   string t = "Score : ";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 150.f/ 400.f * (float) xinfo.width,
                                                         150.f / 300.f * (float) xinfo.height , t.c_str(), t.length());

   int score = grid.score;
   stringstream ss;
   ss << score;
   string MyScore;
   ss >> MyScore;
   int length = MyScore.length();
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], (float)(250 - length * 6) / 400.0f *(float) xinfo.width, 
                                                         150.0f / 300.0f * (float) xinfo.height, MyScore.c_str(), length);

   font = XLoadQueryFont(xinfo.display, "8x16");
   XSetFont(xinfo.display, xinfo.gc[1], font->fid);
   string end = "Press Any Key to Quit the Game";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 80.0f / 400.0f * (float) xinfo.width,
                                                         300.0f / 300.0f * (float) xinfo.height, end.c_str(), end.length());

   XFlush(xinfo.display);
   
   XEvent event;
   while(true) {
      XNextEvent(xinfo.display, &event);
      if(event.type == KeyPress) exit(0);
   }
}

unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	dList.push_front(&paddle);
        dList.push_front(&ball);
        dList.push_front(&grid);
	
	XEvent event;
	unsigned long lastRepaint = 0;

	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
                if(grid.score >= 490) throw 3;
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case MotionNotify:
					handleMotion(xinfo, event);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event, paddle, grid);
					break;
			}
		} 
		else {
			try{
				handleAnimation(xinfo, paddle);
			}catch(int tmp) {
				throw tmp;
			}
			usleep(900000/FPS);
			repaint(xinfo);
		}
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {

	srand(time(NULL));

	XInfo xinfo;

	//initX(argc, argv, xinfo);`

	//cout << "Please enter the the frame-rate you want to play with -> ";
	//cin >> FPS;
	//cout << endl;
	//cout << "Please enter the speed of the ball -> ";
	//cin >> SPEED;
	//cout << endl;
	initX(argc, argv, xinfo);

	try{
		eventLoop(xinfo);
	}catch(int tmp) {
		//printf("GAME OVER\n");
		handleGameOver(xinfo, grid, tmp);
	}
	XCloseDisplay(xinfo.display);
}
