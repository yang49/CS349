/* Copyright (c) 2003 by  
 * Byron Weber Becker, University of Waterloo, Waterloo, Ontario, Canada. 
 * All rights reserved.  Source code is released only with the written
 * permission of the author and may not be redistributed.
 */

package jotto;


import java.util.Scanner;


/** Store one word together with its difficulty.
 *
 * @author Byron Weber Becker */
public class Word extends Object
{
   private String theWord;
   private int difficulty;
	
   /** Read one word from an open file.  
    * <p>File format is a word followed by its difficulty, one pair per
    * line.  The difficulty must be in the range of 0..2.  The file cursor
    * is expected immediately before the word and will be left immediately
    * before the following word in the file.  For example:</p>
    * <pre>TRUCK 0
    *FOLLY 1
    *LIGHT 0
    *EMBAY 2
    *...
    *</pre>
    *
    * @param in An open input file. */
   public Word(Scanner in)
   {  super();
      this.theWord = in.next();
      this.difficulty = in.nextInt();
      in.nextLine();
		
      if (this.theWord.length() != JottoModel.NUM_LETTERS)
      {  throw new Error(
               "The word '" + this.theWord + "' does not have "
               + JottoModel.NUM_LETTERS + " letters.");
      }
      if (this.difficulty < 0 || this.difficulty >= JottoModel.LEVELS.length - 1)
      {  throw new Error(
               "The word '" + this.theWord + "' has an invalid difficulty: "
               + this.difficulty);
      }
   }
	
   /** Construct a new Word object.
    * @param aWord a word with the length specified by JottoModel.NUM_LETTERS
    * @param aLevel a difficulty level.  One of {0, 1, 2}. */
   public Word(String aWord, int aLevel)
   {  super();
      this.theWord = aWord;
      this.difficulty = aLevel;
   }
	
   /** Get the difficulty level.
    * @return the word's difficulty level */
   public int getDifficulty()
   {  return this.difficulty;
   }
	
   /** Get the word.
    * @return the word. */
   public String getWord()
   {  return this.theWord;
   }
   
   /*
    public boolean containsLetter(char letter)
    {  return this.theWord.indexOf(letter) >= 0;
    }
    */
}
