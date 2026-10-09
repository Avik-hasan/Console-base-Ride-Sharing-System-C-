#pragma once

#include <string>
#include "vector.hpp"


struct EdgeNode {
    int dest;     
    double weight;
    EdgeNode* prev;
    EdgeNode* next;

    EdgeNode(int d = -1, double w = 0.0, EdgeNode* p = nullptr, EdgeNode* n = nullptr)
        : dest(d), weight(w), prev(p), next(n) {}
};

class CityGraph {
private:
    Vector<EdgeNode*> adjList;  
    bool dfshlpr(int current, int goal, bool* visited, Vector<int>& path);

public:
    Vector<std::string> cities;  

    CityGraph();
    ~CityGraph();
    
    int addcty(std::string name);
    bool rmvcty(int cityIdx);
    int fndcty(std::string name);
    void shwcty();
    
    void addrd(int from, int to, double weight);
    void rmvrd(int from, int to);
    bool hsrd(int from, int to);
   bool bfs(int start, int goal, Vector<int>& path, double& totalDist);
    bool dfs(int start, int goal, Vector<int>& path);
  void ldnodes(const std::string& filename);
    void svnodes(const std::string& filename);
};

