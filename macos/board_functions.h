#ifndef BOARD_FNS_H //guard
#define BOARD_FNS_H //guard

#include <iostream>  //cout
#include <ncurses.h> //keys
#include <vector>    //vectors
#include <utility>   //pairs

#include "classes.h" //structs
#include "score.h"   //files 

using namespace std; //less typing 

//FUNCTIONS ---------------
void init_board();        //
void clear_board();       //
void draw_board();        //
void add_block(block B);  //
void clear_rows();        //
bool check_game_over();   //
//------------------------

void init_board(){ //set each position to number
    for(int i = 0; i<BOARD_HEIGHT; ++i){    //for each row
        for(int j = 0; j<BOARD_WIDTH; ++j){ //for each col
            BOARD[i][j] = 0;                //make it mean nothing 
        }
    }
    //initialize the bottom row to act like blocks (for collision):
    for(int i = 1; i<BOARD_WIDTH-1; ++i){ //for each col
        BOARD[BOARD_HEIGHT-1][i] = 1;     //make it 1
    }
}


void clear_board(){ //Reset EVERYTHING 
    init_board();   //clear
    SCORE = 0;      //score reset
}

void draw_board(){ //draw the board

    //VARIABLES:
    char const* u = "█"; //ascii block
    int j2; //another j used for display purposes

    //DRAW BORDERS & COLOR
    clear(); //clear screen before just in case 

    for (int i = 0; i < BOARD_HEIGHT; i++){ //for each ROW

        j2 = 0; //second j that increments by 2 

        for (int j = 0; j < BOARD_WIDTH; j++){ //for each COL

            int color = BOARD[i][j]; //the color is the pos val
            if (color > 0 && i != BOARD_HEIGHT-1) { // | if there is color:
                attron(COLOR_PAIR(color));          // | > enable it 
                mvprintw(i,j2, "%s%s", u ,u);       // | > print it at (i,j2)
                attroff(COLOR_PAIR(color));         // | > disable it 
            }else{
               attroff(COLOR_PAIR(color));          // | > make sure there is no color
                mvprintw(i, j2, "  ");              // | > normal space 
            }

            if (i == 0 || i == BOARD_HEIGHT-1 || j == 0 || j == BOARD_WIDTH-1){ //if edges
                if(i == 0 && j == 0){ //if corner
                    mvprintw(i, j2," ╔"); //print
                }else if(i == 0 && j == BOARD_WIDTH-1){ //if corner
                    mvprintw(i, j2,"╗ "); //print
                }else if(i == BOARD_HEIGHT-1 && j == 0){ //if corner
                    mvprintw(i, j2," ╚"); //print
                }else if(i == BOARD_HEIGHT-1 && j == BOARD_WIDTH-1){ //if corner
                    mvprintw(i, j2,"╝ "); //print
                }else if(i == 0 || i == BOARD_HEIGHT-1){ //if border
                    mvprintw(i, j2,"══"); //print
                }else if(j == 0){ //if border
                    mvprintw(i, j2," ║"); //print
                }else{ //if border
                    mvprintw(i, j2,"║ "); //print
                }
            }
            j2 += 2; //increment j2 by 2
        }
    }

    mvprintw(1, BOARD_WIDTH*2 + 1, "SCORE: %d", SCORE); //print score
    mvprintw(2, BOARD_WIDTH*2 + 1, "HIGH SCORE: %d", read_score(DIFFICULTY)); //print highscore

    mvprintw(BOARD_HEIGHT-6, BOARD_WIDTH*2 + 1, "CONTROLS"); //print control
    mvprintw(BOARD_HEIGHT-5, BOARD_WIDTH*2 + 1, "^ OR W - rotate"); //print control
    mvprintw(BOARD_HEIGHT-4, BOARD_WIDTH*2 + 1, "< OR A - left"); //print control
    mvprintw(BOARD_HEIGHT-3, BOARD_WIDTH*2 + 1, "V OR S - down"); //print control
    mvprintw(BOARD_HEIGHT-2, BOARD_WIDTH*2 + 1, "> OR D - right"); //print control
    
}

void add_block(block B){ //add block to board
    for(pair<int,int>p : B.coords){ //for each coordinate
        BOARD[p.first][p.second] = B.type; //add it to the board
    }
}

void clear_rows(){ //clear full rows

    int rows_cleared_num = 0; //number of rows cleared
    bool is_full; //temp variable for whether curr row is full or not 

    for (int i = BOARD_HEIGHT-2; i > 0; --i){ //for each ROW
        is_full = true; //flag
        for (int j = 1; j < BOARD_WIDTH-1; j++){ //for each COL
            if (BOARD[i][j] == 0){ //if it is not a block
                is_full = false; //then it is not full
                break; //exit early 
            }
        }
        // if the row is full, clear it and move all the blocks above it down one row
        if (is_full){  //check
            rows_cleared_num++; //self explanatory
            for (int k = i; k > 0; k--){ //for each sub row
                for (int j = 1; j < BOARD_WIDTH-1; j++){ //for each sub col
                    BOARD[k][j] = BOARD[k-1][j]; //move it down
                }
            }
            for (int j = 1; j < BOARD_WIDTH-1; j++){ //set the top row to all zeros
                BOARD[0][j] = 0; //make it 0
            }
            
            draw_board(); //redraw the new board

            //another full row could have moved down so ++i to recheck row in case
            ++i; //++
        }
    }

    // if any rows were cleared, update the score
    if (rows_cleared_num > 0) {
        SCORE += rows_cleared_num * 10;
        //mvprintw(BOARD_HEIGHT+2, 2, "Score: %d", SCORE);
    }


}

bool check_game_over(){ //show game over screen
    
    int ch; //useless character var 

    clear(); //clear screen
    printw("GAME OVER.\n"); //cout
    printw("YOUR SCORE WAS: %d\n", SCORE); //cout
    printw("\n[To close press any key]\n"); //cout
    ch = getch(); //get
    mvprintw(3, 0, "[To confirm again to close press any key]\n"); //cout
    ch = getch(); //get
    mvprintw(3, 0, "[To FOR SURE confirm again to close press any key]\n"); //cout
    ch = getch(); //get
    return true;  //exit
}

#endif //guard 

