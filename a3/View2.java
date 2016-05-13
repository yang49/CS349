// The list view
import javax.swing.border.*;
import javax.swing.*;
import java.awt.*;
import java.awt.FlowLayout;
import java.awt.Color;
import java.awt.event.*;
import java.util.*;
import javax.swing.border.*;

class View2 extends JPanel implements IView {
	private JPanel temp;
	private JScrollPane scroll;
	private JList<String> list;
	private JLabel label1;
	//private ArrayList<String> words;

	// the model that this view is showing
	final private Model model;

	View2(Model model) {
		this.model = model;
		
		temp = new JPanel();
		scroll = new  JScrollPane();
		scroll.setPreferredSize(new Dimension(200, 405));
		list = new JList<String>();
		label1 = new JLabel("Entered:");
		// create UI
		//setBackground(Color.BLACK);
		
		temp.add(label1);
		temp.add(scroll);
		temp.setLayout(new BoxLayout(temp, BoxLayout.PAGE_AXIS));
		this.add(temp);
	}
	
	public void reset() {
	}
	
	public void updateHelper() {
		if(model.GuessORHint == 0) {
			label1.setText("Entered:");
		}
		if(model.GuessORHint == 1) {
			label1.setText("Hints:");
		}
		DefaultListModel<String> listTemp = new DefaultListModel<String>();
		if(model.GuessORHint == 0) {
			for(String str : model.haveEntered) {
				listTemp.addElement(str);
			}
		}
		else {
			if(model.hintWords != null) {
				for(String MM : model.hintWords) {
					if(MM == null) {
						listTemp.addElement("APPLE");
					}
					else {
					listTemp.addElement(MM);
					}
				}
			}
		}
		list.setModel(listTemp);
		scroll.setViewportView(list);
	}

	// IView interface
	public void updateView() {
		updateHelper();
		if (model.getCounterValue() > 0)
			this.add(new JLabel(model.getTarget()));
	}
}
