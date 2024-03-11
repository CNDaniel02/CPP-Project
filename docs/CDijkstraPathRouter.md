CDijkstraPathRouter Documentation

Overview 
DijkstraPathRouter is a C++ class used to implement Dijkstra's shortest path algorithm. This class can be used for path finding and routing calculations in graphs.

Dependencies
Standard library components, such as<iostream> and <algorithm>

Initialization
CDijkstraPathRouter Router;

Constructor
CDijkstraPathRouter::CDijkstraPathRouter(){
    DImplementation = std::make_unique< SImplementation >();
}

Destructor
CDijkstraPathRouter::~CDijkstraPathRouter(){
}

Member Functions
std::size_t CDijkstraPathRouter::VertexCount() const noexcept{
    return DImplementation->VertexCount();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept{
    return DImplementation->AddVertex(tag);
}
......

These functions are used to get the number of vertices, add a vertex, get the tag associated with a vertex, add an edge, precompute, and find the shortest path respectively.
