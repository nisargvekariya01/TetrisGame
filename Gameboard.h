#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <bits/stdc++.h>
#include "Tetramino.h"
#include "Block.h"
#include <windows.h>
using namespace std;

class GameBoard {

    public:
        void display(vector<vector<Block>> board) {
            for(int i = 0; i < board.size(); i++) {
                cout << " ";
                for(int j = 0; j < board[0].size(); j++) {
                    cout << "-- ";
                }
                cout << endl;
    
                cout << BLACK << "|";
                for(int j = 0; j < board[0].size(); j++) {
                    board[i][j].display();
                }
                cout << endl;
            }
        }
    
        void redraw_underscores(HANDLE hConsole, int row, int cols) {
            COORD pos;
            pos.Y = row * 2;
            pos.X = 1;
            SetConsoleCursorPosition(hConsole, pos);
            for(int j = 0; j < cols; j++) {
                cout << BLACK << "-- ";
            }
        }
    
        int remove_filled_lines(vector<vector<Block>>& board) {
            int lines_removed = 0;
            vector<int> filled_rows;
    
            // Detect filled rows
            for(int i = 19; i >= 0; i--) {
                bool is_filled = true;
                for(int j = 0; j < 10; j++) {
                    if(board[i][j].get_colour() == GRAY) {
                        is_filled = false;
                        break;
                    }
                }
                if(is_filled) {
                    filled_rows.push_back(i);
                    lines_removed++;
                }
            }
    
            if(lines_removed == 0) return 0;
    
            // Update permanent_coordinates
            vector<pair<int, int>> new_permanent;
            for(auto& coord : permanent_coordinates) {
                bool keep = true;
                for(int row : filled_rows) {
                    if(coord.first == row) {
                        keep = false;
                        break;
                    }
                }
                if(keep) {
                    int shift = 0;
                    for(int row : filled_rows) {
                        if(coord.first < row) shift++;
                    }
                    new_permanent.push_back({coord.first + shift, coord.second});
                }
            }
            permanent_coordinates = new_permanent;
    
            // Update board state
            for(int i = 19; i >= 0; i--) {
                bool is_filled = false;
                for(int row : filled_rows) {
                    if(i == row) {
                        is_filled = true;
                        break;
                    }
                }
                
                if(!is_filled) {
                    int new_row = i;
                    for(int row : filled_rows) {
                        if(i < row) new_row++;
                    }
                    if(new_row != i) {
                        for(int j = 0; j < 10; j++) {
                            board[new_row][j].set_colour(board[i][j].get_colour());
                        }
                    }
                }
            }
    
            // Clear top rows
            for(int i = 0; i < lines_removed; i++) {
                for(int j = 0; j < 10; j++) {
                    board[i][j].set_colour(GRAY);
                }
            }
    
            return lines_removed;
        }
};

#endif