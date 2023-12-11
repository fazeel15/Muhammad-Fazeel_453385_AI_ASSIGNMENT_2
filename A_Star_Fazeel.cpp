/*
Name Muhammad Fazeel
Reg No 453385
AI Assignment 2
*/
#include <bits/stdc++.h>

using namespace std;

map<vector<vector<int> >, bool> visited;
map<vector<vector<int> >, vector<vector<int> > > parent;
vector<vector<int> > goal(3, vector<int>(3));

bool visit(vector<vector<int> > a)
{
    if (visited[a] == true)
        return true;
    else
        return false;
}

int heuristic(vector<vector<int> > a, int moves, int choice)
{
    if (choice == 1) // Manhattan Distance
    {
        int dist = moves;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (a[i][j] != 0)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        for (int l = 0; l < 3; l++)
                        {
                            if (a[i][j] == goal[k][l])
                                dist += abs(i - k) + abs(j - l);
                        }
                    }
                }
            }
        }
        return dist;
    }
    else if (choice == 2) // Misplaced Tiles
    {
        int misplaced = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (a[i][j] != goal[i][j])
                    misplaced++;
            }
        }
        return misplaced;
    }

    return 0; // Default case (shouldn't happen)
}

bool isGoal(vector<vector<int> > a)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (a[i][j] != goal[i][j])
                return false;
        }
    }

    return true;
}

bool safe(int i, int j)
{
    if (i >= 0 && i <= 2 && j >= 0 && j <= 2)
        return true;
    else
        return false;
}
int dx[] = {-1, +1, 0, 0};
int dy[] = {0, 0, -1, +1};
vector<vector<vector<int> > > neighbours(vector<vector<int> > a)
{
    pair<int, int> pos;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (a[i][j] == 0)
            {
                pos.first = i;
                pos.second = j;
                break;
            }
        }
    }
    vector<vector<vector<int> > > ans;
    for (int k = 0; k < 4; k++)
    {
        int cx = pos.first;
        int cy = pos.second;
        vector<vector<int> > n = a;
        if (safe(cx + dx[k], cy + dy[k]))
        {
            swap(n[cx + dx[k]][cy + dy[k]], n[cx][cy]);
            ans.push_back(n);
        }
    }

    return ans;
}

typedef pair<vector<vector<int> >, int> state;

struct cmp
{
    bool operator()(state &a, state &b)
    {
        return a.second > b.second; // Changed to use greater than for priority queue
    }
};

void print(vector<vector<int> > s)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", s[i][j]);
        }
        cout << endl;
    }
}
void print_path(vector<vector<int> > s, std::ofstream &outFile)
{
    if (parent.count(s))
        print_path(parent[s], outFile);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            outFile << s[i][j] << " ";
        }
        outFile << endl;
    }

    outFile << endl;
}
void solve(vector<vector<int> > a, int moves, int heuristicChoice, ofstream &outFile)
{

    priority_queue<state, vector<state>, cmp> Q;
    Q.push(state(a, moves));
    while (!Q.empty())
    {
        vector<vector<int> > s = Q.top().first;
        moves = Q.top().second; // Corrected to get the moves from the priority queue
        Q.pop();

        visited[s] = true;

        if (s == goal)
        {
            print_path(s, outFile);
            break;
        }

        vector<vector<vector<int> > > ns = neighbours(s);
        vector<vector<vector<int> > >::iterator it;

        for (it = ns.begin(); it != ns.end(); it++)
        {
            vector<vector<int> > temp = *it;
            if (!visit(temp))
            {
                parent.insert(pair<vector<vector<int> >, vector<vector<int> > >(temp, s));
                int h = heuristic(temp, moves + 1, heuristicChoice);
                Q.push(state(temp, moves + 1 + h));
            }
        }
    }
    return;
}

int main()
{
    vector<vector<int> > a(3, vector<int>(3));
    cout << "Enter the initial state:" << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cin >> a[i][j];
        }
    }

    int heuristicChoice;
    cout << "Choose heuristic:\n1. Manhattan Distance\n2. Misplaced Tiles\n";
    cin >> heuristicChoice;

    goal[0][0] = 0;
    goal[0][1] = 1;
    goal[0][2] = 2;
    goal[1][0] = 3;
    goal[1][1] = 4;
    goal[1][2] = 5;
    goal[2][0] = 6;
    goal[2][1] = 7;
    goal[2][2] = 8;

    ofstream outFile("A Star solution Assignment 2.txt");
    if (!outFile.is_open())
    {
        cerr << "Unable to open the file for writing." << endl;
        return 1; // Exit with an error code
    }

    cout << "Solution...\n\n";
    solve(a, 0, heuristicChoice, outFile);

    outFile.close();
}
