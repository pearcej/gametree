/* This ia a pick up stones game with traditional recursive-based AI technique.
 * Note that there is a more efficient AI technique for this game, but this technique is generalizable.
 * The computer tries here to win if possible.
 * It is written with flags which facilitate reflection on recursion, Big O, and memoization
 * Questions for students relate to the improved efficiency of memoization.
 * Written by D. Jan Pearce, Berea College, 3/24/2018
 */

#include <iostream>
using namespace std;

// These are global constants
const int START_STONES = 8;
const int MAX_TAKE = 2;

//These are global variables
bool analyze_done[START_STONES + 1] = {false}; // sets every element of Boolean array to false
bool can_force_win[START_STONES + 1]; // creates empty Boolean array
int counter = 0; // This variable counts the number of calls to conpute win/loss
char verbose; // This setting tells you what it is doing in memory
char memoize; // This setting determines the use of memoization

int get_human_move(int stonesleft) {
    /* The get_human_move function determines the human's move.
     * It requires an integer input and returns an integer representing stones left
     */
    int hchoice;
    cout << "There are " << stonesleft << " stones left and it's your turn." << endl;
    cout << "Please enter the number of stones that you want to take: ";
    cin >> hchoice;
    if (hchoice >= 1 && hchoice <= min(MAX_TAKE, stonesleft))
        return hchoice;
    else { // when invalid choice is made
        cout << "You have to pick a number between 1 and " << min(MAX_TAKE, stonesleft) << ", so please try again." << endl;
        return get_human_move(stonesleft); // try again via recursion
    }
}

bool human_can_win(int stonesleft) {
    /* This function uses recursion to look to see if there is a move which can prevent the human from winning.
     */
    int mychoice;
    int checknum = min(MAX_TAKE, stonesleft);

    if (memoize=='y' || memoize == 'Y') { //Traditional recursion is less efficient due to lack of memoization
        if (analyze_done[stonesleft]) {
            return can_force_win[stonesleft];
        }
    }
    // look at all of the possible moves
    for (mychoice = 1; mychoice <= checknum; mychoice++) {
        // Try to find a way that the human can't win
        if (!human_can_win(stonesleft - mychoice)) { // recursive
            analyze_done[stonesleft] = true;
            can_force_win[stonesleft] = true;
            counter = counter + 1;
            if (verbose == 'y' || verbose == 'Y') {
                cout << "Wirh stonesleft = " << stonesleft << " I might win!!" << endl;
            }
            return true;
        }
    }
    // we will arrive here when none of my next moves are winning--ie the human messes up
    analyze_done[stonesleft] = true;
    can_force_win[stonesleft] = false;
    counter = counter + 1;
    if (verbose == 'y' || verbose == 'Y') {
        cout << "Wirh stonesleft = " << stonesleft << " I cannot win." << endl;
    }
    return false;
}

int get_my_move(int stonesleft) {
/* The get_my_move function determines my move as the computer.
 * It requires an integer input and returns an integer representing stones left.
 */
    int cchoice;
    int checknum = min(MAX_TAKE, stonesleft);
    cout << "There are " << stonesleft << " stones left, and it's my turn." << endl;
    // I am trying to find a winning move
    for (cchoice = 1; cchoice <= checknum; cchoice++) {
        // I am trying to find a move so the human can't win (! in the following means "not")
        if (!human_can_win(stonesleft - cchoice)) {
            cout << "I am now feeling confident, so I will take " << cchoice << " stones." << endl;
            return cchoice;
        }
    }
    // when no winning moves exist, I will play randomly
    cchoice = min(stonesleft, 1 + rand() % MAX_TAKE); // choice is a random number from 1 to MAX_TAKE
    cout << "Now, I am just trying to trip you up, so I will take " << cchoice << " stones." << endl;
    return cchoice;
}

int main() {

    int stones = START_STONES;

    cout << "We have " << stones << " stones total." << endl;
    cout << "We will take turns taking between 1 and " << MAX_TAKE << " stones each time." << endl;
    cout << "Whoever takes the last stone will win the game!" << endl;

    cout << "Do you want the verbose setting? Please enter y or n: ";
    cin >> verbose;

    cout << "Do you want to use memoization? Please enter y or n: ";
    cin >> memoize;

    while (true) {
        // the human goes first
        stones = stones - get_human_move(stones);
        if (stones == 0) {
            cout << "You took the last of the stones, so you won! Good playing\n!" << endl;
            cout << "You made "<< counter << " function calls to human_can_win()." << endl;
            return 0; // this exits and ends main function
        }
        // I go next
        stones = stones - get_my_move(stones);
        if (stones == 0){
            cout << "I took the last of the stones, so I won!! Take that, human!\n" << endl;
            cout << "You made "<< counter << " function calls to human_can_win()." << endl;
            cout << endl;
            return 0; // this exits and ends main function
        }
    }
}
