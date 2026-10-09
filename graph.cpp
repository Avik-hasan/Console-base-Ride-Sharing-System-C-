#include "graph.hpp"
#include "queue.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

CityGraph::CityGraph() {}

CityGraph::~CityGraph() {
    for (int i = 0; i < adjList.sz(); i++) {
        while (adjList[i]) {
            EdgeNode* next = adjList[i]->next;
            delete adjList[i];
            adjList[i] = next;
        }
    }
}

int CityGraph::addcty(string name) {
    int idx = fndcty(name);
    if (idx != -1) return idx;

    cities.pshbk(name);
    adjList.pshbk(nullptr);
    return cities.sz() - 1;
}

bool CityGraph::rmvcty(int cityIdx) {
    if (cityIdx < 0 || cityIdx >= cities.sz()) return false;

    while (adjList[cityIdx]) {
        rmvrd(cityIdx, adjList[cityIdx]->dest);
    }

    // nijeke dlt koro
    cities.ers(cityIdx);
    adjList.ers(cityIdx);

    //majher theke sohor muchle porer sobar index 1 kome jay tai update kora
    for (int i = 0; i < cities.sz(); i++) {
        for (EdgeNode* e = adjList[i]; e; e = e->next) {
            if (e->dest > cityIdx) e->dest--;
        }
    }
    return true;
}

int CityGraph::fndcty(string name) {
    for (int i = 0; i < cities.sz(); i++) {
        if (cities[i] == name) return i;
    }
    return -1;
}

void CityGraph::shwcty() {
    cout << "\n--- Cities in System (" << cities.sz() << ") ---\n";
    for (int i = 0; i < cities.sz(); i++) {
        cout << i + 1 << ". " << cities[i] << "\t";
        if ((i + 1) % 4 == 0 || i == cities.sz() - 1) cout << "\n";
    }
}

void CityGraph::addrd(int from, int to, double weight) {
    if (from < 0 || from >= cities.sz() || to < 0 || to >= cities.sz() || from == to) return;

    int u[2] = {from, to};
    int v[2] = {to, from};
    for (int i = 0; i < 2; i++) {
        EdgeNode* node = new EdgeNode(v[i], weight, nullptr, adjList[u[i]]);
        if (adjList[u[i]]) adjList[u[i]]->prev = node;
        adjList[u[i]] = node;
    }
}

void CityGraph::rmvrd(int from, int to) {
    if (from < 0 || from >= cities.sz() || to < 0 || to >= cities.sz()) return;

    for (int dir = 0; dir < 2; dir++) {
        int u = (dir == 0) ? from : to;
        int target = (dir == 0) ? to : from;
        EdgeNode *curr = adjList[u];
        while (curr) {
            if (curr->dest == target) {
                if (curr->prev) curr->prev->next = curr->next;
                else adjList[u] = curr->next;

                if (curr->next) curr->next->prev = curr->prev;

                delete curr;
                break;
            }
            curr = curr->next;
        }
    }
}

bool CityGraph::hsrd(int from, int to) {
    if (from < 0 || from >= cities.sz() || to < 0 || to >= cities.sz()) return false;
    for (EdgeNode* curr = adjList[from]; curr; curr = curr->next) {
        if (curr->dest == to) return true;
    }
    return false;
}

bool CityGraph::bfs(int start, int goal, Vector<int>& path, double& totalDist) {
    path.clr();
    totalDist = 0.0;
    int count = cities.sz();
    if (start < 0 || start >= count || goal < 0 || goal >= count) return false;
    if (start == goal) {
        path.pshbk(start);
        return true;
    }

    bool* visited = new bool[count]();
    int* parent = new int[count];
    double* edgeW = new double[count]();
    for (int i = 0; i < count; i++) parent[i] = -1;

    BFSQueue q;
    visited[start] = true;
    q.enq(start);

    bool reached = false;
    while (!q.emp()) {
        int curr = q.deq();
        if (curr == goal) {
            reached = true;
            break;
        }

        for (EdgeNode* e = adjList[curr]; e; e = e->next) {
            if (!visited[e->dest]) {
                visited[e->dest] = true;
                parent[e->dest] = curr;
                edgeW[e->dest] = e->weight;
                q.enq(e->dest);
            }
        }
    }

    if (reached) {
        Vector<int> tempPath;
        int curr = goal;
        while (curr != -1) {
            tempPath.pshbk(curr);
            if (curr != start) totalDist += edgeW[curr];
            curr = parent[curr];
        }
        for (int i = tempPath.sz() - 1; i >= 0; i--) {
            path.pshbk(tempPath[i]);
        }
    }

    delete[] visited;
    delete[] parent;
    delete[] edgeW;
    return reached;
}

bool CityGraph::dfshlpr(int curr, int goal, bool* visited, Vector<int>& path) {
    visited[curr] = true;
    path.pshbk(curr);
    if (curr == goal) return true;

    for (EdgeNode* e = adjList[curr]; e; e = e->next) {
        if (!visited[e->dest] && dfshlpr(e->dest, goal, visited, path)) {
            return true;
        }
    }
    path.ppbk();
    return false;
}

bool CityGraph::dfs(int start, int goal, Vector<int>& path) {
    path.clr();
    int count = cities.sz();
    if (start < 0 || start >= count || goal < 0 || goal >= count) return false;
    bool* visited = new bool[count]();
    bool found = dfshlpr(start, goal, visited, path);
    delete[] visited;
    return found;
}

void CityGraph::ldnodes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string srcName;
        if (!getline(ss, srcName, ',')) continue;

        int srcIdx = addcty(srcName);
        string destName, distStr;
        while (getline(ss, destName, ',') && getline(ss, distStr, ',')) {
            int destIdx = addcty(destName);
            try {
                double dist = stod(distStr);
                if (!hsrd(srcIdx, destIdx)) {
                    addrd(srcIdx, destIdx, dist);
                }
            } catch (...) {}
        }
    }
}

void CityGraph::svnodes(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < cities.sz(); i++) {
        file << cities[i];
        for (EdgeNode* e = adjList[i]; e; e = e->next) {
            file << "," << cities[e->dest] << "," << e->weight;
        }
        file << "\n";
    }
}