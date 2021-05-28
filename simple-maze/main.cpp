#include <iostream>
#include "BTree.h"
#include <bits/stdc++.h>

using namespace std;

namespace dimensions
{
    size_t n, m;
} 

class Vertex
{
public:
    int i, j; 
    int weight;
    bool visited = false;
    size_t shortest_path = INT_MAX;
    Vertex* prev = NULL;
    size_t num;

    Vertex () = default;

    Vertex (size_t i, size_t j)
        :i(i), j(j)
    {}
    
    Vertex (size_t i, size_t j, size_t w)
        :i(i), j(j), weight(w)
    {
        this->num = i*dimensions::m + j;
    } 

    friend ostream& 
    operator<< (ostream& os, const Vertex& obj)
        { return os << '(' << obj.i << ',' << obj.j
                    << ',' << obj.weight << ')'; }
    
    bool 
    operator== (const Vertex& ver) const
        { return ver.i == this->i && ver.j == this->j; }
};

class VerDistPair
{
typedef Vertex* VerPtr;
public:
    VerPtr ver;
    size_t dist;

    VerDistPair () = default;

    VerDistPair (VerPtr ver)
        :ver(ver), dist (ver->shortest_path)
    {}

    VerDistPair (VerPtr ver,const size_t& dist)
        : ver(ver), dist (dist)
    {}

    friend ostream& 
    operator<< (ostream& os, const VerDistPair& obj)
        { return os << '(' << obj.dist << ',' << obj.ver->num
                    << ',' << obj.ver->weight << ')'; }

    bool
    operator== (const VerDistPair& other) const
        { return this->dist == other.dist 
            && this->ver ==  other.ver; }

    bool 
    operator> (const VerDistPair& other) const
    {
        if (this->dist == other.dist)
            return this->ver->num > other.ver->num;
        return this->dist > other.dist;
    }

    bool
    operator< (const VerDistPair& other) const
    {
        if (this->dist == other.dist)   
            return this->ver->num < other.ver->num;
        return this->dist < other.dist;
    }

    bool
    operator<= (const VerDistPair& other) const
        { return *this < other || *this == other; }

    bool
    operator>= (const VerDistPair& other) const
        { return *this > other || *this == other; }
};

class MazeSolver
{
typedef Vertex* VerPtr;
private:
    const size_t n, m;
    VerPtr** maze = NULL;
    BTree<VerDistPair> tree = BTree<VerDistPair>(3);
public:
    MazeSolver ()
        :n(dimensions::n), m(dimensions::m)
    {}

    void
    readMaze ()
    {
        maze = new VerPtr*[n];
        for (size_t i = 0; i < n; i++)
            maze[i] = new VerPtr[m];
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < m; j++)
            {
                size_t curr_w;
                cin >> curr_w;
                maze[i][j] = new Vertex(i, j, curr_w);
            }
    }

    void 
    solveMaze ()
    {
        source = curr_ver = getSourceVer ();
        dest = getDestVer();
        source->shortest_path = source->weight;
        tree.insert(VerDistPair(source));
        solveHelper();
    }

private:
    VerPtr curr_ver, source, dest;

    void 
    solveHelper()
    {
        size_t dist_so_far;
        do 
        {
            curr_ver = tree.popMinData().ver;
            dist_so_far = curr_ver->shortest_path;
            curr_ver->visited = true;
            auto curr_ver_i = curr_ver->i;
            auto curr_ver_j = curr_ver->j;
            if (isValid(curr_ver_i, curr_ver_j + 1))
                updateIfPossible (maze[curr_ver_i][curr_ver_j + 1], dist_so_far);
            if (isValid(curr_ver_i, curr_ver_j - 1))
                updateIfPossible(maze[curr_ver_i][curr_ver_j - 1], dist_so_far);
            if (isValid(curr_ver_i + 1, curr_ver_j))
                updateIfPossible(maze[curr_ver_i + 1][curr_ver_j], dist_so_far);
            if (isValid(curr_ver_i - 1, curr_ver_j))
                updateIfPossible(maze[curr_ver_i - 1][curr_ver_j], dist_so_far);
            
        } while (!tree.isEmpty());
    }

    void 
    updateIfPossible (VerPtr updating_ver, size_t dist_so_far)
    {
        if (!updating_ver->visited) 
        {
            if (updating_ver->shortest_path == INT_MAX)
            {
                updating_ver->shortest_path = updating_ver->weight + dist_so_far; 
                updating_ver->prev = curr_ver;
                tree.insert(VerDistPair(updating_ver));
            }
            else if (updating_ver->shortest_path > dist_so_far + updating_ver->weight)
            {
                updating_ver->shortest_path = updating_ver->weight + dist_so_far; 
                updating_ver->prev = curr_ver;
                tree.deleteData(VerDistPair(updating_ver));
                tree.insert(VerDistPair(updating_ver, dist_so_far));
            }
        }
    }

    bool 
    isValid (size_t i, size_t j)
    { 
        return 0 <= i && i < n && 0 <= j && j < m
            && maze[i][j]->weight; 
    }

    VerPtr 
    getSourceVer ()
    {
        for (int j = 0; j < m; j++)
            if (maze[0][j]->weight) 
                return maze[0][j];
    }

    VerPtr 
    getDestVer ()
    {
        for (int j = 0; j < m; j++)
            if (maze[n - 1][j]->weight) 
                return maze[n - 1][j];
    }

    void
    applyFoundPath ()
    {
        // cout << "heree" << endl;
        auto iter_ver = dest;
        while (iter_ver && iter_ver != source)
        {
            iter_ver->weight = -1;
            iter_ver = iter_ver->prev;
        }
        if (iter_ver)
            iter_ver->weight = -1;
    }

public:

    void 
    printMaze()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                cout << *maze[i][j] << ' ';
            cout << endl;
        }
    }

    void 
    printResult ()
    {
        applyFoundPath();
        for (int i = 0; i < n; i++)
        {
            cout << setw(3);
            for (int j = 0; j < m; j++)
                cout << maze[i][j]->weight << setw(3) ;
            cout << '\n';
        }
    }
};


int main ()
{
    cin >> dimensions::n >> dimensions::m ;
    MazeSolver solver;
    solver.readMaze();
    solver.solveMaze();
    cout << "result:\n\n";
    solver.printResult();
    return 0;
}


