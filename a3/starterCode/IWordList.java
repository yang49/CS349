package jotto;


/** Classes implementing <code>IWordList</code> provide a list of
 * word-difficulty pairs ({@link Word} objects).
 *
 * <p>The <code>becker</code> library contains a file of approximately
 * 4,250 word-difficulty pairs.  This file may be opened with the following
 * code:</p>
 * <pre>
 import java.io.*;
 ...
 InputStream inStream = IWordList.class.getResourceAsStream("words.txt");
 Scanner in = new Scanner(new InputStreamReader(inStream));
 * </pre>
 *
 * <p>Alternatively, the file may be downloaded <a href="doc-files/words.txt">here</a>.</p>
 * 
 * @author Byron Weber Becker */
public interface IWordList 
{

   /** Does the list of words contain a word-difficulty pair with the
    * given word?
    * @param aWord a word to search for in the list
    * @return true if the word is contained in the list; false otherwise. */
   public boolean contains(String aWord);
   
   /** The number of words in the list.
    * @return the number of words in the list. */
   public int numWords();

   /** Find a random word.  The list must contain at least one word.
    * @return a random word from the list. */
   public Word randomWord();
	   
   /** Find a random word with the given difficulty in the list.  The list
    * must contain at least one such word.
    * @param difficulty The required difficulty, one of 0..2
    * @return a word with the required difficulty */
   public Word randomWord(int difficulty);
   
   /** Find one word that passes the given test;  the test is typically
    * given by a {@link Hint} object (which implements IWordPredicate).  
    * The method should start
    * with a random word in the list and then proceed sequentially through
    * the list until it either finds a word that passes the test or it
    * it exhausts the list, in which case it returns <code>null</code>.
    * @param test The test that the Word must pass
    * @return a word passing the test or <code>null</code> if no such word exists. */
   public Word getWord(IWordPredicate test);
   
   /** Find all the words that pass the given test;  the test is typically
    * given by a {@link Hint} object (which implements IWordPredicate).
    * @param test The test that each returned Word must pass.
    * @return a filled array of Words that pass the test;  an array with length
    * 0 if no such words exist. */
   public Word[] getWords(IWordPredicate test);
   
   /** Find at most maxDesired words that pass the given test;  the test is typically
    * given by a {@link Hint} object (which implements IWordPredicate).
    * @param maxDesired the maximum number of words to return.
    * @param test The test that each returned Word must pass.
    * @return a filled array of Words that pass the test;  an array with length
    * 0 if no such words exist. */
   public Word[] getWords(int maxDesired, IWordPredicate test);
}
