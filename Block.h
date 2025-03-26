#ifndef BLOCK_H
#define BLOCK_H

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

#define YELLOW "\033[43m"  // Yellow background
#define BLUE "\033[44m"    // Blue background
#define RED "\033[41m"     // Red background
#define GREEN "\033[42m"   // Green background
#define ORANGE "\033[48;5;214m" // Orange
#define PINK "\033[48;5;213m"    // Pink
#define PURPLE "\033[45m"  // Purple background
#define BLACK "\033[40m"   // Black background
#define RESET "\033[0m"
#define GRAY "\033[48;5;15m"

class Block {
    private:
        string colour;
    
    public:
        Block(string colour) : colour(colour) {}
    
        void display() {
            cout << colour << "  " << RESET << "|" << RESET;
        }
    
        void set_colour(string colour) {
            this->colour = colour;
        }
    
        string get_colour() {
            return colour;
        }
};

#endif