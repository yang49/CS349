/*
CS 349 Code Examples: X Windows and XLib

    hello.min     Demos text (minimal version).

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o drawing.min drawing.min.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./drawing.min

Note: the -L option and -lstdc++ may not be needed on some machines.

*/

#include <cstdlib>
#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>


Display* display;
Window window;                       

int main( int argc, char* argv[] ) {
    // see openwindow.min for the setup code ...
    display = XOpenDisplay("");         
    if (!display) exit (-1);            
    int screen = DefaultScreen(display);
int w = 200;
int h = 100;
window = XCreateSimpleWindow(display,DefaultRootWindow(display),   
     10, 10, w, h, 2, 
     BlackPixel(display, screen), WhitePixel(display, screen));  
XMapRaised(display, window);  
XFlush(display); 
sleep(1);           // let server get set up before sending drawing commands
                    // this is a hack until we get to events
// drawing demo with graphics context here ...
GC gc = XCreateGC(display, window, 0, 0);       // create a graphics context
XSetForeground(display, gc, BlackPixel(display, screen));
XSetBackground(display, gc, WhitePixel(display, screen));

// load a larger font
XFontStruct * font;
font = XLoadQueryFont (display, "12x24");
XSetFont (display, gc, font->fid);

// draw text
std::string text("hello X Windows");
XDrawImageString( display, window, gc,
                  10, h/2, text.c_str(), text.length()  );    

XFlush(display);                   
    std::cout << "ENTER2exit"; std::cin.get(); // wait for input
    XCloseDisplay(display);
}
