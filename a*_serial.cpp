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
#include <set>
#include <queue>
#include <algorithm>
#include <numeric>
#include <utility>
#include <stdlib.h>
#include <list>
#include <bitset>
#include <math.h>
#include <time.h>

typedef unsigned short tileValue;
typedef std::pair<size_t, size_t> tile;
typedef std::vector<tileValue> row;
typedef std::vector<tileValue> planeBoard;
typedef std::vector<row> board;

size_t n;       /// <Size of (n)-puzzle game
board puzzle;   /// <Sliding-tile puzzle with n**2 tiles 
tile blankTile; /// <Position of blank tile on board

///< Goal state puzzle
board goal_puzzle;

/**
* @Brief This struct is a node for the state-space graph
*/
struct Node
{
    board grid; /// <Contains the current state of the puzzle
    int h;  /// <heuristic function value
    int g;  /// <goal distance function (tree height position)
    short move;
    tile blankTile;
    Node* parent;
    
    /*bool operator<(const Node& other) const
    {
        return (this.grid == other.grid);
    }*/
    
    bool operator==(const Node& other) const
    {
        return (this.grid == other.grid);
    }
};

/// Functions used on this program, see each one for description.
void initialize_puzzle();
void print_puzzle();
std::list<tile> neighbours(tile &pTile);
short neighbours_m(tile &pTile);
std::vector<Node> generate_neightbours(struct Node &current, short nn);
void moveBlankTile(struct Node &p, short side_bit);
void moveBlankTile(short side_bit);
size_t h1(board &current);
bool isSolvable(planeBoard &p);
bool compare(struct Node &n1, struct Node &n2);
std::list<short> solve(board &n_puzzle);

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
    size_t distance = h1(puzzle);
    std::cout << "Manhattan distance: " << distance << std::endl;
    /*std::list<tile> ve = neighbours(blankTile);
    for(std::list<tile>::iterator it = ve.begin(); it != ve.end(); ++it)
    {
        std::cout << "Neighbour for blank: " << it->first << " " << it->second << std::endl;
    }*/
    
    /*short nn = neighbours_m(blankTile);
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
    print_puzzle();*/
    
    solve(puzzle);
    return 0;
}

/**
* @brief Solves a n-puzzle game using A* search algorithm
* @param n_puzzle, references to a nxn grid board of sliding tiles
* @return list_moves, which is the list of moves to solve the puzzle
*           on the shortest path 
*/
std::list<short> solve(board &n_puzzle)
{
    std::list<short> lstMoves;
    struct Node root;
    root.grid = puzzle; ///< copy initial state
    root.g = 0;     ///< cost of going from start to start is zero
    root.h = h1(n_puzzle);      ///< first node completely heuristic
    root.parent = NULL;
    root.blankTile = blankTile;
    /// <Priority queue of Nodes used as openSet
    std::priority_queue<Node, std::vector<Node>, decltype(&compare)> openSet(compare);
    /// <Set of Nodes used as closedSet
    std::vector<Node> closedSet;
    
    openSet.push(root);
    Node current;
    //tile currBlankTile = blankTile;
    int i;
    while(!openSet.empty())
    {
        current = openSet.top();
        if(current.g + current.h == 0)    ///< Reconstruct path
            return lstMoves;
        
        openSet.pop(); ///< Caution!
        closedSet.push_back(current);
        /// <First iteartion blankTile == 
        short nn = neighbours_m(current.blankTile);
        std::cout << nn << std::endl;
        std::bitset<4> b(nn); 
        std::cout << "Neighbours:  " << b << std::endl;
        std::vector<Node> neighbours = generate_neightbours(current, nn);
        /// <For each neighbour of current
        for(i = 0; i < neighbours.size(); i++)
        {
            //if(std::find());
        }
    }
    return lstMoves;
}

bool compare(struct Node &n1, struct Node &n2)
{
    return (n1.h + n1.g) > (n2.h + n2.g);
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
        goal_puzzle.resize(n, row(n, 0));
        planeBoard puzzleValues(n*n);
        std::iota(puzzleValues.begin(), puzzleValues.end(), 0);
        std::random_shuffle(puzzleValues.begin(), puzzleValues.end());
        
        while(!isSolvable(puzzleValues))
        {
            std::iota(puzzleValues.begin(), puzzleValues.end(), 0);
            std::random_shuffle(puzzleValues.begin(), puzzleValues.end());
            for(int i = 0; i < n*n; i++)
                std::cout << " " << puzzleValues[i];
            std::cout << std::endl;
        }
        
        // Add the n**2 values for each puzzle tile
        for(size_t i = 0; i < n; i++)
        {
            for(size_t j = 0; j < n; j++)
            {
                puzzle[i][j] = puzzleValues[i*n + j];
                goal_puzzle[i][j] = (i*n + j) + 1;
                if(puzzleValues[i*n + j] == 0)
                    blankTile = std::make_pair(i,j);
            }
        }
        
        goal_puzzle[n-1][n-1] = 0;
    }
    catch(...){
        std::cout << "Size of puzzle wasn't defined yet, you idiot!";
        exit(EXIT_FAILURE);
    }    
}

