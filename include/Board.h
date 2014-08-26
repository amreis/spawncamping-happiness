#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <iostream>
#include <vector>

using std::list;
using std::vector;

class Board
{

    public:
        static const Board GOAL_BOARD;
        Board(const short b[9]);
        Board(Board* const other);
        Board(const Board& other);
        Board(vector<short>::iterator first, vector<short>::iterator last);
        void moveWhiteTo(int index);
        void getMoves(vector<Board>& out);
        float getCost() const;
        const short* getBoard() const;
        friend std::ostream& operator<<(std::ostream&, const Board&);
        Board& operator=(const Board& other);
        Board& operator=(Board* const other);
        //friend bool operator<(const Board& me, const Board& other);
        bool operator<(Board& other);
        bool operator>(Board&);
        bool operator!=(const Board&) const;
        bool operator==(const Board&) const;
        bool operator<(const Board& other) const;
        float operator-(const Board& other) const;
        void setG(float newG) {this->g = newG;};
        float getG() const;
        void setParent(Board* parent) {
            this->parent = parent;
            this->g = parent->getG() + 1;
        };
        Board* getParent() const;
    protected:
        short board[9];
        float cost = -1;
        int whiteIndex;
    private:
        float eval_board();
        float g = 0;
        Board* parent = NULL;
};

#endif // BOARD_H
