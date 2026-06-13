/* name - Anna Denisova
   date - April 23
   title - TETRIS
   desc - Tetris game playable in terminal
*/

#include <iostream>  //output
#include <ncurses.h> //keys
#include <vector>    //vector lists
#include <utility>   //pair
#include <stdlib.h>  //standard library
#include <time.h>    //for random

#include "classes.h" //structs
#include "board_functions.h" //functions for board
#include "score.h" //files 

using namespace std; //less typing 

//FUNCTIONS ------------------
bool menu();                //
void ascii();               //
bool loop();                //
void choose_difficulty();   //
//----------------------------

int main(){	

	//INITIALIZE NCURSES:
	setlocale(LC_ALL, ""); //enable utf-8
    initscr();             //initialize screen
    cbreak();              //other ncurses initialization
    noecho();              //other ncurses initialization
    curs_set(0);           //set cursor pos
    keypad(stdscr, TRUE);  //set to standard screen output
    start_color();         //enable color 

    srand(time(NULL)); //for random num

	ascii(); //ascii art

	clear(); //clear the screen

    // define the color pairs for each block type
	init_pair(1, COLOR_WHITE, COLOR_BLACK);   //1 = white
    init_pair(2, COLOR_RED, COLOR_BLACK);     //2 = red
    init_pair(3, COLOR_GREEN,COLOR_BLACK);    //3 = green
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  //4 = yellow
    init_pair(5, COLOR_BLUE, COLOR_BLACK);    //5 = blue
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); //6 = magenta
    init_pair(7, COLOR_CYAN, COLOR_BLACK);    //7 = cyan
	init_color(100, 500, 0, 500);             //defines a purple color
	init_pair(8, 100, COLOR_BLACK);           //8 = purple

	refresh(); //send everything to screen

	bool quit = menu(); //quitting returns 1
	while(!quit){       //while not quitted
		quit = menu();  //menu again
	}

	endwin(); //end ncurses mode
	return 0; //exit
}

//==========================================

bool menu(){ //menu, return true if we want to quit

	int choice = 0; //menu choice 
	string options[] = {"PLAY", "HOW TO", "QUIT"}; //options for menu 
	int c; //temp char var
	int confirm; //temp confirm var

	clear(); //clear screen

	printw("WHAT DO YOU WANT TO DO? (up/down to move, enter key to select, enter key again to confirm choice)\n"); //ask
	
	while(true){ //loop 
        for (int i = 0; i < 3; i++){ //print list of options
            if (i == choice){ //if i selected
                attron(A_STANDOUT);// highlight the chosen option
            }
            mvprintw(i+1,0,"%d. %s\n", i+1, options[i].c_str()); //print option
            attroff(A_STANDOUT); //unhighlight 
        }
        c = getch(); //get key
        if (c == KEY_DOWN){ //if down arrow
            choice = (choice + 1) % 3; //manip pos
        }else if (c == KEY_UP){ //if up arrow
            choice = (choice + 2) % 3; //manip pos 
        }
        else if (c == 10){ //enter key
            attron(A_BOLD); //bold
            attron(A_STANDOUT); //highlight
            mvprintw(choice+1, 0, "%d. %s?\n", choice+1, options[choice].c_str()); //confirmation
            confirm = getch(); //get again
            if(confirm == 10){ //if enter again
                break; //get out of loop
            }else{ //if decided to not confirm 
                attroff(A_BOLD); //no bold
                attroff(A_STANDOUT); //no highlight 
                if (confirm == KEY_DOWN){ //if going down 
                    choice = (choice + 1) % 3; //manip pos
                }else if (confirm == KEY_UP){ //if going up
                    choice = (choice + 3) % 3; //manip pos 
                }
            }
        }
    }
	attrset(A_NORMAL); //reset any highlighted stuff

	if(choice == 0){ //PLAY
		choose_difficulty(); //call fn
		clear_board();       //clear everything before start
		while(1){ //loop
			bool is_done = loop(); //check if done 
			if(is_done){ //if done 
				break; //break out of loop
			}
		}
		if(SCORE > read_score(DIFFICULTY)){ //if high score was beat
			write_score(SCORE, DIFFICULTY); //update 
		}

	}else if(choice == 1){ //RULES

		clear(); //clear screen
	
		/* the following code prints the rules */
		mvprintw(1, 0, "HOW TO PLAY:\n");
		mvprintw(2, 0, "TRY TO GET AS MANY POINTS AS POSSIBLE.\n");
		mvprintw(3, 0, "CONTROLS FOR TETRIS BLOCKS:\n");
		mvprintw(4, 0, "     ---                 ---            \n");  
		mvprintw(5, 0, "    | W |               | ^ |           \n");
		mvprintw(6, 0, "     ---       OR        ---            \n");
		mvprintw(7, 0, " --- --- ---         --- --- ---        \n");
		mvprintw(8, 0, "| A | S | D |       | < | v | > |       \n");
		mvprintw(9, 0, " --- --- ---         --- --- ---        \n");
		printw("\nVERY IMPORTANT - IF YOU ARE UNABLE TO ROTATE, IT IS BECAUSE \nROTATING THE BLOCK MAKES IT GO OUT OF BOUNDS. THIS MEANS YOU \nMIGHT HAVE TO MOVE IT A SPACE LEFT OR RIGHT BEFORE ROTATING.\n\nNote: high scores are stored directly onto your computer\n");
		printw("\nYou can close your current game at any time by pressing 'q'\n\n");
		printw("[PRESS ANY KEY TO CLOSE THIS WINDOW]\n");

		//for exiting:
		int ch = getch();

	}else{ //QUIT
		return true; //exit
	}

	return false; //exit 

}

