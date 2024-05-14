#include <iostream>
#include <fstream>
#include <unordered_set>
#include <map>
#include <vector>
#include "wordle.h"

using namespace std;

typedef long long ll;
typedef vector<pair<ll,string>> vls;

pair<ll,string> findBestWord(vls holyWords, vector<vector<bool>> possibleLetters, vector<int> letterCounts, bool useOnlyHolyWords);

int solveWordle(vls holyWords, string solution, bool useOnlyHolyWords);

vls parseText(string fileName, unordered_set<string> wordleSolutions, unordered_set<string> wordleGuesses);

int main(){
    //input wordle words
    unordered_set<string> wordleSolutions;
    unordered_set<string> wordleGuesses;
    ifstream wordleStream;

    //input from wordle-La
    wordleStream.open("wordle-La.txt");
    if (wordleStream.fail()){
        cout << "wordle-La not available\n";
        return 0;
    }
    string word;
    while (wordleStream >> word){
        wordleSolutions.insert(word);
        wordleGuesses.insert(word);
    }
    wordleStream.close();

    //input from wordle-Ta
    wordleStream.open("wordle-Ta.txt");
    if (wordleStream.fail()){
        cout << "wordle-Ta not available\n";
        return 0;
    }
    while (wordleStream >> word){
        wordleGuesses.insert(word);
    }
    wordleStream.close();

    
    ifstream fileNames;
    fileNames.open("bible-list.txt");
    if (fileNames.fail()){
        cout << "bible-list.txt not available\n";
        return 0;
    }

    string fileName;
    while (fileNames >> fileName){

        //parse all 5 letter words from given holy text
        vls holyWords = parseText(fileName,wordleSolutions,wordleGuesses);


        int solutionWordAmount = 0;
        int solutionWordScoreSum = 0;
        int winAmount = 0;
        int wordAmount = 0;
        for (auto it = wordleSolutions.begin(); it != wordleSolutions.end(); it ++){
            wordAmount ++;

            int theistScore = solveWordle(holyWords, *it, true);
            int agnosticScore = solveWordle(holyWords, *it, false);

            if (theistScore != -1){
                solutionWordAmount ++;
                solutionWordScoreSum += theistScore;
            }

            if (agnosticScore > 0 && agnosticScore <= 6){
                winAmount ++;
            }
        }
        cout << fileName << ":\n";
        cout << "\tSolution words occurring: " << solutionWordAmount << "\n";
        cout << "\tAverage holy word score: " << ((double)solutionWordScoreSum)/solutionWordAmount << "\n";
        cout << "\tNumber of wins: " << winAmount << "/" << wordAmount << " (" << 100.0*winAmount/wordAmount << ")\n\n";
        
    
    }

    fileNames.close();

    return 0;
}

vls parseText(string fileName, unordered_set<string> wordleSolutions, unordered_set<string> wordleGuesses){
    //open input file stream to holy text
    ifstream inStream;
    inStream.open(fileName);
    if (inStream.fail()){
        cout << fileName << " not available\n";
        return vls();
    }
    
    //input all five letter words from holy text
    string inputWord;
    map<string,ll> holyWordsMap;
    while (inStream >> inputWord){
        string word = "";
        //remove letters from word, make lowercase
        for (size_t i = 0; i < inputWord.length(); i ++){
            if (isalpha(inputWord[i])){
                word += tolower(inputWord[i]);
            }
        }
        if (word.length() == 5 && wordleGuesses.count(word)){
            if (holyWordsMap.count(word) == 0){
                holyWordsMap[word] = 1;
            }
            else {
                holyWordsMap[word] ++;
            }
        }
    }
    inStream.close();

    //put all words into a vector
    vls holyWords;
    for (auto it = holyWordsMap.begin(); it != holyWordsMap.end(); it ++){
        holyWords.push_back({it->second,it->first});
    }
    for (auto it = wordleSolutions.begin(); it != wordleSolutions.end(); it ++){
        if (holyWordsMap.count(*it) == 0){
            holyWords.push_back({0,*it});
        }
    }
    sort(holyWords.rbegin(),holyWords.rend());

    return holyWords;
}

//returns amount of guesses taken, -1 if not solved
int solveWordle(vls holyWords, string solution, bool useOnlyHolyWords){
    vector<vector<bool>> possibleLetters(5, vector<bool>(26, true));
    vector<int> letterCounts(26, 0);

    bool guessed = false;
    int guessAmount = 0;
    while (!guessed){
        guessAmount ++;
        pair<ll,string> bestGuess = findBestWord(holyWords, possibleLetters, letterCounts, useOnlyHolyWords);
        string nextGuess = bestGuess.second;

        if (nextGuess == "xxxxx"){
            return -1;
        }

        vector<int> score = guessWord(nextGuess, solution);

        guessed = true;
        vector<int> scoreLetterCounts(26,0);
        for (int i = 0; i < 5; i ++){

            if (score[i] != 0){
                scoreLetterCounts[nextGuess[i]-'a'] ++;
            }

            if (score[i] == 2){
                for (int x = 0; x < 26; x ++){
                    possibleLetters[i][x] = false;
                    possibleLetters[i][nextGuess[i]-'a'] = true;
                }
            }
            else {
                guessed = false;
                possibleLetters[i][nextGuess[i]-'a'] = false;
            }

        }

        for (int i = 0; i < 26; i ++){
            letterCounts[i] = max(letterCounts[i], scoreLetterCounts[i]);
        }
    }
    return guessAmount;
}

pair<ll,string> findBestWord(vls holyWords, vector<vector<bool>> possibleLetters, vector<int> letterCounts, bool useOnlyHolyWords){
    for (size_t i = 0; i < holyWords.size(); i ++){
        bool valid = true;
        for (int x = 0; x < 5; x ++){
            if (!possibleLetters[x][holyWords[i].second[x]-'a']){
                valid = false;
            }
        }

        vector<int> wordLetterCounts(26,0);
        for (int x = 0; x < 5; x ++){
            wordLetterCounts[holyWords[i].second[x]-'a'] ++;
        }
        for (int x = 0; x < 26; x ++){
            if (letterCounts[x] > wordLetterCounts[x]){
                valid = false;
            }
        }

        if (valid && (holyWords[i].first > 0 || !useOnlyHolyWords)){
            return holyWords[i];
        }
    }
    return {0,"xxxxx"};
}
