
import java.awt.event.*;
import java.awt.Color;
import java.lang.String;	

class Controller2 implements ActionListener, MouseListener, KeyListener {

	Model model;
	public View view;

	Controller2(Model model) {	
		this.model = model;
	}

	// event from the view's button 
   public void actionPerformed(ActionEvent e) {
     	model.updateTable = true;
     	view.setModel = 1;
      model.setGuess("     ");
      view.input.setText("");
   }
      
   public void keyTyped(KeyEvent e) {
   }

   public void keyPressed(KeyEvent e) {
     	if (e.getKeyCode() == KeyEvent.VK_ENTER) {
     		model.updateTable = true;
        	if(view.input.getText().length() != 5) {
        		view.statusLabel.setText("Please enter a FIVE-LETTER word");
        		view.status.setBackground(Color.RED);
        		return;
        	}
         model.setGuess(view.input.getText().toUpperCase());
         view.input.setText("");
      }
   }
         
   public void keyReleased(KeyEvent e) {
   	String str = view.input.getText().toUpperCase();
     	if(model.GuessORHint == 1) {
      	model.updateTable = false;
      	model.getHint(str);
      	model.notifyObservers();
      }
   }

	public void mouseClicked(MouseEvent e) { }

	public void mouseEntered(MouseEvent e) { }
	public void mouseExited(MouseEvent e) { }
	public void mousePressed(MouseEvent e) { }
	public void mouseReleased(MouseEvent e) { }

} 
