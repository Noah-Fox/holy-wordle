# holy-wordle

This program uses "holy texts" to solve Wordle problems, based off of the frequency of five-letter words in each document. Assuming that Wordle is the arbiter of the universe, and ignoring all obvious logical and statistical trends, this can therefore show which religion's deities are most powerful.

Each document is processed in the following way:
- parse all strings in the document by whitespace
- remove all non-alphabetic characters
- convert to lowercase
- record the amount of occurrences of every five-letter word that is a valid guess in Wordle

There are two modes in which a document can be used to solve a Wordle:
- agnostic: uses all words which are valid Wordle solutions, even if they are not in the document
- theist: uses only words occurring in the document

A document is used to solve a Wordle problem in the following way:
- possible letters for each character space are recorded after each guess
- minimum occurrences of every letter are recorded after each guess
- for each guess, check every word in order of most occurrences to least. Once a word is found that follows the rules recorded, it is selected for a guess
- if no words can be guessed, the game is ended

For every document given, the following data is found:
- Solution words occurring: number of valid Wordle solutions that occur in the document
- Average holy word score: average number of guesses taken when solving for each word in the document, solved theistically
- Number of wins: amount of words that can be solved in six or less guesses, agnostically



asv-bible.txt - American Standard Version Bible

wordle-La.txt - Words that can be guessed in Wordle and can be the solution

wordle-Ta.txt - Words that can be guessed in Wordle but are never the solution
