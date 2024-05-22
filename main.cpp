#include <bits/stdc++.h>

#include <utility>
using namespace std;

#ifndef ONLINE_JUDGE
#include "Debug.cpp"
#define FileI(fileName) freopen(fileName, "r", stdin);
#define FileO(fileName) freopen(fileName, "w", stdout);
#else
#define FileI(fileName)
#define FileO(fileName)
#define debug(...) 1
#define Time(i, x...) x
#endif
#define un unsigned
#define allc(x) begin(x), end(x)

template<typename T>
using HashedBoard = vector<pair<T, T>>;

string output, expected;
const string Dir[] = {"left", "up", "right", "down"};

struct Graph
{
    enum Direction {NONE = -1, LEFT, UP, RIGHT, DOWN};
    enum Algorithm {S_LOG_S, N_SQUARED, S_PLUS_N};
    struct Node
    {
        HashedBoard<un short> ballBoardState;
        Direction parentDir = NONE;
        int sourceDistance = 0;
        Node* Parent = nullptr;
        Node() = default;
        Node(const HashedBoard<un short>& ballBoardState, int sourceDistance = 0, Node* Parent = nullptr, Direction parentDir = NONE)
        {
            this->ballBoardState = ballBoardState;
            this->sourceDistance = sourceDistance;
            this->Parent = Parent;
            this->parentDir = parentDir;
        }
    };

    unsigned moveTimer = 0; // counter for how many times we call move method
    Node* initialState, *finalState = nullptr;
    vector<vector<char>> globalBoard; // grid without balls
    vector<deque<un short>> ballsRows, ballsCols;
    vector<vector<pair<un short, un short>>> obstaclesRowPos, obstaclesColPos; // used for constant queries about the nearest obstacle in 4 main directions
    vector<pair<un short, un>> ballsRowPos; // dynamically updating the nearest ball (above or below) me for constant queries
    pair<un short, un short> target; // coordinates of the target
    const short toBase = UCHAR_MAX + 1; // toBase = 256
    vector<vector<string>> baseChar; // constant queries for ContainerHashing function
    Algorithm choosenAlgo; // algorithm to apply on vertical moves
    vector<vector<Direction>> adj = {{UP, DOWN}, {LEFT, RIGHT}, {UP, DOWN}, {LEFT, RIGHT}}; // for every direction what are the next directions

    Graph(const vector<vector<char>>& board, const pair<un short, un short>& target) // O(max(slogs, n^2))
    {
        HashedBoard<un short> slidersPos;
        int n = board.size() - 2;

        baseChar = vector<vector<string>>(n + 2, vector<string>(n + 2));
        for (int i = 0; i <= n + 1; i++)
            for (int j = 0; j <= n + 1; j++)
                ConvFromDec(i, baseChar[i][j]), ConvFromDec(j, baseChar[i][j]);

        globalBoard = board;
        ballsRows = ballsCols = vector<deque<un short>>(n + 2);
        ballsRowPos = vector<pair<un short, un>>(n + 2, {-1, moveTimer});
        obstaclesRowPos = obstaclesColPos = vector<vector<pair<un short, un short>>>(n + 2, vector<pair<un short, un short>>(n + 2));
        for (int i = 0; i <= n + 1; i++)
            for (int lj = 0, rj = n + 1, j = 0, jj = n + 1; j <= n + 1; j++, jj--)
            {
                if (board[i][j] != '#')
                    obstaclesColPos[i][j].first = lj;
                else
                    lj = j;
                if (board[i][jj] != '#')
                    obstaclesColPos[i][jj].second = rj;
                else
                    rj = jj;
                if (board[i][j] == 'o')
                    slidersPos.emplace_back(i, j), globalBoard[i][j] = '.';
            }
        for (int j = 0; j <= n + 1; j++)
            for (int ui = 0, di = n + 1, i = 0, ii = n + 1; i <= n + 1; i++, ii--)
            {
                if (board[i][j] != '#')
                    obstaclesRowPos[j][i].first = ui;
                else
                    ui = i;
                if (board[ii][j] != '#')
                    obstaclesRowPos[j][ii].second = di;
                else
                    di = ii;
            }

        initialState = new Node(slidersPos); // first state have HashedBoard of balls sorted about rows then cols
        this->target = target;

        choosenAlgo = N_SQUARED;
        auto ci = chrono::high_resolution_clock::now();
        Move(slidersPos, UP);
        auto cf = chrono::high_resolution_clock::now();
        auto squareTime = chrono::duration_cast<chrono::microseconds>(cf - ci).count();
        choosenAlgo = S_PLUS_N;
        ci = chrono::high_resolution_clock::now();
        Move(slidersPos, UP);
        cf = chrono::high_resolution_clock::now();
        auto s_plus_n = chrono::duration_cast<chrono::microseconds>(cf - ci).count();
        choosenAlgo = S_LOG_S;
        ci = chrono::high_resolution_clock::now();
        Move(slidersPos, UP);
        cf = chrono::high_resolution_clock::now();
        auto s_lg_s = chrono::duration_cast<chrono::microseconds>(cf - ci).count();

        if (squareTime <= s_lg_s && squareTime <= s_plus_n)
            choosenAlgo = N_SQUARED;
        else if (s_plus_n <= squareTime && s_plus_n <= s_lg_s)
            choosenAlgo = S_PLUS_N;

        debug(squareTime);
        debug(s_plus_n);
        debug(s_lg_s);
    }

