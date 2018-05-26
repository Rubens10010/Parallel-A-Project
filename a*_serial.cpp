/**
* @file a*_serial.cpp
* @author Ruben Edwin Huala Quispe
* @date 05/21/2018
* @version 1.0
*
* @brief CSUNSA 2018, Final project of Parallel Algorithms subject
* 
* @section DESCRIPTION
* This is a simple implementation of the A* serial algorithm
* made using pure c++, for solving the n-puzzle problem {n -> 2,3,4},
* the program is written in a functional style to avoid future issues
* on parallel implementations.
*
* Command line arguments are not needed, the only function to call is
* run() which will create a new puzzle and solve, the output for this
* function is the total wall time required to solve the puzzle.
*
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <stdlib.h>
#include <list>
#include <bitset>
#include <time.h>

typedef int tileValue;
typedef std::pair<size_t, size_t> tile;
typedef std::vector<tileValue> row;
typedef std::vector<tileValue> planeBoard;
typedef std::vector<row> board;

size_t n;       /// <Size of (n)-puzzle game
board puzzle;   /// <Sliding-tile puzzle with n**2 tiles 
tile blankTile; /// <Position of blank tile on board

/// Functions used on this program, see each one for description.
void initialize_puzzle();
void print_puzzle();
std::list<tile> neighbours(tile &pTile);
short neighbours_m(tile &pTile);
void moveBlankTile(tile &nTile);
void moveBlankTile(short side_bit);

/**
* @brief Main function and entry function for the application
* @param argc is the number of arguments, just 1 is required. (2 total)
* @param argv contains the name of the program and the paramenter 'n'
* @return 0 for success otherwise -1
*/
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Not enough parameters given. Size of puzzle missing (2,3,4)." << std::endl;
        return -1;
    }
    srand(time(0));
    
    n = atoi(argv[1]);
    initialize_puzzle();
    print_puzzle();
    
    /*std::list<tile> ve = neighbours(blankTile);
    for(std::list<tile>::iterator it = ve.begin(); it != ve.end(); ++it)
    {
        std::cout << "Neighbour for blank: " << it->first << " " << it->second << std::endl;
    }*/
    short nn = neighbours_m(blankTile);
    std::bitset<8> b(nn); 
    std::cout << "Neighbours:  " << b << std::endl;
    //tile nTile = std::make_pair(blankTile.first - 1, blankTile.second);
    moveBlankTile(1);
    print_puzzle();
    moveBlankTile(2);
    print_puzzle();
    moveBlankTile(4);
    print_puzzle();
    moveBlankTile(8);
    print_puzzle();
    moveBlankTile(0);
    print_puzzle();
    return 0;
}


/**
* @brief Initializes a new board for a n-puzzle game
* @param None, global: size of puzzle board(n).
* @return None, side effect: initialize n-puzzle-board randomly
*/
inline void initialize_puzzle()
{
    try{
        // Initialize puzzle board with default value set to 0
        puzzle.resize(n, row(n, 0));
        // Add the n**2 values for each puzzle tile
        planeBoard puzzleValues(n*n);
        std::iota(puzzleValues.begin(), puzzleValues.end(), 0);
        std::random_shuffle(puzzleValues.begin(), puzzleValues.end());
        for(size_t i = 0; i < n; i++)
        {
            for(size_t j = 0; j < n; j++)
            {
                puzzle[i][j] = puzzleValues[i*n + j];
                if(puzzleValues[i*n + j] == 0)
                    blankTile = std::make_pair(i,j);
            }
        }
    }
    catch(...){
        std::cout << "Size of puzzle wasn't defined yet, you idiot!";
        exit(EXIT_FAILURE);
    }    
}


