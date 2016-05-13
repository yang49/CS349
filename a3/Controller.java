
import java.awt.event.*;	

class Controller implements ActionListener, MouseListener {

	Model model;
	public View view;

	Controller(Model model) {	
		this.model = model;
	}

	// event from the view's button 
   public void actionPerformed(ActionEvent e) {
     	model.updateTable = false;
     	view.setModel = 0;
     	String str = view.input.getText().toUpperCase();
     	if(model.GuessORHint == 0) {
     		model.GuessORHint = 1;
     		model.showHint(str);
     	}
     	else {
     		model.GuessORHint = 0;
     	}
   }

	public void mouseClicked(MouseEvent e) { }

	public void mouseEntered(MouseEvent e) { }
	public void mouseExited(MouseEvent e) { }
	public void mousePressed(MouseEvent e) { }
	public void mouseReleased(MouseEvent e) { }

} 