/**
* @brief Checks wether or nor a puzzle configuration is solvable
* @param p is a board for a given n puzzle
* @return solvable, true if it is or not otherwise.
*/
inline bool isSolvable(planeBoard &p)
{
    bool solvable = false;
    tileValue curr;
    std::set<tileValue> order;
    std::set<tileValue>::iterator it;
    std::pair<std::set<tileValue>::iterator,bool> ret;
    short n_inversions = 0;
    short blankRow;
    for(size_t i = 0; i < p.size(); i++)
    {
        curr = p[i];
        if(curr == 0)
        {
            blankRow = i;
            continue;
        }
        ret = order.insert(curr);
        if (ret.second == false)    // element is repeated
        {
            std::cout << "Solvability check failed!" << std::endl;
            exit(EXIT_FAILURE);
        }
        else{   // successfully inserted
            it = ret.first;
            n_inversions += std::distance(it, order.end()) - 1;
            // __if DEBUG__
            //std::cout << "Curr pos: [" << i << "] n_inv: " << std::distance(it, order.end()) -1<< std::endl;
        }
    }
    std::cout << "n_inversiones: " << n_inversions << std::endl;
    if(n%2 != 0)    // odd
    {
        return (n_inversions%2 == 0) ? true:false;
    }
    else    // even
    {
        return ((n_inversions + blankRow) % 2 != 0) ? true:false;
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

/**
* @brief Expands a current node into its childs given a mask of moves.
* @param current, a node of a given state from a puzzle.
* @param nn, is the mask of possible moves for the blank tile
* @return vector of expanded nodes from current, on list order      
*                           {left,right,top,down}
*/
std::vector<Node> generate_neightbours(struct Node &current, short nn)
{
    std::vector<Node> neighboursLst;
    struct Node neighbour;
    if(nn & 1 == 1) // move down
    {
        neighbour = current;    ///< get a cpy of current on neighbour
        moveBlankTile(neighbour, 1);
        neighboursLst.push_back(neighbour);
    }
    if(nn & 2 == 2) // move down
    {
        neighbour = current;
        moveBlankTile(neighbour, 2);
        neighboursLst.push_back(neighbour);
    }
    if(nn & 4 == 4) // move down
    {
        neighbour = current;
        moveBlankTile(neighbour, 4);
        neighboursLst.push_back(neighbour);
    }
    if(nn & 8 == 8) // move down
    {
        neighbour = current;
        moveBlankTile(neighbour, 8);
        neighboursLst.push_back(neighbour);
    }
    return neighboursLst;
}

void moveBlankTile(struct Node &p, short side_bit)
{   
    try{
        size_t x = p.blankTile.second;
        size_t y = p.blankTile.first;
        switch(side_bit)
        {
            case 1:     //! move tile down
                std::swap(p.grid[y][x], p.grid[y + 1][x]);
                p.blankTile = std::make_pair(y+1,x);
                p.move = 1;
                break;
            case 2:     //! move tile up
                std::swap(p.grid[y][x], p.grid[y - 1][x]);
                p.blankTile = std::make_pair(y-1,x);
                p.move = 2;
                break;
            case 4:     //! move tile forward
                std::swap(p.grid[y][x], p.grid[y][x + 1]);
                p.blankTile = std::make_pair(y,x+1);
                p.move = 4;
                std::cout << "moved to " << y << " " << x+1 <<" "<<puzzle[y][x]<< std::endl;
                break;
            case 8:     //! move tile backward
                std::swap(p.grid[y][x], p.grid[y][x - 1]);
                p.blankTile = std::make_pair(y,x-1);
                p.move = 8;
                break;
            case 0:     //! no side where to move
                p.move = 0;
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

/**
* @brief Calculates the manhattan distance between current and goal. 
* @param current, is the current board configuration of puzzle.
* @return m_distance, is the manhattan distance of the heuristic.
*               this is the summation of the grid distances between
*               each tile in current grid to goal grid.
*/
size_t h1(board &current)
{
    size_t m_distance = 0;
    tileValue val;
    // Add the n**2 values for each puzzle tile
    for(short i = 0; i < n; i++)
    {
        for(short j = 0; j < n; j++)
        {
            val = current[i][j];
            // If tile is not in correct position
            if(val != goal_puzzle[i][j] && val != 0)
            {
                // goal position for this val
                float v = (val - 1)/n;
                //std::cout << "real i: " << v << std::endl;
                short dx = round(i - v);
                v = (val - 1)%n;
                //std::cout << "real j: " << v << std::endl;
                short dy = round(j - v);
                //std::cout << "dx "<<dx << " dy " << dy << std::endl; 
                m_distance += abs(dx) + abs(dy);
                // if __DEBUG__
                //std::cout << "From this i:"<<i << " j:"<<j<<" val: " << val << " to goal " << val << " = " << abs(dx) + abs(dy) << std::endl;
            }
        }
    }

    return m_distance;
}
