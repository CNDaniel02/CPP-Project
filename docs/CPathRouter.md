CPathRouter.md Overview CPathRouter is abstract base class for path
routing. It defines interface for path routing function, such as adding
vertices, adding edges, and finding the shortest path.

Dependencies

Standard C++ libraries.

Member Functions

VertexCount returns the number of vertices in the graph. AddVertex adds
a vertex to the graph. GetVertexTag retrieves the tag of a vertex.
AddEdge adds an edge between two vertices. Precompute optimizes internal
data structures for pathfinding. FindShortestPath computes the shortest
path between two vertices.
