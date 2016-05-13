import java.util.ArrayList;

// View interface
interface IView {
	public void updateView();
	public void reset();
}

public class Model {

   public static int NUM_LETTERS = 5;
	public static  String[] LEVELS = {
      "Easy", "Medium", "Hard", "Any Difficulty"};

	// the data in the model, just a counter
	private int counter;	
	public WordList wordlist;
	public int newGame;
	public int GuessORHint;	  // = 0 in the Guess model, = 1 in the hint model
	public boolean updateTable;
	public boolean died;
	
	private int diff = 1;
	private Word target; 	// This is the target word
	public String guess;		
	
	public boolean won;
	
	// contains array of words start with the given letters
	public ArrayList<String> haveEntered;
	// contains array of words start with the given letters plus the next letter
	// of the target word
	public ArrayList<String> hintWords;
	
	// all views of this model
	private ArrayList<IView> views = new ArrayList<IView>();
	
	// constructor
	Model() {
		died = false;
		updateTable = true;
		won = false;
		newGame = 0;
		haveEntered = new ArrayList<String>();
		wordlist = new WordList("words.txt");
		if(diff <= 0) {
			target = wordlist.randomWord(diff);
		}
		else {
			target = wordlist.randomWord();
		}
		//System.out.println(target.getWord());
	}
	
	public void setTarget(String s) {
		target = new Word(s.toUpperCase());
	}
	
	// calculate the exact
	// compare guess(String) and target.getWord() (which returns a String)
	public int getExcat() {
		System.out.println(target.getWord());
		int retval = 0;
		for(int i = 0; i <= 4; i++) {
			if(target.getWord().charAt(i) == guess.charAt(i)) {
				retval++;
			}
		}
		if(retval == 5) {
			won = true;
			System.out.println("WON!!!");
			notifyObservers();
			return 5;
		}
		return retval;
	}
	
	// calculate the partial
	// compare guss(String) and target.getWord() (which returns a String)
	public int getPartial() {
		int retval = 0;
		for(int i = 0; i <= 4; i++) {
			for(int j = 0; j <= 4; j++) {
				if((target.getWord().charAt(i) == guess.charAt(j)) && (i != j)) {
				retval++;
				break;
				}
			}
		}
		return retval;
	}
	
	// set the view observer
	public void addView(IView view) {
		views.add(view);
		view.updateView();
	}
	
	public int getCounterValue() {
		return counter;
	}
	
	public void incrementCounter() {
		counter++;
		System.out.println("Model: increment counter to " + guess);
		notifyObservers();
	} 	
	
	// notify the IView observer
	public void notifyObservers() {
		if(!died) {
			for (IView view : this.views) {
			//	System.out.println("Model: notify View");
				view.updateView();
			}
		}
	}
	
	public String getTarget() {
		return target.getWord();
	}

	public void insertHaveEntered(String s) {
		haveEntered.add(s);
	}
		
	public void setGuess(String s) {
		guess = s;
		insertHaveEntered(s);
		notifyObservers();
	}
	
	public void getHint(String s) {
		int leng = s.length();
		hintWords = new ArrayList<String>();
		for(int i = 0; i < wordlist.numUsed; i++) {
			if(wordlist.words[i].getWord().contains(s)) {
				hintWords.add(wordlist.words[i].getWord());
			}
		}
	}
	
	public void showHint(String s) {
		int leng = s.length();
		ArrayList<String> wl = wordlist.getWordList(s);
		notifyObservers();
	}
	
	public int check() {	// returns 1 if they are the same
		if (guess == target.getWord()) {
			return 1;
		}
		else {
			return 0;
		}
	}
	
	public void reset() {
		newGame = 0;
		GuessORHint = 0;
		updateTable = false;
		haveEntered = new ArrayList<String>();
		for (IView view : this.views) {
			//	System.out.println("Model: notify View");
			view.reset();
		}
		notifyObservers();
	}
}