//==========================================

bool loop(){ //main game loop, returns true when game is done
	
	block ACTIVE_PIECE; //curr active piece
	int ch; //temp thing for input 

	clear_rows(); //clear rows if anything is full 

	ACTIVE_PIECE.generate_block(); //generate type 

	if(!check_valid_pos(ACTIVE_PIECE.coords)){ //spawn blocked
		check_game_over(); //show game over
		return true; //game is over
	}
	
	add_block(ACTIVE_PIECE); //add it to the board
	draw_board(); //display the board

	while(1){ //loop 

		//see if collision happened 
		//if yes make piece to 1s so as if it does not exist
		if(ACTIVE_PIECE.check_collision()){ //check
			add_block(ACTIVE_PIECE); //rewrite this pos with 1s 
			return false; //this turn is OVER 
			
		}

		draw_board(); //draw board 
		
        ch = getch(); //check for user input 
        switch (ch){ //switch on it 
            case KEY_LEFT: //if left arrow pressed 
                ACTIVE_PIECE.move_LEFT(); //call move 
                break;  //break
            case KEY_RIGHT: //if right arrow pressed 
                ACTIVE_PIECE.move_RIGHT(); //call move 
                break; //break
            case KEY_DOWN: //if down arrow pressed 
                ACTIVE_PIECE.move_DOWN(); //call move 
                break; //break
            case KEY_UP: //if up arrow pressed 
                ACTIVE_PIECE.rotate_block(); //call move 
                break; //break
			case 'a': //left alternative
				ACTIVE_PIECE.move_LEFT(); //call move 
                break; //break
			case 'd': //right alternative
				ACTIVE_PIECE.move_RIGHT(); //call move 
                break; //break
			case 's': //down alternative
				ACTIVE_PIECE.move_DOWN(); //call move 
                break; //break
			case 'w': //rotate alternative
				ACTIVE_PIECE.rotate_block(); //call move 
                break; //break
            case 'q': //if q pressed
                return true; //exit to menu
		}
	}
}

//==========================================

