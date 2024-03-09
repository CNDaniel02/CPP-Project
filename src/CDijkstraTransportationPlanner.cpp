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
double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID>& path) override {
    
    return 0.0; 
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