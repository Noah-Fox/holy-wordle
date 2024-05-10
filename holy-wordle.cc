#include <iostream>
#include <fstream>
#include <unordered_set>
#include <map>
#include <vector>
#include "wordle.h"

using namespace std;

typedef long long ll;

pair<ll,string> findBestWord(vector<pair<ll,string>> holyWords, vector<vector<bool>> possibleLetters, vector<int> letterCounts);

int solveWordle(vector<pair<ll,string>> holyWords, string solution);

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

    //open input file stream to bible
    ifstream bibleStream;
    bibleStream.open("asv-bible.txt");
    if (bibleStream.fail()){
        cout << "Bible not available\n";
        return 0;
    }

    //input all five letter words from bible
    map<string,ll> holyWordsMap;
    while (bibleStream >> word){
        if (word.length() == 5){
            bool valid = true;
            for (ll i = 0; i < 5; i ++){
                if (!isalpha(word[i])){
                    valid = false;
                }
                if (isupper(word[i])){
                    word[i] = tolower(word[i]);
                }
            }
            if (valid && wordleGuesses.count(word)){
                if (holyWordsMap.count(word) == 0){
                    holyWordsMap[word] = 1;
                }
                else {
                    holyWordsMap[word] ++;
                }
            }
        }
    }
    bibleStream.close();

    //put all words into a vector
    vector<pair<ll,string>> holyWords;
    for (auto it = holyWordsMap.begin(); it != holyWordsMap.end(); it ++){
        holyWords.push_back({it->second,it->first});
    }
    for (auto it = wordleSolutions.begin(); it != wordleSolutions.end(); it ++){
        if (holyWordsMap.count(*it) == 0){
            holyWords.push_back({0,*it});
        }
    }
    sort(holyWords.rbegin(),holyWords.rend());

    
    
    

    int guessSum = 0;
    int wordAmount = 0;
    int winAmount = 0;
    for (auto it = wordleSolutions.begin(); it != wordleSolutions.end(); it ++){
        int guessAmount = solveWordle(holyWords, *it);
        guessSum += guessAmount;
        wordAmount ++;
        if (guessAmount <= 6){
            winAmount ++;
        }
    }

    cout << "Average score: " << guessSum/wordAmount << "\n";
    cout << "Win amount: " << winAmount << "/" << wordAmount << " (" << 100*winAmount/wordAmount << "%)" << "\n";

    return 0;
}

int solveWordle(vector<pair<ll,string>> holyWords, string solution){
    vector<vector<bool>> possibleLetters(5, vector<bool>(26, true));
    vector<int> letterCounts(26, 0);

    bool guessed = false;
    int guessAmount = 0;
    while (!guessed){
        guessAmount ++;
        pair<ll,string> bestGuess = findBestWord(holyWords, possibleLetters, letterCounts);
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

pair<ll,string> findBestWord(vector<pair<ll,string>> holyWords, vector<vector<bool>> possibleLetters, vector<int> letterCounts){
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

        if (valid){
            return holyWords[i];
        }
    }
    return {0,"xxxxx"};
}
