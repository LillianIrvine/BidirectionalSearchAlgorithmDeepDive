#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

//structure representing a position in the grid
struct Node {
	int row;
	int col;

	bool operator==(const Node& other) const {
		return row == other.row && col == other.col;
	}
};

//hash function for unordered_map
struct NodeHash {
	size_t operator()(const Node& n) const {
		return hash<int>()(n.row) ^ hash<int>()(n.col);
	}
};

// directions for moving up, down, left, and right

int dr[] = { -1, 1, 0, 0 };
int dc[] = { 0, 0, -1, 1 };

// function to check validity of position
bool isValid(int r, int c, vector<vector<char>>& grid) {
	return r >= 0 &&
		r < grid.size() &&
		c >= 0 &&
		c < grid[0].size() &&
		grid[r][c] != '#';
}

// function to reconstruct final path
vector<Node> buildPath(
	Node meetingPoint,
	unordered_map<Node, Node, NodeHash>& parentStart,
	unordered_map<Node, Node, NodeHash>& parentGoal,
	Node start,
	Node goal)
{
	vector<Node> path;

// build path from meeting point back to start
	Node current = meetingPoint;
	while (!(current == start)) {
		path.push_back(current);
		current = parentStart[current];
	}

	path.push_back(start);

	reverse(path.begin(), path.end());

// build path from meeting point to goal
current = meetingPoint;

while (!(current == goal)) {
	current = parentGoal[current];
	path.push_back(current);
	}

	return path;
}

int main() {

//grid setup
	vector<vector<char>> grid = {
	{'S', '.', '.', '#', '.'},
	{'#', '#', '.', '#', '.'},
	{'.', '.', '.', '.', 'G'}
	};

	int rows = grid.size();
	int cols = grid[0].size();

	Node start, goal;

//find start and goal positions
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (grid[r][c] == 'S')
				start = { r, c };

			if (grid[r][c] == 'G')
				goal = { r, c };
		}
	}

//queues for BFS

	queue<Node> qStart;
	queue<Node> qGoal;

//Visited maps
    unordered_map<Node, bool, NodeHash> visitedStart;
    unordered_map<Node, bool, NodeHash> visitedGoal;

//parent tracking
    unordered_map<Node, Node, NodeHash> parentStart;
    unordered_map<Node, Node, NodeHash> parentGoal;

// Initialize searches
    qStart.push(start);
    qGoal.push(goal);

    visitedStart[start] = true;
    visitedGoal[goal] = true;

    bool found = false;
    Node meetingPoint;

// Bidirectional BFS loop
    while (!qStart.empty() && !qGoal.empty()) {

// Expand from start side
        int sizeStart = qStart.size();

        for (int i = 0; i < sizeStart; i++) {

            Node current = qStart.front();
            qStart.pop();

// Check all neighbors
            for (int d = 0; d < 4; d++) {

                int nr = current.row + dr[d];
                int nc = current.col + dc[d];

                if (isValid(nr, nc, grid)) {

                    Node neighbor = { nr, nc };

                    if (!visitedStart[neighbor]) {

                        visitedStart[neighbor] = true;
                        parentStart[neighbor] = current;
                        qStart.push(neighbor);

// Check if other search visited this node
                        if (visitedGoal[neighbor]) {
                            found = true;
                            meetingPoint = neighbor;
                            break;
                        }
                    }
                }
            }

            if (found) break;
        }

        if (found) break;

// Expand from goal side
        int sizeGoal = qGoal.size();

        for (int i = 0; i < sizeGoal; i++) {

            Node current = qGoal.front();
            qGoal.pop();

// Check neighbors
            for (int d = 0; d < 4; d++) {

                int nr = current.row + dr[d];
                int nc = current.col + dc[d];

                if (isValid(nr, nc, grid)) {

                    Node neighbor = { nr, nc };

                    if (!visitedGoal[neighbor]) {

                        visitedGoal[neighbor] = true;
                        parentGoal[neighbor] = current;
                        qGoal.push(neighbor);

// Check if searches meet
                        if (visitedStart[neighbor]) {
                            found = true;
                            meetingPoint = neighbor;
                            break;
                        }
                    }
                }
            }

            if (found) break;
        }
    }

// Output result
    if (found) {

        vector<Node> path = buildPath(
            meetingPoint,
            parentStart,
            parentGoal,
            start,
            goal
        );

        cout << "Path found!\n\n";

// Mark path on grid
        for (Node p : path) {

            if (grid[p.row][p.col] == '.')
                grid[p.row][p.col] = '*';
        }

// Print grid
        for (auto row : grid) {

            for (char cell : row)
                cout << cell << " ";

            cout << endl;
        }

    }
    else {

        cout << "No path found.\n";
    }

    return 0;
}