/**
* @brief Prints a puzzle board on console
* @param None, global: size of puzzle board(n).
* @return None, side effect: initialize n-puzzle-board randomly
*/
inline void print_puzzle()
{
    std::cout << "Puzzle: [\n";
    for(size_t i = 0; i < n; i++)
    {
        std::cout << " ";
        for(size_t j = 0; j < puzzle[i].size(); j++)
        {
            if(blankTile.first == i && blankTile.second == j)
                std::cout << "(0) ";
            else
                std::cout << puzzle[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << " ]\n";
}

/**
* @brief Calculates the list of neighbours tiles to a given tile. 
* @param pTile, is the query tile.
* @return std::list<tile>, are tiles which can be visited from pTile
*                           on list order {left, right, top, down}
*/
std::list<tile> neighbours(tile &pTile)
{
    size_t left_x, right_x, left_y, right_y;
    try{
        left_y = pTile.first - 1;
        right_y = pTile.first + 1;
        left_x = pTile.second - 1;
        right_x = pTile.second + 1;
    }
    catch(...){
        std::cout << "Surely a tile is not initialized!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    tile neighbour;
    std::list<tile> tileNeighbours;
    tile impossible = std::make_pair(n+1,n+1);
    if(left_x > 0)      //! left neighbour
    {
        neighbour = std::make_pair(pTile.first,left_x);
        tileNeighbours.push_back(neighbour);
    }
    else
    {
        tileNeighbours.push_back(impossible);
    }
    
    if(right_x < n)     //! right neighbour
    {
        neighbour = std::make_pair(pTile.first,right_x);
        tileNeighbours.push_back(neighbour);
    }
    else
    {
        tileNeighbours.push_back(impossible);
    }
    if(left_y > 0)     //! top neighbour
    {
        neighbour = std::make_pair(left_y,pTile.second);
        tileNeighbours.push_back(neighbour);
    }
    else
    {
        tileNeighbours.push_back(impossible);
    }
    if(right_y < n)      //! bottom neighbour
    {
        neighbour = std::make_pair(right_y, pTile.second);
        tileNeighbours.push_back(neighbour);        
    }
    else
    {
        tileNeighbours.push_back(impossible);
    }
    return tileNeighbours;
}

/**
* @brief Calculates the list of neighbours tiles to a given tile. 
* @param pTile, is the query tile.
* @return mask, is the bit position for tiles which can be 
*                   visited from pTile on list order {left, right, *                                                        top, down}
*/
short neighbours_m(tile &pTile)
{
    /// neighbour mask (i.e) 1001 where 1 means pTile can visit the \\
            tile neighbour at that position [y][x]
    short mask = 0; // 0 0 0 0
    
    int left_x, right_x, left_y, right_y;
    try{
        left_y = pTile.first - 1;
        right_y = pTile.first + 1;
        left_x = pTile.second - 1;
        right_x = pTile.second + 1;
    }
    catch(...){
        std::cout << "Surely a tile is not initialized!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    if(right_y < n)
        mask = mask | 1;
        //mask += 1;  // 0 0 0 1
    if(left_y >= 0)
        mask = mask | 2;
        //mask += 2;  // 0 0 1 0
    if(right_x < n)
        mask = mask | 4;
        //mask += 4;  // 0 1 0 0
    if(left_x >= 0)
        mask = mask | 8;
        //mask += 8;  // 1 0 0 0  
    return mask;
}

void moveBlankTile(tile &nTile)
{
    std::swap(puzzle[blankTile.first][blankTile.second], puzzle[nTile.first][nTile.second]);
    blankTile = nTile;
}

void moveBlankTile(short side_bit)
{
    try{
        size_t x = blankTile.second;
        size_t y = blankTile.first;
        switch(side_bit)
        {
            case 1:     //! move tile down
                std::swap(puzzle[y][x], puzzle[y + 1][x]);
                blankTile = std::make_pair(y+1,x);
                break;
            case 2:     //! move tile up
                std::swap(puzzle[y][x], puzzle[y - 1][x]);
                blankTile = std::make_pair(y-1,x);
                break;
            case 4:     //! move tile forward
                std::swap(puzzle[y][x], puzzle[y][x + 1]);
                blankTile = std::make_pair(y,x+1);
                std::cout << "moved to " << y << " " << x+1 <<" "<<puzzle[y][x]<< std::endl;
                break;
            case 8:     //! move tile backward
                std::swap(puzzle[y][x], puzzle[y][x - 1]);
                blankTile = std::make_pair(y,x-1);
                break;
            case 0:     //! no side where to move
                break;
            default:    //! error!
                std::cout << "Cant move tile, something went wrong" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    catch(...)
    {
        std::cout << "Thing is... I cannot move to given position, faggot!";
        exit(EXIT_FAILURE);    
    }
}
