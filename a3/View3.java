// The table view

import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;


class View3 extends JPanel implements IView {
	private JTable table = new JTable();
	private Model model;
	private JScrollPane scroll;
	private DefaultTableModel defaultTable;
	private int counter;
	
	private int bool = 0;
	
	public View3(final Model model) {
		counter = 0;
		this.model = model;
		
		JPanel tableP = new JPanel();
		tableP.setLayout(new BoxLayout(tableP, BoxLayout.LINE_AXIS));
		// Here to init the table
		String[] tiles = {"#","Word", "Exact", "Partial"};
		defaultTable = new DefaultTableModel(0,0);
		defaultTable.setColumnIdentifiers(tiles);
		
		table.setModel(defaultTable);
	
		///////////////////////////////////////////////////////////////////		
		scroll = new JScrollPane(table);
		tableP.add(scroll);
		
		tableP.setLayout(new BoxLayout(tableP, BoxLayout.LINE_AXIS));
		this.add(tableP);
	}
	
	public void reset() {
		counter = 1;
		JPanel tableP = new JPanel();
		tableP.setLayout(new BoxLayout(tableP, BoxLayout.LINE_AXIS));
		// Here to init the table
		String[] tiles = {"#","Word", "Exact", "Partial"};
		defaultTable = new DefaultTableModel(0,0);
		defaultTable.setColumnIdentifiers(tiles);
		
		table.setModel(defaultTable);
		scroll = new JScrollPane(table);
		tableP.add(scroll);
		
		tableP.setLayout(new BoxLayout(tableP, BoxLayout.LINE_AXIS));
		this.add(tableP);
	}
	
	public void updateView() {
   	if(model.updateTable) {
      	if(bool != 0) {
      		if(!model.won) {
      			defaultTable.addRow(new Object[] {counter,model.guess, model.getExcat(), model.getPartial()});
      		}
      	}
      	bool = 1;
      	counter++;
    	}
    }
}
