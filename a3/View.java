import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.*;
import java.awt.Color;
import java.lang.String;

class View extends JPanel implements IView {

	JPanel temp;
   final Model model;
   public int setModel;	// = 0 if user is setting up guess or hint model
   
   public JPanel status;
   public JLabel statusLabel;
   private int lives;

	// the view's main user interface
	final private JButton button1 = new JButton("Enter");		// for enter
	private JButton button2 = new JButton("Get hints");		// for hint
	private JButton button3 = new JButton("Give up"); 			// for giving up
	
	private JLabel guessLabel;	// display the label of the input textbox
	private JPanel guessArea;	// the input textbox
	final public JTextField input = new JTextField(5);
	
	View(final Model model, Controller controller, Controller2 controller2,
        Controller3 controller3) {
		controller.view = this;
		controller2.view = this;
		controller3.view = this;
		setModel = 1;
		lives = 10;
		// create the view UI
		temp = new JPanel();
		
		this.setLayout(new GridBagLayout());
		
		// set the model 
		this.model = model;
		// Here are the listeners
		button1.addActionListener(controller3);
      button2.addActionListener(controller);
      
 		button3.addActionListener(controller2);
      
		input.addKeyListener(controller2);
        
		guessLabel = new JLabel("Please enter a 5 letter word: ");
		guessArea = new JPanel();
		
		guessLabel.setLabelFor(input);
		
		guessArea.add(guessLabel);
		guessArea.add(input);
		
		status = new JPanel();
		status.setLayout(new BoxLayout(status, BoxLayout.X_AXIS));

		statusLabel = new JLabel();
		status.add(statusLabel);		
		statusLabel.setHorizontalAlignment(SwingConstants.LEFT);
		
		guessArea.add(button1);
		guessArea.add(Box.createRigidArea(new Dimension(45, 0)));
		guessArea.add(button2);
		guessArea.add(button3);
		
		temp.setLayout(new BoxLayout(temp, BoxLayout.PAGE_AXIS));
		temp.add(guessArea);
		temp.add(Box.createRigidArea(new Dimension(0,0)));
		temp.add(status);
		
		this.add(temp);
	}
	
	public void reset() {
		setModel = 1;
		lives = 10;
	}

	// IView interface 
	public void updateView() {
		status.setBackground(Color.YELLOW);
		if(model.won) {
			status.setBackground(Color.GREEN);
			statusLabel.setText("You've Won!!!");
			return;
		}
		if(model.newGame == 0) {
			lives = 10;
			String txt1 = "Welcome to the Game!";
			statusLabel.setText(txt1);
			model.newGame = 1;
			lives--;
		}
		else {
			String txt2 = "You have " + lives + " chance(s) left!";
			if(setModel == 1) {
				statusLabel.setText(txt2);
				lives--;
				if(lives <= -1) model.died = true;
			}
		}
	}
} 
