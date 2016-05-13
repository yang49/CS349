CS 349 X Windows and XLib Example Code
=========================================

To make ("compile and link") an example, use the included makefile with 
the name of cpp file passed as a variable. For example, to make null.cpp:

	make NAME="null"

Then, to run:

	./null

Or you can even do it one step:

	make run NAME="null"


Examples
--------

    hello.min.cpp       Draws text in window, demos larger font choice.

    null.cpp        	Creates and destroys a display (a good first test to see
                		if X Windows is working).

    openwindow.min.cpp  Opens a single blank window (minimal version)

	openwindow.cpp     	Opens a single blank window

	drawing.min.cpp	    Demos drawing and graphics context (minimal version)
       
    drawing.cpp     	Demos drawing functions and graphics contexts  

    eventloop.min.cpp   Demos events by displaying mouse motion events 
    					to console (minimal version)

    eventloop.cpp     	Demos events by drawing text at cursor location

    animation.cpp   	Demos animation using non-blocking
    					event loop.

    doublebuffer.cpp    Demos double buffering (using same 
    				    demo as animation.cpp)

    clipping.cpp        Demos clipping using a mouse controlled clipping
                        rectangle to reveal a pattern of random lines.

               

