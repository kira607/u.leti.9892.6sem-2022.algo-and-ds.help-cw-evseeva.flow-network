#include "graph.h"
#include "log.h"


int main(){
    Graph graph;

    std::string data_path = "../data/data_1.txt";
    if(!loadGraph(graph, data_path))
        return 1;

    DEBUGLN("Loaded graph from " << data_path);

    DEBUG("Vertices (count: " << graph.sizeVertices() << "):\n");
    int num = 1;
    for(const auto &vertex: graph.getVertices())
    {
        DEBUGLN('[' << num << "]: " << vertex);
        ++num;
    }

    graph.print();

    int max_flow = graph.getMaxFlow("S", "T");
    std::cout << "Max flow is: " << max_flow << '\n';

    return 0;
}
