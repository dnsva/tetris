#ifndef CLASSES_H //guard
#define CLASSES_H //guard

#include <iostream> //cout
#include <vector>   //vector
#include <utility>  //pair

using namespace std; //less typing

int BOARD_WIDTH = 10;  //width of board
int BOARD_HEIGHT = 20; //height of board 

int DIFFICULTY = 0; //0 - easy, 1 - med, 2 - hard, 3 - impossible

int SCORE = 0; //score
int BOARD[100][100] = {}; //THE BOARD

//FUNCTIONS
bool check_valid_pos(vector<pair<int,int>>curr_coords, vector<pair<int,int>>old_coords = {}); //checks if valid
void update_block_pos(vector<pair<int,int>>coords, int type); //updates block pos 

struct block{ //class
    vector<pair<int,int>>coords; //its coordinates
    int type = 0; //its type (color too)

    void generate_block(){ //randomly generate a block 
        int c = BOARD_WIDTH/2; //col num (will store center)
        int selection = rand()%7 + 1; //1-7 (there are 7 types of pieces )
        switch(selection){ //make the block 
            case 1: //I BLOCK
                this->coords = { //set coords 
                    {1,c-2},
                    {1,c-1},
                    {1,c},
                    {1,c+1}
                };
                this->type = 2; //set type
                break;          //break
            case 2: //J BLOCK
                this->coords = { //set coords 
                    {1,c},
                    {2,c},
                    {3,c-1},
                    {3,c}
                };
                this->type = 3; //set type
                break;          //break
            case 3: //L BLOCK
                this->coords = { //set coords 
                    {1,c},
                    {2,c},
                    {3,c},
                    {3,c+1}
                };
                this->type = 4; //set type
                break;          //break
            case 4: //Z BLOCK
                this->coords = { //set coords 
                    {1,c-1},
                    {1,c},
                    {2,c},
                    {2,c+1}
                };
                this->type = 5; //set type
                break;          //break
            case 5: //S BLOCK
                this->coords = { //set coords 
                    {1,c},
                    {1,c+1},
                    {2,c-1},
                    {2,c}
                };
                this->type = 6; //set type
                break;          //break
            case 6: //T BLOCK
                this->coords = { //set coords 
                    {1,c-1},
                    {1,c},
                    {1,c+1},
                    {2,c}
                };
                this->type = 7; //set type
                break;          //break
            case 7: //O BLOCK
                this->coords = { //set coords 
                    {1,c-1},
                    {1,c},
                    {2,c-1},
                    {2,c}
                };
                this->type = 8; //set type
                break;          //break
        }
    }
    
    void rotate_block(){ //rotate block
        vector<pair<int,int>>new_coords(coords); //make a copy 

        int cx = (new_coords[0].first + new_coords[1].first + new_coords[2].first + new_coords[3].first) / 4;     //find center
        int cy = (new_coords[0].second + new_coords[1].second + new_coords[2].second + new_coords[3].second) / 4; //find center 

        // Rotate each point of the block around the center
        for (int i = 0; i < 4; i++) { //loop
            int tmp_x = new_coords[i].first; //temp var
            int tmp_y = new_coords[i].second; //temp var
            new_coords[i].first = cx + (tmp_y - cy); //calculate
            new_coords[i].second = cy - (tmp_x - cx); //calculate 

        }
        if(check_valid_pos(new_coords, coords)){ //if valid 
            coords = {new_coords}; //make the real coords these
            update_block_pos(coords, type); //update the blocks position in main board array
        }
    }

    bool check_collision(){ //check if collision with board 
        vector<pair<int,int>>new_coords(coords); //make copy 
        for(int i = 0; i<new_coords.size(); ++i){ //for each coord
            new_coords[i].first++; //add to rows
        }
        if(!check_valid_pos(new_coords, coords)){ //if cannot move down
            this->type = 1; //set type to 1
            return true; //return true 
        }
        return false; //otherwise if no collision
    }

    void move_DOWN(){ //move block down 
        vector<pair<int,int>>new_coords(coords); //make copy 
        for(int i = 0; i<new_coords.size(); ++i){ //for each coord
            new_coords[i].first++; //add to rows
        }
        if(check_valid_pos(new_coords, coords)){ //check if valid 
            coords = {new_coords}; //change 
            update_block_pos(coords, type); //add
        }
    }

    void move_LEFT(){ //move block left
        vector<pair<int,int>>new_coords(coords); //make copy 
        for(int i = 0; i<new_coords.size(); ++i){ //for each coordinate
            new_coords[i].second--; //sub from cold
        }
        if(check_valid_pos(new_coords, coords)){ //check if valid
            coords = {new_coords}; //change 
            update_block_pos(coords, type); //add
        }
    }

    void move_RIGHT(){ //move block right 
        vector<pair<int,int>>new_coords(coords); //make copy 
        for(int i = 0; i<new_coords.size(); ++i){ //for each coordinate
            new_coords[i].second++; //add to cols 
        }
        if(check_valid_pos(new_coords, coords)){ //check if valid
            coords = {new_coords}; //change
            update_block_pos(coords, type); //add
        }
    }

};

bool check_valid_pos(vector<pair<int,int>>curr_coords, vector<pair<int,int>>old_coords){ //check if B is in valid position on board
    for(pair<int,int>pos:curr_coords){ //for each coordinate 
        if(pos.first <= -1 || pos.second <= -1){ //if neg coords 
            return false; //bad
        }
        if(pos.first < 1 || pos.first > BOARD_HEIGHT-2){ //if not in existing rows
            return false; //bad
        }
        if(pos.second < 1 || pos.second > BOARD_WIDTH-2){ //if not in existing cols
            return false; //bad 
        }
        if(BOARD[pos.first][pos.second] != 0){ //if occupied
            bool is_old = false; //part of the moving piece
            for(pair<int,int>old:old_coords){ //for each old coord
                if(old.first == pos.first && old.second == pos.second){ //if same cell
                    is_old = true; //ignore it
                    break; //break
                }
            }
            if(!is_old){ //if blocked by another block
                return false; //bad
            }
        }
    }
    return true; //if passed everything 
} 

void update_block_pos(vector<pair<int,int>>coords, int type){ //update pos of some block on board 

    int piece_found = 0; //max 4
    bool all_found = false; //if 4 found 

    //remove current ones (THERE IS ONLY ONE COLORED BLOCK ON THE BOARD AT A TIME, THE REST ARE WHITE)
    for(int i = 0; i<BOARD_HEIGHT; ++i){ //rows
        for(int j = 0; j<BOARD_WIDTH; ++j){ //cols
            if(BOARD[i][j] > 1){ //if color
                piece_found++; //means piece was found 
                BOARD[i][j] = 0; //so now set it to nothing
            } 
            if(piece_found == 4){ //if all 4 pieces of block were found
                all_found = true; //set this to true
                break; //break 
            }
        }
        if(all_found) break; //break
    }

    //add new ones
    for(pair<int,int>p:coords){ //for each coordinate
        BOARD[p.first][p.second] = type; //move it to new pos 
    }
}

#endif //guard