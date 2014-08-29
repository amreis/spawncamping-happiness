#include "../include/Board.h"
const Board Board::GOAL_BOARD = Board(new short[9]{1,2,3,4,5,6,7,8,0});
Board::Board(const short b[9])
{
    for (int i = 0; i < 9; ++i)
    {
        this->board[i] = b[i];
        if (b[i] == 0) whiteIndex = i;
    }
    this->cost = -1;
    this->g = 0;
    this->eval_board();
}

Board::Board(const Board& other)
    : g(other.getG()), parent(other.getParent())
{
    const short *b = other.getBoard();
    for (int i = 0; i < 9; ++i)
    {
        this->board[i] = b[i];
        if (b[i] == 0) this->whiteIndex = i;
    }
    this->cost = -1;
    this->cost = this->eval_board();
}

Board::Board(Board* const other)
    : g(other->getG()), parent(other->getParent())
{
    const short* b = other->getBoard();
    for (int i = 0; i < 9; ++i)
    {
        this->board[i] = b[i];
        if (b[i] == 0) whiteIndex = i;
    }
    this->cost = -1;
    this->cost = this->eval_board();
}

Board::Board(vector<short>::iterator first, vector<short>::iterator last)
{
    int i = 0;
    for (vector<short>::iterator it = first; it != last; ++it)
        this->board[i++] = *it;
    this->cost = -1;
    this->eval_board();
}

const short* Board::getBoard() const
{
    return this->board;
}



void Board::moveWhiteTo(int index)
{
    if (! (index >= 0 && index <= 8) )
        std::cerr << "Error in moveWhiteTo. Index: " << index << std::endl;
    if (index == whiteIndex + 1)
    {
        if (whiteIndex % 3 != 2)
        {
            std::swap(board[index], board[whiteIndex]);
        }
    }
    else if (index == whiteIndex - 1)
    {
        if (whiteIndex % 3 != 0)
            std::swap(board[index], board[whiteIndex]);
    }
    else if (index == whiteIndex - 3)
    {
        if (whiteIndex / 3 != 0)
            std::swap(board[index], board[whiteIndex]);
    }
    else if (index == whiteIndex + 3)
    {
        if (whiteIndex / 3 != 2)
            std::swap(board[index], board[whiteIndex]);
    }
    whiteIndex = index;
    this->cost = -1.0f;
    this->cost = this->eval_board();
}

void Board::getMoves(vector<Board>& out)
{
    int whiteRow = whiteIndex/3, whiteCol = whiteIndex%3;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = ((i != 0) ? 0 : -1); j <= ((i != 0) ? 0 : 1); j += 2)
        {
            int access = (whiteRow + i)*3 + (whiteCol + j);
            if (access < 0 || access > 8) continue;
            Board n(*this);
            n.moveWhiteTo(access);
            out.push_back(n);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
    const short* board = b.getBoard();
    os << "---------" << std::endl << board[0] << '\t' << board[1] << '\t' << board[2] << std::endl;
    os << board[3] << '\t' << board[4] << '\t' << board[5] << std::endl;
    os << board[6] << '\t' << board[7] << '\t' << board[8] << std::endl;
    os << "---------";
    return os;
}

Board& Board::operator=(const Board& other)
{
    const short *b = other.getBoard();
    for (int i = 0; i < 9; ++i)
    {
        this->board[i] = b[i];
        if (b[i] == 0) whiteIndex = i;
    }
    this->cost = -1;
    this->cost = this->eval_board();
    this->g = other.getG();
    this->parent = other.getParent();
    return (*this);
}

Board& Board::operator=(Board* const other)
{
    const short *b = other->getBoard();
    for (int i = 0; i < 9; ++i)
    {
        this->board[i] = b[i];
        if (b[i] == 0) whiteIndex = i;
    }
    this->cost = -1;
    this->cost = this->eval_board();
    this->g = other->getG();
    this->parent = other->getParent();
    return (*this);
}

float Board::eval_board()
{
    if (this->cost != -1.0f) return this->cost;
    // float cost = (*this) - GOAL_BOARD;
    float cost = 0;
    const short *b = board;
    for (int i = 0; i < 9; ++i)
    {
        short p = b[i];
        int row = i/3, col = i%3;
        p = p - 1;
        if (p < 0) p += 9;
        int mRow = p/3, mCol = p%3;
        // Manhattan Distance between tile position and where it should be.
        cost += abs(row-mRow) + abs(col-mCol);
    }
    /*
    const short* b = this->getBoard();
    for (int i = 0; i < 9; ++i)
    {
        int row = i/3, col = i%3;
        int mRow = b[i]/3, mCol = b[i]%3;
        float fact = 1;
        if (row == 1)
            fact = 0.2;
        cost += fact*(abs(row-mRow) + abs(col-mCol));
    }
    cost /= 9;
    */

    cost += this->g;
    this->cost = cost;

    return cost;
}
/*
bool operator<(const Board& me, const Board& other)
{
    return me.eval_board() < other.eval_board();
}*/

bool Board::operator<(Board& other)
{
    return this->eval_board() < other.eval_board();
}

bool Board::operator>(Board& other)
{
    return this->eval_board() > other.eval_board();
}

float Board::getCost() const
{ return this->cost; }

bool Board::operator!=(const Board& other) const
{
    const short* b = other.getBoard();
    for (int i = 0; i < 9; ++i)
    {
        if (this->board[i] != b[i])
            return true;
    }
    return false;
}

bool Board::operator==(const Board& other) const
{
    return ! (this->operator!=(other));
}

bool Board::operator<(const Board& other) const
{
    const short *lb = this->getBoard();
    const short *rb = other.getBoard();
    for (int i = 0; i < 9; ++i)
    {
        if (lb[i] < rb[i])
            return true;
        else if (rb[i] < lb[i])
            return false;
    }
    return false;
}

float Board::operator-(const Board& other) const
{
    const short *l = this->getBoard();
    const short *r = other.getBoard();
    float diff = 0;
    for (int i = 0; i < 9; ++i)
        diff += ((l[i] != r[i]) ? 1 : 0);
    return diff;
}

float Board::getG() const
{
     return this->g;
}

Board* Board::getParent() const
{ return this->parent; }
