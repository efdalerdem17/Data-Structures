/*
This code was prepared by Ayşe Efdal Erdem for Sabancı University CS300 Course Homework 4
This program handles both bipartite checking and maximum matching finding in a graph
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

// check graph is bipartite or not
// find max matching
class taskworkergraph {
private:
    int n, m; // n for vertices , m for edges
    vector<vector<int>> adjelist; // for graph representation
    vector<int> match1, match2; // matching arrays
    vector<int> pathlengths;
    vector<bool> visitedvert;

public:
    //constructor
    taskworkergraph(int n, int m) : n(n), m(m) {
        adjelist.resize(n + 1);
        match1.assign(n + 1, -1);
        match2.assign(n + 1, -1);
        pathlengths.resize(n + 1); // for path finding
        visitedvert.resize(n + 1); // for marking visited ones
    }
// for adding directed edge from vertex u to vertex v
    void newedgeaddition(int u, int v) {
        adjelist[u].push_back(v);
    }
// kuhn algorithm implementation
// finds maximum bipartite matching by iteratively searching for augmenting paths
    bool kuhnalg(int v) {
        if (visitedvert[v]) return false; // do not get into account already visited node
        visitedvert[v] = true;
        for (int u : adjelist[v]) {
            if (match2[u] == -1 || kuhnalg(match2[u])) {
                match2[u] = v; // update matching
                match1[v] = u;
                return true;
            }
        }
        return false;
    }

    int numofmaxmatch() {
        //path for each vertex
        for (int v = 1; v <= n; ++v) {
            fill(visitedvert.begin(), visitedvert.end(), false); //reset
            kuhnalg(v);
        }
// number of matched vertices
        int result = 0;
        for (int i = 1; i <= n; ++i)
            if (match1[i] != -1)
                result++;
        return result;
    }
// decide whether bipartite or not
    bool checkbip() {
        vector<int> color(n + 1, -1);
        for (int i = 1; i <= n; ++i) {
            if (color[i] == -1) {
                queue<int> q;
                color[i] = 0; //color first vertex
                q.push(i);
                // color adjacent vertices
                while (!q.empty()) {
                    int u = q.front();
                    q.pop();
                    for (int v : adjelist[u]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u]; // color with opposite color
                            q.push(v);
                            // if same color --> not bipartite
                        } else if (color[v] == color[u]) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
};

int main() {
    string filename;
    while (true) {
        cout << "Enter the graph name (e.g., 'graph1' or 'exit' to terminate): ";
        cin >> filename;
        if (filename == "exit") break;
// open and read input file
        ifstream file(filename + ".txt");
        if (!file.is_open()) {
            continue;
        }
// read graph dimensions
        int n, m;
        file >> n >> m;
        taskworkergraph graph(n, m);
// read edges
        int u, v;
        for (int i = 0; i < m; ++i) {
            file >> u >> v;
            graph.newedgeaddition(u, v);
        }
        file.close();
// output results
        if (graph.checkbip()) {
            cout << "The graph is bipartite." << endl;
            int matchingSize = graph.numofmaxmatch();
            cout << "Maximum matching size: " << matchingSize << endl;
        } else {
            cout << "The graph is not bipartite." << endl;
        }
    }
    return 0;
}