    HashedBoard<un short> Move(const HashedBoard<un short>& currentBoard, Direction dir) // O(min(s*log(s), n^2, n + s)), Ω(s)
    {
        moveTimer++;
        un s = currentBoard.size(); // number of sliders
        switch (dir)
        {
            case RIGHT:
            {
                auto nextBoard = currentBoard;
                un short mnj;
                int it = s - 1;
                if (s)
                    nextBoard[it].second = obstaclesColPos[nextBoard[it].first][nextBoard[it].second].second - 1;
                for (it--; ~it; it--)
                {
                    auto& [x, y] = nextBoard[it];
                    mnj = obstaclesColPos[x][y].second;
                    if (nextBoard[it + 1].first == x)
                        mnj = min(mnj, nextBoard[it + 1].second);
                    y = mnj - 1;
                }
                return nextBoard;
            }
            case LEFT:
            {
                auto nextBoard = currentBoard;
                un short mxj;
                int it = 0;
                if (s)
                    nextBoard[it].second = obstaclesColPos[nextBoard[it].first][nextBoard[it].second].first + 1;
                for (it++; it < s; it++)
                {
                    auto& [x, y] = nextBoard[it];
                    mxj = obstaclesColPos[x][y].first;
                    if (nextBoard[it - 1].first == x)
                        mxj = max(mxj, nextBoard[it - 1].second);
                    y = mxj + 1;
                }
                return nextBoard;
            }
            case UP:
            {
                switch (choosenAlgo)
                {
                    case S_LOG_S:
                    {
                        HashedBoard<un short> nextBoard;
                        un short mxi;
                        for (int it = 0; it < s; it++)
                        {
                            auto &[x, y] = currentBoard[it];
                            mxi = obstaclesRowPos[y][x].first;
                            if (ballsRowPos[y].second < moveTimer)
                                ballsRowPos[y].second = moveTimer;
                            else
                                mxi = max(mxi, ballsRowPos[y].first);
                            ballsRowPos[y].first = mxi + 1;
                            nextBoard.emplace_back(mxi + 1, y);
                        }
                        stable_sort(allc(nextBoard));
                        return nextBoard;
                    }
                    case N_SQUARED:
                    {
                        un n = globalBoard.size() - 2;
                        HashedBoard<un short> nextBoard;
                        un short mxi;
                        for (int it = 0; it < s; it++)
                        {
                            auto &[x, y] = currentBoard[it];
                            mxi = obstaclesRowPos[y][x].first;
                            if (ballsRowPos[y].second < moveTimer)
                                ballsRowPos[y].second = moveTimer;
                            else
                                mxi = max(mxi, ballsRowPos[y].first);
                            ballsRowPos[y].first = mxi + 1;
                            globalBoard[mxi + 1][y] = 'o';
                        }
                        for (int i = 1; i <= n; i++)
                            for (int j = 1; j <= n; j++)
                                if (globalBoard[i][j] == 'o')
                                    globalBoard[i][j] = '.', nextBoard.emplace_back(i, j);
                        return nextBoard;
                    }
                    case S_PLUS_N:
                    {
                        unsigned n = globalBoard.size() - 2;
                        HashedBoard<un short> nextBoard;
                        un short mxi;
                        for (int it = 0; it < s; it++)
                        {
                            auto& [x, y] = currentBoard[it];
                            mxi = obstaclesRowPos[y][x].first;
                            if (!ballsRows[y].empty())
                                mxi = max(mxi, ballsRows[y].back());
                            ballsRows[y].push_back(mxi + 1);
                        }
                        for (int y = 1; y <= n; y++)
                            while (!ballsRows[y].empty())
                                ballsCols[ballsRows[y].front()].push_back(y), ballsRows[y].pop_front();
                        for (int x = 1; x <= n; x++)
                            while (!ballsCols[x].empty())
                                nextBoard.emplace_back(x, ballsCols[x].front()), ballsCols[x].pop_front();
                        return nextBoard;
                    }
                }
            }
            case DOWN:
            {
                switch (choosenAlgo)
                {
                    case S_LOG_S:
                    {
                        HashedBoard<un short> nextBoard;
                        un short mni;
                        for (int it = s - 1; ~it; it--)
                        {
                            auto& [x, y] = currentBoard[it];
                            mni = obstaclesRowPos[y][x].second;
                            if (ballsRowPos[y].second < moveTimer)
                                ballsRowPos[y].second = moveTimer;
                            else
                                mni = min(mni, ballsRowPos[y].first);
                            ballsRowPos[y].first = mni - 1;
                            nextBoard.emplace_back(mni - 1, y);
                        }
                        stable_sort(allc(nextBoard));
                        return nextBoard;
                    }
                    case N_SQUARED:
                    {
                        un n = globalBoard.size() - 2;
                        HashedBoard<un short> nextBoard;
                        un short mni;
                        for (int it = s - 1; ~it; it--)
                        {
                            auto& [x, y] = currentBoard[it];
                            mni = obstaclesRowPos[y][x].second;
                            if (ballsRowPos[y].second < moveTimer)
                                ballsRowPos[y].second = moveTimer;
                            else
                                mni = min(mni, ballsRowPos[y].first);
                            ballsRowPos[y].first = mni - 1;
                            globalBoard[mni - 1][y] = 'o';
                        }
                        for (int i = 1; i <= n; i++)
                            for (int j = 1; j <= n; j++)
                                if (globalBoard[i][j] == 'o')
                                    globalBoard[i][j] = '.', nextBoard.emplace_back(i, j);
                        return nextBoard;
                    }
                    case S_PLUS_N:
                    {
                        unsigned n = globalBoard.size() - 2;
                        HashedBoard<un short> nextBoard;
                        un short mni;
                        for (int it = s - 1; ~it; it--)
                        {
                            auto& [x, y] = currentBoard[it];
                            mni = obstaclesRowPos[y][x].second;
                            if (!ballsRows[y].empty())
                                mni = min(mni, ballsRows[y].back());
                            ballsRows[y].push_back(mni - 1);
                        }
                        for (int y = 1; y <= n; y++)
                            while (!ballsRows[y].empty())
                                ballsCols[ballsRows[y].front()].push_back(y), ballsRows[y].pop_front();
                        for (int x = 1; x <= n; x++)
                            while (!ballsCols[x].empty())
                                nextBoard.emplace_back(x, ballsCols[x].front()), ballsCols[x].pop_front();
                        return nextBoard;
                    }
                }
            }
            case NONE:
                return currentBoard;
        }
        return currentBoard;
    }

