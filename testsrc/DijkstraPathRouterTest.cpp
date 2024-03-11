#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

TEST(DijkstraPathRouter, FindShortestPath){
    CDijkstraPathRouter Router;
    std::vector<CPathRouter::TVertexID> Vertices;
    for (int Index = 0; Index < 5; Index++){
        Vertices.push_back(Router.AddVertex(Index));
    }
    Router.AddEdge(Vertices[0], Vertices[1], 1);
    Router.AddEdge(Vertices[1], Vertices[3], 4);
    Router.AddEdge(Vertices[2], Vertices[3], 5);
    Router.AddEdge(Vertices[0], Vertices[4], 22);
    Router.AddEdge(Vertices[1], Vertices[4], 7);
    Router.AddEdge(Vertices[3], Vertices[4], 1);

    std::vector<CPathRouter::TVertexID> Path;
    std::vector<CPathRouter::TVertexID> ExpectedPath = {Vertices[1], Vertices[3], Vertices[4]};
    EXPECT_EQ(5.0, Router.FindShortestPath(Vertices[1], Vertices[4], Path));
    EXPECT_EQ(Path, ExpectedPath);
}

TEST(DijkstraPathRouter, AddVertex){
    CDijkstraPathRouter Router;
    EXPECT_EQ(0, Router.VertexCount());

    Router.AddVertex(1);
    EXPECT_EQ(1, Router.VertexCount());

    Router.AddVertex(2);
    EXPECT_EQ(2, Router.VertexCount());
}

TEST(DijkstraPathRouter, GetVertexTag){
    CDijkstraPathRouter Router;
    auto Vertex1 = Router.AddVertex(1);
    auto Vertex2 = Router.AddVertex(2);

    EXPECT_EQ(1, std::any_cast<int>(Router.GetVertexTag(Vertex1)));
    EXPECT_EQ(2, std::any_cast<int>(Router.GetVertexTag(Vertex2)));
}

TEST(DijkstraPathRouter, NoPathExists){
    CDijkstraPathRouter Router;
    auto Vertex1 = Router.AddVertex(1);
    auto Vertex2 = Router.AddVertex(2);

    std::vector<CPathRouter::TVertexID> Path;
    EXPECT_EQ(CPathRouter::NoPathExists, Router.FindShortestPath(Vertex1, Vertex2, Path));
}