#include <bits/stdc++.h>
#include "Gameboard.h"
#include <windows.h>
using namespace std;

int main() {

    system("cls");
    vector<vector<Block>> board(20, vector<Block>(10, Block(GRAY)));
    vector<vector<Block>> next_piece(4, vector<Block>(4, Block(GRAY)));
    make();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));

    GameBoard Board;
    int score = 0;
    int current_tetramino, next_tetramino, level_decider = 1, level = 0, total_lines_cleared = 0;
    next_tetramino = rand() % 7;

    // Draw initial grid
    Board.display(board);

    DWORD last_move_time = GetTickCount();
    DWORD last_down_time = GetTickCount();
    const DWORD move_delay = 100;
    DWORD down_delay = 500;
    DWORD soft_drop_delay = 100;
    DWORD prev_down_delay;

    while(true) {

        current_tetramino = next_tetramino;
        next_tetramino = rand() % 7;

        COORD pos_tetris;
        pos_tetris.X = 40;
        pos_tetris.Y = 5;
        SetConsoleCursorPosition(hConsole, pos_tetris);

        cout << "\033[1;33m TTTTT  EEEEE  TTTTT  RRRRR   III  SSSSS ";
        pos_tetris.Y += 1;
        SetConsoleCursorPosition(hConsole, pos_tetris);
        cout << "   T    E        T    R   R    I   S     ";
        pos_tetris.Y += 1;
        SetConsoleCursorPosition(hConsole, pos_tetris);
        cout << "   T    EEEE     T    RRRRR    I   SSSSS ";
        pos_tetris.Y += 1;
        SetConsoleCursorPosition(hConsole, pos_tetris);
        cout << "   T    E        T    R  R     I       S  ";
        pos_tetris.Y += 1;
        SetConsoleCursorPosition(hConsole, pos_tetris);
        cout << "   T    EEEEE    T    R   R   III  SSSSS";

        pos_tetris.Y += 5;
        pos_tetris.X += 12;
        SetConsoleCursorPosition(hConsole, pos_tetris);
        cout << "Level : " << level << "\033[0m";

        Tetramino tetramino(current_tetramino);
        if(tetramino.game_over(board)){

            COORD pos;
            pos.X = 1;
            pos.Y = 20 * 2;
            SetConsoleCursorPosition(hConsole, pos);
            cout << RED << "GAME OVER" << RESET << endl;
            cout << RED << "Score : " << score << RESET << endl;
            return 0;
        }

        tetramino.form(board);
        tetramino.draw_next_piece(hConsole, next_piece, next_tetramino);
        tetramino.draw(hConsole, board);
        
        bool space_pressed = false;

        while(!landed) {
            DWORD current_time = GetTickCount();

            if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
                if(current_time - last_move_time >= move_delay) {
                    tetramino.update("LEFT", board);
                    tetramino.draw(hConsole, board);
                    last_move_time = current_time;
                }
            }

            if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                if(current_time - last_move_time >= move_delay) {
                    tetramino.update("RIGHT", board);
                    tetramino.draw(hConsole, board);
                    last_move_time = current_time;
                }
            }

            if(!space_pressed && (GetAsyncKeyState(VK_SPACE) & 0x8000)) {

                prev_down_delay = down_delay;
                down_delay = 0;
                space_pressed = true;
            }

            if(GetAsyncKeyState(VK_UP) & 0x8000) {
                if(current_time - last_move_time >= move_delay) {
                    tetramino.rotate(board);
                    tetramino.draw(hConsole, board);
                    last_move_time = current_time;
                }
            }

            if(current_time - last_down_time >= soft_drop_delay && GetAsyncKeyState(VK_DOWN) & 0x8000){
                tetramino.update("DOWN", board);
                tetramino.draw(hConsole, board);
                last_down_time = current_time;
            }

            if(current_time - last_down_time >= down_delay) {
                tetramino.update("DOWN", board);
                tetramino.draw(hConsole, board);
                last_down_time = current_time;
            }

            if(GetAsyncKeyState(VK_ESCAPE)) {
                COORD pos = {1, 40};
                SetConsoleCursorPosition(hConsole, pos);
                return 0;
            }

            set<int> affected_rows;
            for(auto i : tetramino.get_coordinates()) {
                if(i.first >= 0 && i.first < 20) affected_rows.insert(i.first);
            }
            for(int row : affected_rows) {
                Board.redraw_underscores(hConsole, row, 10);
            }

            Sleep(10);
        }

        tetramino.draw(hConsole, board);

        // Remove filled lines and redraw
        int lines = Board.remove_filled_lines(board);
        if(lines > 0) {
            score += lines * 100;
            cout << "\a\a\a\a";
            system("cls");
            Board.display(board);  // Full redraw after line removal

            COORD pos = {50, 38};
            SetConsoleCursorPosition(hConsole, pos);
            cout << "\033[1;33mScore: " << score << "\033[0m";
        }

        total_lines_cleared += lines;

        landed = false;
        if(space_pressed){

            down_delay = prev_down_delay;
        }

        if(total_lines_cleared >= level_decider){

            level += 1;
            level_decider += (level * 2);
            down_delay -= 100;
            soft_drop_delay -= 70;
        }

        while(GetAsyncKeyState(VK_SPACE) & 0x8000) {
            Sleep(10);   
        }
    }

    return 0;
}
