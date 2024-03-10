#include "DijkstraPathRouter.h"
#include <algorithm>
#include <iostream>

struct CDijkstraPathRouter::SImplementation{
    using TEdge = std::pair<double, TVertexID>;

    struct SVertex{
        std::any DTag;
        std::vector< TEdge > DEdges;
    };

    std::vector< SVertex > DVertices;

    std::size_t VertexCount() const noexcept{
        return DVertices.size();
    }//returns the number of vertices in the graph

    TVertexID AddVertex(std::any tag) noexcept{
        TVertexID NewVertexID = DVertices.size();
        DVertices.push_back({tag,{}});
        return NewVertexID;
    }//adds a vertex to the graph and returns the vertex ID

    std::any GetVertexTag(TVertexID id) const noexcept{
        if (id < DVertices.size()){
            return DVertices[id].DTag;
        }
        return std::any();
    }//returns the tag associated with the vertex

    bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept{
        if ((src < DVertices.size()) && (dest < DVertices.size()) && (0.0 <= weight)){
            //if the source and destination vertices are valid and the weight is non-negative
            DVertices[src].DEdges.push_back(std::make_pair(weight, dest));
            if (bidir){
                DVertices[dest].DEdges.push_back(std::make_pair(weight, src));
            }//if bidir exists, add the edge in the opposite direction
            return true;
        }
        return false;//if the source and destination vertices are invalid or the weight is negative, return false
    }

    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
        return true;
    }

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        std::vector<TVertexID> PendingVertices;
        std::vector<TVertexID> Previous(DVertices.size(), CPathRouter::InvalidVertexID);
        std::vector< double > Distances(DVertices.size(), CPathRouter::NoPathExists);
        auto VertexCompare = [&Distances](TVertexID left, TVertexID right){
            return Distances[left] < Distances[right];
        };//compare the distances of two vertices

        Distances[src] = 0.0;
        PendingVertices.push_back(src);
        while (!PendingVertices.empty()){
            auto CurrentID = PendingVertices.front();
            std::pop_heap(PendingVertices.begin(), PendingVertices.end(), VertexCompare);
            PendingVertices.pop_back();

            for (auto Edge : DVertices[CurrentID].DEdges){
                auto EdgeWeight = Edge.first;
                auto DestID = Edge.second;
                auto TotalDistance = Distances[CurrentID] + EdgeWeight;
                if (TotalDistance < Distances[DestID]){
                    if (CPathRouter::NoPathExists == Distances[DestID]){
                        PendingVertices.push_back(DestID);
                    }
                    Distances[DestID] = TotalDistance;
                    Previous[DestID] = CurrentID;
                }
                }//if the total distance is less than the distance to the destination, update the distance and previous vertex
            std::make_heap(PendingVertices.begin(), PendingVertices.end(), VertexCompare);
            }
        if (CPathRouter::NoPathExists == Distances[dest]){
            return CPathRouter::NoPathExists;
        }//if no path exists, return NoPathExists
        double PathDistance = Distances[dest];
        path.clear();
        path.push_back(dest);
        do {
            dest = Previous[dest];
            path.push_back(dest);
        } while (src != dest);//add the previous vertex to the path until the source vertex is reached
        std::reverse(path.begin(), path.end());
        return PathDistance;
    }//finds the shortest path between two vertices and returns the distance of the path
};

CDijkstraPathRouter::CDijkstraPathRouter(){
    DImplementation = std::make_unique< SImplementation >();
}

CDijkstraPathRouter::~CDijkstraPathRouter(){

}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept{
    return DImplementation->VertexCount();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept{
    return DImplementation->AddVertex(tag);
}

std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept{
    return DImplementation->GetVertexTag(id);
}

bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept{
    return DImplementation->AddEdge(src, dest, weight, bidir);
}

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
    return DImplementation->Precompute(deadline);
}

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
    return DImplementation->FindShortestPath(src, dest, path);
}



