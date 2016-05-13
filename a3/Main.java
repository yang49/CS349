/**
 * Two views coordinated with the observer pattern.  Separate controller. 
 * The mechanics of a separate controller are starting to break down.
 */ 

import javax.swing.*;
import java.awt.*;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.*;	

public class Main{

	static Model model;

	public static void main(String[] args){	
		JFrame frame = new JFrame("Jotto");
		
		frame.setJMenuBar(Main.makeMenuBar());
		
		// create Model and initialize it
		model = new Model();
		
		if(args.length == 1) {
			model.setTarget(args[0]);
		}
		// create Controller, tell it about model
		Controller controller = new Controller(model);
		Controller2 controller2 = new Controller2(model);
		Controller3 controller3 = new Controller3(model);
		
		// create View, tell it about model and controller
		View view = new View(model, controller, controller2, controller3);
		// tell Model about View. 
		model.addView(view);
		
		// create second view ...
		View2 view2 = new View2(model);
		model.addView(view2);
		
		// create third view ...
		View3 view3 = new View3(model);
		model.addView(view3);
		
		// create the window
		//JPanel p = new JPanel(new GridLayout(10,1));
		JPanel p = new JPanel(new BorderLayout());
		frame.getContentPane().add(p);
		p.add(view, BorderLayout.PAGE_START);
      p.add(view2, BorderLayout.EAST);
      p.add(view3, BorderLayout.WEST);
		
		frame.setPreferredSize(new Dimension(700,600));
		frame.pack();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	
	private static JMenuBar makeMenuBar() {
		JMenuBar mbar = new JMenuBar();
		
		JMenu file = new JMenu("File");
		mbar.add(file);
		
		final JMenuItem reset = new JMenuItem("New Game");
		final JMenuItem quit = new JMenuItem("Quit");
		file.add(reset);
		file.add(quit);
		
		reset.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				model.reset();
			}
		});
		
		quit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (quit.getText().equals("Quit")) {
				System.exit(0);
				}
		   }
		});
		
		return mbar;
	}
}
