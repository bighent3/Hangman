//Henton Hailey-Marshall
//CSE 143 AA with Ido Avnon
//Homework A5: EvilHangman
import java.util.*;
//This class builds an evil hangman game that accepts a collection of words, a user given word
//length and number of guesses. Then the uses gives letters in an attempt to guess this classes
//word. This class responds by attempting have the user not guess its word by accepting the users
//input and the cutting its library to a library that does or doesnt include the users letter
//depending on which has the greatest amount of options
public class HangmanManager {
   private Set<String> currentWords;
   private Set<Character> guessedLetters;
   private int maxGuesses;
   private int guessesLeft;
   private String currentPattern;
   
   //Accepts a dictionary of words, a user given length, and a max number of guesses
   //Throws an IllegalArgumentException if word has 0 characters
   //Throws an IllegalArgumentException if user asks for negative guesses
   //Throws an IllegalArgumentException if no dictionary word equals user given word size
   //Post: adds word from dictionary equal to user given length to a Hashset
   //Creates starting display of correct guesses
   ///Begins the game
   public HangmanManager(Collection<String> dictionary, int length, int max) {
      if (length < 1 || max < 0) {
         throw new IllegalArgumentException(
         "Length cannot be less than 1, guesses cannot be less than 0");
      }
      
      currentWords = new HashSet<>();
      for (String word : dictionary) {
         if (word.length() == length) {
            currentWords.add(word);
         }
      }
      
      if (currentWords.isEmpty()) {
         throw new IllegalArgumentException("No words of the user given length");
      }
      
      guessedLetters = new HashSet<>();
      maxGuesses = max;
      guessesLeft = max;
      currentPattern = createFirstPattern(length);
   }
   
   //Returns a hashset of added words
   public Set<String> words() {
      return new HashSet<>(currentWords);
   }
   
   //returns int of number of guesses left
   public int guessesLeft() {
      return guessesLeft;
   }
   
   //returns an unordered set of user guessed letters
   public Set<Character> guesses() {
      return new HashSet<>(guessedLetters);
   }
   
   //throws illegalstatexception if set of words available is empty
   //returns current pattern of words displayed for hangman game
   public String pattern() {
      if (currentWords.isEmpty()) {
         throw new IllegalStateException("No words left to choose from");
      }
      
      return currentPattern;
   }
   
   //throws illegalargumentexception if number of guesses is less than 0 or list of available
   //words is 0
   //throws illegalargumentexception is a letter is guessed more than once
   //post: updates and displays new pattern of words with guesses made and tell how many of
   //that letter were guessed
   public int record(char guess) {
      if (guessesLeft < 1 || currentWords.isEmpty()) {
         throw new IllegalStateException("Game is over or no words available");
      }
      if (guessedLetters.contains(guess)) {
         throw new IllegalArgumentException("Letter has already been guessed");
      }
      guessedLetters.add(guess);
      
      Map<String, Set<String>> patternGroups = new HashMap<>();
      
      for (String word : currentWords) {
         String pattern = buildPattern(word);
         if (!patternGroups.containsKey(pattern)) {
            patternGroups.put(pattern, new HashSet<>());
         }
         
         patternGroups.get(pattern).add(word);
      }
      
      String largestPattern = null;
      int maxGroup = 0;
      for (String pattern : patternGroups.keySet()) {
         int groupSize = patternGroups.get(pattern).size();
         if (groupSize > maxGroup) {
            largestPattern = pattern;
            maxGroup = groupSize;
         }
      }
      
      currentWords = patternGroups.get(largestPattern);
      currentPattern = largestPattern;
      int occurrences = numOccurrences(currentPattern, guess);
      
      if (occurrences == 0) {
         guessesLeft--;
      }
      
      return occurrences;
   }
   
   //helper method accepts user parameter length and draws default blank pattern
   //returns a display of "- " with last space cut off
   private String createFirstPattern(int length) {
      String pattern = "";
      for (int i = 0; i < length; i++) {
         pattern += "- ";
      }
      return pattern.trim();
   }
   
   //helper method accepts a word replaces dash with letter guessed by user
   //returns display with letter in space and blank spot trimmed off of end
   private String buildPattern(String word) {
      String pattern = "";
      for (int i = 0; i < word.length(); i++) {
         if (guessedLetters.contains(word.charAt(i))) {
            pattern += word.charAt(i) + " ";
         } else {
            pattern += "- ";
         }
      }
      return pattern.trim();
   }
   
   //helper method accepts current pattern and letter guessed by user.
   //returns a count of letter guessed to tell user how many of those words were present
   private int numOccurrences(String pattern, char guess) {
      int count = 0;
      for (int i = 0; i < pattern.length(); i++) {
         if (pattern.charAt(i) == guess) {
            count++;
         }
      }
      return count;
   }
}