package jotto;


/** Classes implementing <class>IWordPredicate</code> are used to find out
 * whether or not a given word meets some criteria determined by the
 * implementation of {@link #isOK(Word w)}.
 *
 * @author Byron Weber Becker */
public interface IWordPredicate 
{

   /** Does the given Word meet a condition?
    * @param w The word-difficulty object to test
    * @return true if it meets the condition;  false otherwise. */
   public boolean isOK(Word w);
}
