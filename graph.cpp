#include "graph.h"

bool loadGraph(Graph &graph, const std::string &path)
{
    try {
        std::ifstream file;

        file.open(path);
        if(!file.is_open())
        {
            std::stringstream ss;
            ss << "Could not open file: " << path;
            throw std::runtime_error(ss.str());
        }

        MATRIX(std::string) lines;
        std::string buffer;
        while(std::getline(file, buffer))
        {
            DEBUGLN("loaded line: " << buffer);
            std::stringstream ss(buffer);
            std::vector<std::string> l;
            while(std::getline(ss, buffer, ' '))
            {
                l.push_back(buffer);
            }
            lines.push_back(l);
        }

        file.close();

        int line_num = 1, expected_line_tokens = 3;
        for(auto line : lines)
        {
            if(line.size() != expected_line_tokens)
            {
                std::stringstream ss;
                ss << "std::ifstream loading error: too "
                << (line.size() < expected_line_tokens ? "few" : "much")
                << " values on line " << line_num << '.';
                throw std::runtime_error(ss.str());
            }
            auto a = line.at(0);
            auto b = line.at(1);
            auto w = std::stoi(line.at(2));
            DEBUGLN("Adding edge (" << a << " -" << w << "-> " << b << ")");
            graph.addVertex(a);
            graph.addVertex(b);
            graph.addEdge(a, b, w);
            ++line_num;
        }
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return false;
    }
    return true;
}

Edge::Edge(int start, int end, int w):
        source{start},dest{end},weight{w}{}

Graph::Graph() = default;

void Graph::print(const std::string &end) const
{
    int index = 0;
    LOG("Graph matrix (" << sizeVertices() << 'x' << sizeVertices() << "):\n");
    for(const auto& line : _adjacencyMatrix)
    {
        LOG(getVertexName(index) << ' ');
        for(auto digit : line)
        {
            LOG(digit << ' ');
        }
        LOG('\n');
        ++index;
    }
    LOG(end);
}

int Graph::sizeVertices() const
{
    return static_cast<int>(_vertices.size());
}

int Graph::sizeEdges() const
{
    int edges = 0;
    for(const auto &row : _adjacencyMatrix)
    {
        for(const auto &weight : row)
        {
            if(weight != 0)
            {
                ++edges;
            }
        }
    }
    return edges;
}

Edge Graph::getEdge(const std::string &start, const std::string &end) const
{return getEdge(getVertexIndex(start), getVertexIndex(end));}

Edge Graph::getEdge(const std::string &start, int end) const
{return getEdge(getVertexIndex(start), end);}

Edge Graph::getEdge(int start, const std::string &end) const
{return getEdge(start, getVertexIndex(end));}

Edge Graph::getEdge(int start, int end) const
{return {start, end, _adjacencyMatrix.at(start).at(end)};}

int Graph::getVertexIndex(const std::string &key) const
{
    auto node = _vertices.find(key);
    if(node != _vertices.end())
    {
        return node->second;
    }
    return -1;
}

std::string Graph::getVertexName(int index) const
{
    for(const auto &pair : _vertices)
    {
        if(pair.second == index)
        {
            return pair.first;
        }
    }
    std::stringstream ss;
    ss << "Unknown index: " << index;
    throw std::runtime_error(ss.str());
}

std::vector<std::string> Graph::getVertices() const
{
    std::vector<std::string> nds;
    for(const auto& n : _vertices)
    {
        nds.push_back(n.first);
    }
    return nds;
}

std::vector<Edge> Graph::getEdges() const
{
    std::vector<Edge> edges;
    for(int i = 0; i < sizeVertices(); ++i)
    {
        for(int j = 0; j < sizeVertices(); ++j)
        {
            auto e = getEdge(i, j);
            if(e.weight != 0)
            {
                edges.push_back(e);
            }
        }
    }
    return edges;
}

void Graph::addVertex(const std::string &key) {
    auto node = _vertices.find(key);
    if(node != _vertices.end())
    {
        return;
    }
    int new_index = static_cast<int>(_vertices.size());
    _vertices.insert({key, new_index});
    _adjacencyMatrix.resize(new_index + 1);
    for(auto &row : _adjacencyMatrix)
    {
        row.resize(new_index + 1);
    }
}

void Graph::addEdge(const std::string &start, const std::string &end, int weight) {
    int i = getVertexIndex(start), j = getVertexIndex(end);
    _adjacencyMatrix[i][j] = weight;
}

int Graph::getMaxFlow(const std::string &source, const std::string &sink)
{
    int n = sizeVertices();
    MATRIX(int) flow(n); for(auto &l : flow) l.resize(n);
    int s = getVertexIndex(source);
    int t = getVertexIndex(sink);
    std::vector<Edge> path = _getAugmentingPath(flow, s, t);
    while(!path.empty())
    {
        int bottleneck = _getBottleneckFlow(path, flow);
        for(auto edge : path)
        {
            int i = edge.source, j = edge.dest;
            flow[i][j] += bottleneck;
            flow[j][i] -= bottleneck;
        }
        path = _getAugmentingPath(flow, s, t);
    }

    int max_flow = 0;
    for(const auto &f : flow[s])
    {
        max_flow += f;
    }
    return max_flow;
}

std::vector<Edge> Graph::_getAugmentingPath(const MATRIX(int) &flow, int s, int t) {
    std::queue<int> q; q.push(s);
    std::map<int, std::vector<Edge>> paths;
    paths[s] = {};

    if(s == t)
    {
        return paths[s];
    }

    while(!q.empty())
    {
        int u = q.front(); q.pop();
        for(const auto &vn : getVertices())
        {
            int v = getVertexIndex(vn);
            if((_adjacencyMatrix[u][v] - flow[u][v] > 0) and (paths.find(v) == paths.end()))
            {
                std::vector<Edge> new_path = paths[u];
                new_path.push_back(getEdge(u, v));
                if(v == t)
                {
                    return new_path;
                }
                paths[v] = new_path;
                q.push(v);
            }
        }
    }
    return {};
}

int Graph::_getBottleneckFlow(const std::vector<Edge> &path, const MATRIX(int) &flow) {
    int bottleneck = INT_MAX, residual_capacity;
    for(const auto &edge : path)
    {
        residual_capacity = edge.weight - flow[edge.source][edge.dest];
        if(residual_capacity < bottleneck)
        {
            bottleneck = residual_capacity;
        }
    }
    return bottleneck;
}
