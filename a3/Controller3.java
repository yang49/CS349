
import java.awt.event.*;
import java.awt.Color;
import java.lang.String;	

class Controller3 implements ActionListener {

	Model model;
	public View view;

	Controller3(Model model) {	
		this.model = model;
	}

	// event from the view's button 
   public void actionPerformed(ActionEvent e) {
    	model.updateTable = true;
     	view.setModel = 1;
     	if(view.input.getText().length() != 5) {
   	  	view.statusLabel.setText("Please enter a FIVE-LETTER word");
        	view.status.setBackground(Color.RED);
        	return;
      }
   	model.setGuess(view.input.getText().toUpperCase());
      view.input.setText("");
	}
} 
