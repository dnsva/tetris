#ifndef BOARD_FNS_H
#define BOARD_FNS_H

#include <iostream>
#include "classes.h"
#include <ncurses.h>
#include <vector>
#include <utility>

using namespace std;


void init_board(){ //initialize all the bottom row to act like blocks (for collision)

    for(int i = 1; i<BOARD_WIDTH-1; ++i){
        BOARD[BOARD_HEIGHT-1][i] = 1;
    }

    //make edges off-limit so -1
    for(int i = 1; i<BOARD_WIDTH-1; ++i){
        BOARD[0][i] = -1;
    }
    for(int i = 1; i<BOARD_HEIGHT-1; ++i){
        BOARD[i][0] = -1;
    }
    for(int i = 1; i<BOARD_HEIGHT-1; ++i){
        BOARD[i][BOARD_WIDTH-1] = -1;
    }

}

void clear_board(){
    //Reset EVERYTHING 
}

void draw_board(){
    //DRAW BORDERS & COLORR
    clear(); //clear everything before just in case 
    for (int i = 0; i < BOARD_HEIGHT; i++) {

        int j2 = 0;

        for (int j = 0; j < BOARD_WIDTH; j++) {
                
            

            int color = BOARD[i][j];
            if (color > 0 && i != BOARD_HEIGHT-1) {
                attron(COLOR_PAIR(color));
                
                //char const* u = "\u25A1";
            
                //mvprintw(i,j, "%s", u);

                char const* u = "█";
                mvprintw(i,j2, "%s%s", u ,u);

                //mvaddch(i,j,(char)219);

                //wchar_t mychar = L'\u25A1';
                // print the character using wprintw
                //mvprintw(i,j, "%lc\n", mychar);
                //wprintw(stdscr, "%lc\n", mychar);
                //mvprintw(i, j, "□");
                attroff(COLOR_PAIR(color));
            }else{
               // attrset(A_NORMAL); //reset everything just in case 
                mvprintw(i, j2, "  ");
            }

            if (i == 0 || i == BOARD_HEIGHT-1 || j == 0 || j == BOARD_WIDTH-1) {
                mvprintw(i, j2, "##");
            }

            j2 += 2;

            
        }
    }
}

void add_block(block B){ //add block to board
    for(pair<int,int>p : B.coords){
        BOARD[p.first][p.second] = B.type;
    }
}

void clear_rows(){ //clear full rows

    int rows_cleared_num = 0;
    for (int i = BOARD_HEIGHT-2; i > 0; --i){
        // check if the row is full
        bool is_full = true;
        for (int j = 1; j < BOARD_WIDTH-1; j++) {
            if (BOARD[i][j] == 0) {
                is_full = false;
                break;
            }
        }
        // if the row is full, clear it and move all the blocks above it down one row
        if (is_full) {
            rows_cleared_num++;
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < BOARD_WIDTH-1; j++) {
                BOARD[k][j] = BOARD[k-1][j];
                }
            }
            // set the top row to all zeros
            for (int j = 1; j < BOARD_WIDTH-1; j++) {
                BOARD[0][j] = 0;
            }
            // redraw the board
            draw_board();
        }
    }
    // if any rows were cleared, update the score
    if (rows_cleared_num > 0) {
        SCORE += rows_cleared_num * 10;
        mvprintw(BOARD_HEIGHT+2, 2, "Score: %d", SCORE);
    }


}

bool check_game_over(){
    //check if the first row has blocks
    for (int j = 1; j < BOARD_WIDTH-1; j++) {
        if (BOARD[1][j] == 1) {
            return true;
        }
    }
    return false;
}

#endif