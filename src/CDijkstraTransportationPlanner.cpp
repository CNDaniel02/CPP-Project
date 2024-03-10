#include "DijkastraTransportationPlanner.h"
#include "GeographicUtils.h" 

#include "BusSystem.h"
#include "StreetMap.h"
#include "DSVReader.h"
#include "CSVBusSystem.h"

#include <limits>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>



// CDijkstraTransportationPlanner member functions
// Constructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) :DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->config = config;
    auto streetMap = config->StreetMap();
}

// Destructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {
    
}


// Returns the number of nodes in the street map 
std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    
    return DImplementation->config->StreetMap()->NodeCount();
}



// Returns the street map node specified by index if index is less than the  
// NodeCount(). nullptr is returned if index is greater than or equal to  
// NodeCount(). The nodes are sorted by Node ID. 
std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    auto streetMap = DImplementation->config->StreetMap();//functions in streetmap.h
    std::vector<std::shared_ptr<CStreetMap::SNode>> nodes;

    //Copy all nodes to a local vector
    for (std::size_t i = 0; i < streetMap->NodeCount(); ++i) {
        nodes.push_back(streetMap->NodeByIndex(i));
    }

    //Sort nodes by their ID
    std::sort(nodes.begin(), nodes.end(), [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
        return a->ID() < b->ID();
        });//if successfully sorted, then a<b will return true

    //Check if the index is valid and return the node
    if (index < nodes.size()) {
        return nodes[index];
    }
    else {//Index out of bounds
        return nullptr;
    }
}




// Returns the distance in miles between the src and dest nodes of the  
// shortest path if one exists. NoPathExists is returned if no path exists.  
// The nodes of the shortest path are filled in the path parameter. 
double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID>& path) {

    auto streetMap = DImplementation->config->StreetMap();


    // unable to reach the destination
    constexpr double NoPathExists = std::numeric_limits<double>::infinity();

    

    std::unordered_map<TNodeID, double> distances; //unordered map store the shortest distance from source node to all other nodes
    std::unordered_map<TNodeID, TNodeID> predecessors; // unordered map store the previous node of every node on the shortest path

    // initialize the distance to all nodes are unreachable
    for (std::size_t i = 0; i < streetMap->NodeCount(); ++i) {
        distances[streetMap->NodeByIndex(i)->ID()] = NoPathExists;
    }

    // priority queue, use std::greater<> to make it the smallest heap, which the smalleset number element will be picked.
    std::priority_queue<std::pair<double, TNodeID>, std::vector<std::pair<double, TNodeID>>, std::greater<>> pq;

    // from the source, the source to source distance is 0.
    distances[src] = 0;
    pq.push({ 0, src });

    while (!pq.empty()) {
        auto [dist, nodeID] = pq.top();//return the smallest element in pq as [dist, nodeID]
        pq.pop();

        if (dist > distances[nodeID]) {//if what we found is not the shortest, continue the while
            continue;
        }

        
    }

    return NoPathExists; // 如果没有找到路径
}
}



// Returns the time in hours for the fastest path between the src and dest  
// nodes of the if one exists. NoPathExists is returned if no path exists.  
// The transportation mode and nodes of the fastest path are filled in the  
// path parameter. 
double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) override {
    
    return 0.0; 
}




// Returns true if the path description is created. Takes the trip steps path 
// and converts it into a human readable set of steps.
bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector<TTripStep>& path, std::vector<std::string>& desc) const override {
    
    return false;
}