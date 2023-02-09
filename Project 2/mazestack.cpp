//
//  main.cpp
//  mazestack
//
//  Created by Charles Qin on 2/4/23.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord
      {
        public:
          Coord(int r, int c) : m_row(r), m_col(c) {}
          int r() const { return m_row; }
          int c() const { return m_col; }
        private:
          int m_row;
          int m_col;
      };


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    
    stack<Coord> coordsStack;
    Coord a(sr, sc);
    coordsStack.push(a);
    maze[sr][sc] = '*';
    
    while(!coordsStack.empty()){
        Coord e = coordsStack.top();
        cerr << e.r() << ", " << e.c() << endl;
        coordsStack.pop();
        maze[e.r()][e.c()] = '*';

        if(e.r() == er && e.c() == ec) return true;
        
        if(maze[e.r()][e.c() + 1] == '.'){
            Coord x(e.r(), e.c() + 1);
            coordsStack.push(x);
            maze[e.r()][e.c() + 1] = '*';
        }
        
        if(maze[e.r() - 1][e.c()] == '.'){
            Coord x(e.r() - 1, e.c());
            coordsStack.push(x);
            maze[e.r() - 1][e.c()] = '*';
        }

        if(maze[e.r()][e.c() - 1] == '.'){
            Coord x(e.r(), e.c() - 1);
            coordsStack.push(x);
            maze[e.r()][e.c() - 1] = '*';
        }

        if(maze[e.r() + 1][e.c()] == '.'){
            Coord x(e.r() + 1, e.c());
            coordsStack.push(x);
            maze[e.r() + 1][e.c()] = '*';

        }
        
    }
    
    return false;
}

          // Return true if there is a path from (sr,sc) to (er,ec)
          // through the maze; return false otherwise


//Push the starting coordinate (sr,sc) onto the coordinate stack and
//    update maze[sr][sc] to indicate that the algorithm has encountered
//    it (i.e., set maze[sr][sc] to have a value other than '.').
//While the stack is not empty,
//{   Pop the top coordinate off the stack. This gives you the current
//        (r,c) location that your algorithm is exploring.
//    If the current (r,c) coordinate is equal to the ending coordinate,
//        then we've solved the maze so return true!
//    Check each place you can move from the current cell as follows:
//        If you can move EAST and haven't encountered that cell yet,
//            then push the coordinate (r,c+1) onto the stack and update
//            maze[r][c+1] to indicate the algorithm has encountered it.
//        If you can move NORTH and haven't encountered that cell yet,
//            then push the coordinate (r-1,c) onto the stack and update
//            maze[r-1][c] to indicate the algorithm has encountered it.
//        If you can move WEST and haven't encountered that cell yet,
//            then push the coordinate (r,c-1) onto the stack and update
//            maze[r][c-1] to indicate the algorithm has encountered it.
//        If you can move SOUTH and haven't encountered that cell yet,
//            then push the coordinate (r+1,c) onto the stack and update
//            maze[r+1][c] to indicate the algorithm has encountered it.
//}
//There was no solution, so return false


int main()
        {
            string maze[10] = {
                "XXXXXXXXXX",
                "X..X...X.X",
                "X.XXXX.X.X",
                "X.X.X..X.X",
                "X...X.XX.X",
                "XXX......X",
                "X.X.XXXX.X",
                "X.XXX....X",
                "X...X..X.X",
                "XXXXXXXXXX"
            };
        
            if (pathExists(maze, 10,10, 5,3, 8,8))
                cout << "Solvable!" << endl;
            else
                cout << "Out of luck!" << endl;
        }