    void Debug(HashedBoard<un short> c)
    {
        for (auto& [x, y] : c)
            globalBoard[x][y] = 'o';

        un n = globalBoard.size() - 2;
        for (int i = 1; i <= n; i++, cerr << '\n', output += '\n')
            for (int j = 1; j <= n; j++)
            {
                cerr << globalBoard[i][j], (output += globalBoard[i][j]), cerr << (j == n ? "" : ", "), (output += (j == n ? "" : ", "));
                if (globalBoard[i][j] == 'o')
                    globalBoard[i][j] = '.';
            }
        cerr << '\n', (output += '\n');
    }

    void PrintAnswer(int Difficulty)
    {
        Time(Algorithm, ShortestPath_BFS();)

        if (!finalState)
            return cerr << "Unsolvable\n", (output += "Unsolvable\n"), void();

        auto curr = finalState;
        deque<Direction> path;
        while (curr)
        {
            path.push_front(curr->parentDir);
            curr = curr->Parent;
        }
        int k = path.size();

        output += "Solvable\nMin number of moves: " + to_string(k - 1) + "\nSequence of moves: ";
        for (int i = 1; i < k; i++)
            output += Dir[path[i]] + ", ";
        cerr << (output += '\n');

        if (Difficulty)
            return;

        auto curBallBoardState = initialState->ballBoardState;
        cerr << "Initial\n", (output += "Initial\n");
        Debug(curBallBoardState);

        for (int i = 1; i < k; i++)
            cerr << Dir[path[i]] + "\n", (output += Dir[path[i]] + "\n"), Debug(curBallBoardState = Move(curBallBoardState, path[i]));
    }

