#ifndef WORDLE
#define WORDLE

#include <vector>
#include <string>

using namespace std;

typedef long long ll;

/**
 * returns score for each character:
 *  0 - letter does not exist in the solution
 *  1 - letter exists in another position of the solution
 *  2 - letter is correct
*/
vector<int> guessWord(string guess, string solution){
    vector<int> score = {0,0,0,0,0};
    vector<bool> lettersTaken = {false, false, false, false, false};

    //check for 2-scores
    for (int i = 0; i < 5; i ++){
        if (guess[i] == solution[i]){
            score[i] = 2;
            lettersTaken[i] = true;
        }
    }

    //check for 1-scores
    for (int i = 0; i < 5; i ++){
        if (score[i] == 0){
            for (int x = 0; x < 5; x ++){
                if (solution[x] == guess[i] && !lettersTaken[x]){
                    score[i] = 1;
                    lettersTaken[x] = true;
                }
            }
        }
    }

    return score;
}

#endif
