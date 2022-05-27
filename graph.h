#ifndef INC_GRAPH_H
#define INC_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

#include "log.h"

#define MATRIX(_T) std::vector<std::vector<_T>>


struct Edge
{
    Edge(int start, int end, int w);
    int source;
    int dest;
    int weight;
};


class Graph
{
public:
    Graph();

    void print(const std::string &end = "\n") const;

    int sizeVertices() const;
    int sizeEdges() const;

    Edge getEdge(const std::string &start, const std::string &end) const;
    Edge getEdge(const std::string &start, int end) const;
    Edge getEdge(int start, const std::string &end) const;
    Edge getEdge(int start, int end) const;

    int getVertexIndex(const std::string &key) const;
    std::string getVertexName(int index) const;

    std::vector<std::string> getVertices() const;
    std::vector<Edge> getEdges() const;

    void addVertex(const std::string &key);
    void addEdge(const std::string &start, const std::string &end, int weight = 0);

    int getMaxFlow(const std::string &source, const std::string &sink);

private:
    std::map<std::string, int> _vertices;
    std::string _source, _sink;
    MATRIX(int) _adjacencyMatrix;
    MATRIX(int) _flowMatrix;
    MATRIX(std::string) _lines;

    std::vector<Edge> _getAugmentingPath(const MATRIX(int) &flow, int s, int t);
    static int _getBottleneckFlow(const std::vector<Edge> &path, const MATRIX(int) &flow);
};


bool loadGraph(Graph &graph, const std::string &path);

#endif //INC_GRAPH_H