    inline void ConvFromDec(un short n, string& ret) // O(log256(n))
    {
        while (n)
            ret += (n % toBase), n /= toBase;
        ret += ' ';
    }

    string ContainerHashing(const HashedBoard<un short>& container) // O(s*log256(n))
    {
        string ret;
        un s = container.size();
        for (int it = 0; it < s; it++)
            ret += baseChar[container[it].first][container[it].second];
        ret.pop_back();
        return ret;
    }

    void ShortestPath_BFS() // O(2^min(k, d) * min(s log(s), n^2)) : (k) is the depth of the answer if solvable, (d) is the max depth of the graph,   (d) is O(log((n^2 - b)C(s)))
    {
        queue<Node*> nxt;
        unordered_set<string> vis;
        vis.insert(ContainerHashing(initialState->ballBoardState));

        if (binary_search(allc(initialState->ballBoardState), target))
            return finalState = initialState, void();
        for (auto dir : {LEFT, UP, RIGHT, DOWN})
        {
            auto newBoardState = move(Move(initialState->ballBoardState, dir));
            if (vis.insert(ContainerHashing(newBoardState)).second)
            {
                auto child = new Node(newBoardState, initialState->sourceDistance + 1, initialState, dir);
                if (binary_search(allc(newBoardState), target))
                    return finalState = child, void();
                nxt.push(child);
            }
        }
        while (!nxt.empty())
        {
            auto cur = nxt.front(); nxt.pop();

            for (auto dir : adj[cur->parentDir])
            {
                auto newBoardState = move(Move(cur->ballBoardState, dir));
                if (vis.insert(ContainerHashing(newBoardState)).second)
                {
                    auto child = new Node(newBoardState, cur->sourceDistance + 1, cur, dir);
                    if (binary_search(allc(newBoardState), target))
                        return finalState = child, void();
                    nxt.push(child);
                }
            }
        }
        return void();
    }
};

void Solve()
{
    short chooseDifficulty; cout << "Choose Test Difficulty 1 : sample , 2 : Hard .\n"; cin >> chooseDifficulty;
    string FileName, outFileName;
    if (--chooseDifficulty)
    {
        FileName += "Complete Tests/";
        cout << "\nChoose Case size: ";
        short sz; cin >> sz;
        if (sz == 1)
            FileName += "1 small/";
        else if (sz == 2)
            FileName += "2 medium/";
        else if (sz == 3)
            FileName += "3 large/";

        short inputCase; cout << "\nChoose Test Case File: ";
        cin >> inputCase;
        FileName += "Case " + to_string(inputCase) + "/Case" + to_string(inputCase);
    }
    else
    {
        short inputCase; cout << "\nChoose Test Case File: ";
        cin >> inputCase;
        FileName = "Sample Tests/Case" + to_string(inputCase);
    }
    outFileName = FileName + "-output.txt";
    FileName += ".txt";
    FileI(FileName.c_str());

    int n; cin >> n;
    vector<vector<char>> grid(n + 2, vector<char>(n + 2, '#'));

    char cell;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            while (cin >> cell, cell != '#' && cell != '.' && cell != 'o');
            grid[i][j] = cell;
        }


    pair<un short, un short> target;

    cin >> target.second, cin.ignore(), cin >> target.first;
    target.first++;
    target.second++;

    Graph g(grid, target);
    g.PrintAnswer(chooseDifficulty);


    fclose(stdin);
    FileI(outFileName.c_str())
    string tmp;
    while (getline(cin, tmp) && tmp != "Initial")
        expected += tmp + '\n';
    if (!chooseDifficulty)
        if (tmp.size())
            while (expected += tmp + '\n', getline(cin, tmp));

    cerr << "\t\t\t\t\t" << (output == expected ? "ACCEPTED" : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [REJECTED] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
}

signed main()
{
    Solve(), cout << '\n';
    return 0;
}
