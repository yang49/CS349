/*
CS 349 Code Examples: X Windows and XLib

    null        Creates and destroys a display (a good first test to see
                if X Windows is working).

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o null null.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./null

Note: the -L option and -lstdc++ may not be needed on some machines.

*/


#include <cstdlib>
#include <iostream>
#include <X11/Xlib.h> // main Xlib header

Display* display;

int main() {
    display = XOpenDisplay(""); // open display (using DISPLAY env var)
    if (display == NULL) {
        std::cout << "error\n";
        exit (-1);
    } else {
        std::cout << "success!\n";

    XCloseDisplay(display);     // close display
    }
}
