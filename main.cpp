#include <iostream>
#include "../include/Board.h"
#include <list>
#include <queue>
#include <set>
#include <algorithm>
#include <vector>
#include <random>
#include <map>
#include <pthread.h>


using namespace std;
//typedef list<Board> l_boards;
typedef priority_queue<Board, vector<Board>, greater<Board> > pq_boards;
typedef set<Board, bool(*)(const Board&,const Board&) > set_boards;
typedef multiset<Board, greater<Board>> mset_boards;
typedef map<Board,Board> map_boards;
typedef vector<Board> l_boards;
bool piecewise_less(const Board& l, const Board& r)
{
    const short *lb = l.getBoard();
    const short *rb = r.getBoard();
    for (int i = 0; i < 9; ++i)
    {
        if (lb[i] < rb[i])
            return true;
        else if (rb[i] < lb[i])
            return false;
    }
    return false;
}

bool helper(Board& l, Board& r)
{
    return l > r;
}

void printSolution(Board& goal, map_boards& m)
{
    while (m.find(goal) != m.end())
    {
        cout << goal << endl;
        goal = m.at(goal);
    }
    cout << goal << endl;
}


int main()
{

    vector<short> v({1,2,3,4,5,6,7,8,0});
    Board goal(v.begin(), v.end());
    vector<Board> open;
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);

    Board b(v.begin(), v.end());
    set_boards closed(piecewise_less);
    map_boards parent;
    cout << "Solving :" << endl << b << endl;
    vector<Board> moves;
    b.setG(0);
    open.push_back(b);
    push_heap(open.begin(), open.end(), helper);
    while (! open.empty())
    {
        Board x = Board(open.front());
        pop_heap(open.begin(), open.end(), helper); open.pop_back();
        if (x == goal)
        {
            cout << "ITS WORKZ" << endl;
            cout << x.getG() << " steps." << endl;
            return 0;
        }
        moves.clear();
        x.getMoves(moves);
        l_boards::iterator whereOpen;
        set_boards::iterator whereClosed;
        for (Board b : moves)
        {
            //Board b(*it);
            if ((whereOpen = find(open.begin(), open.end(), b)) != open.end())
            {
                if (whereOpen->getCost() <= b.getCost())
                {
                    continue;
                }
            }
            if ((whereClosed = closed.find(b)) != closed.end())
            {
                if (whereOpen->getCost() <= b.getCost())
                    continue;
            }
            if (whereClosed != closed.end())
                closed.erase(whereClosed);
            if (whereOpen != open.end())
                open.erase(whereOpen);
            //parent.insert(make_pair(b, x));
            b.setParent(&x);
            open.push_back(b);
        }
        make_heap(open.begin(), open.end(), helper);
        closed.insert(x);
    }
}


