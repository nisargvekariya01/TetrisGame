#ifndef TETRAMINO_H
#define TETRAMINO_H

#include <bits/stdc++.h>
#include <windows.h>
#include "Block.h"
using namespace std;

map<char, vector<pair<int, int>>> tetraminos;
map<string, pair<int, int>> movement;
vector<pair<int, int>> permanent_coordinates;
char choose[] = {'T','O','J','L','I','S','Z'};
string colour[] = {PURPLE, YELLOW, PINK, ORANGE, BLUE, RED, GREEN};

void make() {

    tetraminos['T'] = {{0,0},{0,-1},{0,1},{-1,0}};
    tetraminos['O'] = {{0,0},{-1,0},{0,1},{-1,1}};
    tetraminos['J'] = {{0,0},{0,-1},{-1,0},{-2,0}};
    tetraminos['L'] = {{0,0},{0,1},{-1,0},{-2,0}};
    tetraminos['I'] = {{0,0},{1,0},{-1,0},{-2,0}};
    tetraminos['S'] = {{0,0},{0,-1},{-1,0},{-1,1}};
    tetraminos['Z'] = {{0,0},{0,1},{-1,0},{-1,-1}};

    movement["LEFT"] = {0, -1};
    movement["RIGHT"] = {0, 1};
    movement["DOWN"] = {1, 0};
}

void operator+=(pair<int,int> &p1, pair<int, int> &p2) {
    p1.first += p2.first;
    p1.second += p2.second;
}

void operator-=(pair<int,int> &p1, pair<int, int> &p2) {
    p1.first -= p2.first;
    p1.second -= p2.second;
}

bool operator!=(pair<int, int> p1, pair<int, int> p2) {
    return p1.first != p2.first || p1.second != p2.second;
}

bool landed = false;

class Tetramino {
    
    private:

        int tetramino;
        pair<int, int> pivot = {2 , 2 + rand() % 7};
        pair<int, int> pivot_next_piece = {2, 2};
        vector<pair<int, int>> coordinates;
        vector<pair<int, int>> prev_coordinates;
        pair<int, int> center;
    
    public:
        Tetramino(int tetramino) {

            this -> tetramino = tetramino;
            coordinates = tetraminos[choose[tetramino]];
            for(auto &i : coordinates) {
                i += pivot;
            }
            prev_coordinates = coordinates;
            center = pivot;
        }
    
        bool game_over(vector<vector<Block>> &board){
    
            for(auto i : coordinates){
    
                if(board[i.first][i.second].get_colour() != GRAY)
                    return true;
            }
    
            return false;
        }
    
        void form(vector<vector<Block>> &board) {
            for(auto i : coordinates) {
                board[i.first][i.second].set_colour(colour[tetramino]);
            }
        }
    
        bool can_move(string move, vector<vector<Block>> &board) {
            vector<pair<int, int>> new_coords = coordinates;
            for(auto &i : new_coords) {
                i += movement[move];
                if(i.first >= 20 || i.second < 0 || i.second >= 10) {
                    return false;
                }
                for(auto &p : permanent_coordinates) {
                    if(i.first == p.first && i.second == p.second) {
                        return false;
                    }
                }
            }
            return true;
        }
    
        void update(string move, vector<vector<Block>> &board) {

            prev_coordinates = coordinates;
            for(auto &i : coordinates) {
                board[i.first][i.second].set_colour(GRAY);
            }
    
            if(can_move(move, board)) {
                for(auto &i : coordinates) {
                    i += movement[move];
                }
                
                if(move == "DOWN")
                    center.first += 1;
    
                else if(move == "LEFT")
                    center.second -= 1;
    
                else
                    center.second += 1;
            } 
            
            else if(move == "DOWN") {
                landed = true;
                for(auto &i : coordinates) {
                    permanent_coordinates.push_back(i);
                }
                form(board);
            } 
            
            else {
                coordinates = prev_coordinates;
                form(board);
            }
        }
    
        void rotate(vector<vector<Block>> &board) {
            prev_coordinates = coordinates;  // Save current state for rollback
            
            // Clear current position
            for(auto i : coordinates) {
                board[i.first][i.second].set_colour(GRAY);
            }
        
            // Try rotating each coordinate
            vector<pair<int, int>> new_coords = coordinates;
            bool valid_rotation = true;
            
            for(auto &i : new_coords) {
                // Translate to origin (relative to center)
                i -= center;
                
                // Perform 90-degree clockwise rotation: (x,y) -> (-y,x)
                int temp = i.first;
                i.first = -i.second;
                i.second = temp;
                
                // Translate back
                i += center;
                
                // Check boundaries and collisions
                if(i.first >= 20 || i.first < 0 || i.second >= 10 || i.second < 0) {
                    valid_rotation = false;
                    break;
                }
                
                // Check collision with permanent blocks
                for(auto &p : permanent_coordinates) {
                    if(i == p) {
                        valid_rotation = false;
                        break;
                    }
                }
            }
            
            // If rotation is valid, apply it; otherwise rollback
            if(valid_rotation) {
                coordinates = new_coords;
            } else {
                coordinates = prev_coordinates;
            }
            
            // Update board with final position
            form(board);
        }
    
        void draw(HANDLE hConsole, vector<vector<Block>> &board) {
            COORD pos;
            
            for(auto i : prev_coordinates) {
                pos.X = i.second * 3 + 1;
                pos.Y = i.first * 2 + 1;
                SetConsoleCursorPosition(hConsole, pos);
                cout << GRAY << "  " << RESET << BLACK << "|" << RESET;
            }
            for(auto i : coordinates) {
                pos.X = i.second * 3 + 1;
                pos.Y = i.first * 2 + 1;
                SetConsoleCursorPosition(hConsole, pos);
                cout << colour[tetramino] << "  " << RESET << BLACK << "|" << RESET;
            }
        }

        void draw_next_piece(HANDLE hConsole, vector<vector<Block>> &next_piece, int tetramino_index){

            for(int i = 0; i < 4; i++){

                for(int j = 0; j < 4; j++){

                    next_piece[i][j].set_colour(BLACK);
                }
            }

            vector<pair<int, int>> coords = tetraminos[choose[tetramino_index]];

            for(auto &i : coords){

                i += pivot_next_piece;
                next_piece[i.first][i.second].set_colour(colour[tetramino_index]);
            }

            COORD pos;
            pos.X = 49;
            pos.Y = 20;
            SetConsoleCursorPosition(hConsole, pos);
            cout << "\033[1;33m---NEXT PIECE---\033[0m";
            
            pos.X = 50;
            for(int i = 0; i < 4; i++) {
                
                pos.Y = 21 + (i * 2);  
                SetConsoleCursorPosition(hConsole, pos);
                
                cout << " "; 
                for(int j = 0; j < 4; j++) {
                    cout << "-- ";
                }
                
                pos.Y = 22 + (i * 2);  
                SetConsoleCursorPosition(hConsole, pos);
                
                cout << BLACK << "|";
                for(int j = 0; j < 4; j++) {
                    next_piece[i][j].display();
                }
            }
            
        }
    
        vector<pair<int, int>> get_coordinates() {
            return coordinates;
        }
};

#endif