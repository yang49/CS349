/*
CS 349 Code Examples: X Windows and XLib

    openwindow.min     Opens a single blank window.

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o openwindow.min openwindow.min.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./openwindow.min

Note: the -L option and -lstdc++ may not be needed on some machines.

*/

#include <cstdlib>
#include <iostream>
#include <X11/Xlib.h>

Display* display;
Window window;                          // save the window id 

int main( int argc, char* argv[] ) {
    display = XOpenDisplay("");         // open display using DISPLAY env var
    if (!display) exit (-1);            // couldn't open, so bail
    int screen = DefaultScreen(display);// info about the display
    window = XCreateSimpleWindow(
         display,                       // display where window appears
         DefaultRootWindow(display),    // window's parent
         10, 10,                        // location: x,y
         400, 300,                      // size: width, height
         2,                             // width of border
         BlackPixel(display, screen),   // foreground colour
         WhitePixel(display, screen));  // background colour
    XMapRaised(display, window);        // put window on screen
    XFlush(display);                    // flush the output buffer
    std::cout << "ENTER2exit"; std::cin.get(); // wait for input
    XCloseDisplay(display);
}
