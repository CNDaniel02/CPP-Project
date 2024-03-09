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



// CDijkstraTransportationPlanner member functions
// Constructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config)
    : DImplementation(std::make_unique<SImplementation>(config)) {
    
}

// Destructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {
    
}


// Returns the number of nodes in the street map 
std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    
    return 0;
}



// Returns the street map node specified by index if index is less than the  
// NodeCount(). nullptr is returned if index is greater than or equal to  
// NodeCount(). The nodes are sorted by Node ID. 
std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    
    return DImplementation->sortedNodes[index];
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