void choose_difficulty(){ //select difficulty menu 

	int choice = 0; //menu choice
	string options[] = {"EASY", "MEDIUM", "HARD", "IMPOSSIBLE"}; //options
	int c; //temp char storage
	int confirm; //temp confirm var 

	clear(); //clear screen 
	printw("CHOOSE DIFFICULTY\n"); //output 
	
	while(true){
        for (int i = 0; i < 4; i++){ //print list of options
            if (i == choice){ //if we are on the current choice 
                attron(A_STANDOUT);// highlight the chosen option
            }
            mvprintw(i+1,0,"%d. %s\n", i+1, options[i].c_str()); //print
			if(i == 1){ //if medium 
				mvprintw(i+1,0,"%d. %s\n", i+1, "MEDIUM (recommended)"); //mention that its recommended
			}
            attroff(A_STANDOUT); //highlight
        }
        c = getch(); //get key input 
        if (c == KEY_DOWN){ //if down arrow
            choice = (choice + 1) % 4; //manip choice
        }
        else if (c == KEY_UP){ //if up arrow
            choice = (choice + 3) % 4; //manip choice 
        }
        else if (c == 10){ //if enter key 
            attron(A_BOLD); //bold it
            attron(A_STANDOUT); //highlight it 
            mvprintw(choice+1, 0, "%d. %s?\n", choice+1, options[choice].c_str()); //confirmation
            confirm = getch(); //get input 
            if(confirm == 10){ //if enter 2
                break; //get out
            }else{
                attroff(A_BOLD); //disable bold
                attroff(A_STANDOUT); //disable highlight 
                if (confirm == KEY_DOWN){ //if down arrow
                    choice = (choice + 1) % 4; //manip choice 
                }
                else if (confirm == KEY_UP){ //if up arrow
                    choice = (choice + 3) % 4; //manip choice 
                }
            }
        }
    }

	attrset(A_NORMAL); //reset any highlighted stuff

	DIFFICULTY = choice;   //global var difficulty set 
	if(choice == 0){       //EAST
		BOARD_WIDTH = 20;  //set w
		BOARD_HEIGHT = 30; //set h
	}else if(choice == 1){ //MED
		BOARD_WIDTH = 10;  //set w
		BOARD_HEIGHT = 20; //set h
	}else if(choice == 2){ //HARD
		BOARD_WIDTH = 7;   //set w
		BOARD_HEIGHT = 15; //set h
	}else if(choice == 3){ //IMPOSSIBLE
		BOARD_WIDTH = 6;   //set w
		BOARD_HEIGHT = 12; //set h
	}
}

//==========================================

void ascii(){ //prints ascii   

	/* The following lines print ascii art */
	printw("               __.....__                      .--.     \n");     
	printw("           .-''         '.                    |__|       \n");   
	printw("     .|   /     .-''\"'-.  \`.      .|  .-,.--. .--.       \n");   
	printw("   .' |_ /     /________\   \   .' |_ |  .-. ||  |        \n");  
	printw(" .'     ||                  | .'     || |  | ||  |     _    \n");
	printw("'--.  .-'\\    .-------------''--.  .-'| |  | ||  |   .' |   \n");
	printw("   |  |   \\    '-.____...---.   |  |  | |  '- |  |  .   | / \n");
	printw("   |  |    \`.             .'    |  |  | |     |__|.'.'| |/\/ \n");
	printw("   |  '.'    \`''-...... -'      |  '.'| |       .'.'.-'  /  \n");
	printw("   |   /                        |   / |_|       .'   \\_.'   \n");
	printw("   \`'-'                         \`'-'                        \n");
	printw("* MacOS or Linux recommended but not required *\n* Can be played with a RP2040 microcontroller using button keys, ask me (Anna) if you want to try * \n* Have fun! * \n\n[To continue press any key]\n\n\nNote: This is the macos/linux version\nCopyright (c) 2023 Anna Denisova");
	//printw("[To continue press any key]\n");
	
                                                          



	int c = getch(); //to end 
}

//==========================================


