#include "DijkastraTransportationPlanner.h"
#include "GeographicUtils.h" 
#include "CDijkstraPathRouter.h"
#include "OpenStreetMap.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
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
#include <unordered_set>
#include <any>


// CDijkstraTransportationPlanner member functions
// Constructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) :DImplementation(std::make_unique<SImplementation>()) {
    auto pathRouter = std::make_unique<CDijkstraPathRouter>();
    DImplementation->config = config;
    auto streetMap = config->StreetMap();

    for (std::size_t i = 0; i < streetMap->NodeCount(); ++i) {
        auto node = streetMap->NodeByIndex(i);
        
        auto vertexId = pathRouter->AddVertex(node); 

        
    }

    DImplementation->pathRouter = std::move(pathRouter);
}

// Destructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {
    
}


// Returns the number of nodes in the street map 
std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    
    return DImplementation->config->StreetMap()->NodeCount();
}





/*
std::vector<CStreetMap::TNodeID> GetNeighbors(const COpenStreetMap::SImplementation& mapData, CStreetMap::TNodeID nodeID) {
    std::unordered_set<CStreetMap::TNodeID> neighbors;

    // iterate all paths
    for (const auto& wayPair : mapData.DWayIDToWay) {
        const auto& way = wayPair.second;

        // 查找当前节点在path中的位置
        auto it = std::find(way->Nodes.begin(), way->Nodes.end(), nodeID);
        if (it != way->Nodes.end()) {
            // 如果找到节点，添加前后节点为邻居（如果存在）
            if (it != way->Nodes.begin()) {
                neighbors.insert(*(it - 1));
            }
            if (it + 1 != way->Nodes.end()) {
                neighbors.insert(*(it + 1));
            }
        }
    }

    return std::vector<CStreetMap::TNodeID>(neighbors.begin(), neighbors.end());
}
*/






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
    /*
    auto streetMap = DImplementation->config->StreetMap();


    // unable to reach the destination
    constexpr double NoPathExists = std::numeric_limits<double>::infinity();

    

    std::unordered_map<TNodeID, double> distances; //unordered map store the shortest distance from source node to all other nodes
    std::unordered_map<TNodeID, TNodeID> predecessors; // unordered map store the previous node of every node on the shortest path

    // initialize the distance to all nodes are unreachable
    for (std::size_t i = 0; i < streetMap->NodeCount(); ++i) {
        distances[streetMap->NodeByIndex(i)->ID()] = NoPathExists;
    }
    
    // priority queue, use std::greater<> to make it the smallest heap, which the smalleset number element will be picked, which is the shortest distance node.
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

        // if reach the node we want, which is dest, start to construct path (predecessors)
        if (nodeID == dest) {
            double totalDistance = distances[dest];
            TNodeID current = dest;
            while (current != src) {
                path.insert(path.begin(), current);
                current = predecessors[current];
            }
            path.insert(path.begin(), src); // insert sorce node.
            return totalDistance;
        }
        
        

        
        }
    }
    */

    //use CDijkstraPathRouter to accomplish this function instead of coding Dijkstra directly
    std::vector<TNodeID> temppath; // store the nodes on the path 
    double totalDistance = pathRouter.FindShortestPath(src, dest, temppath); //use CDijkstraPathRouter find the shortest path

    if (totalDistance != CDijkstraPathRouter::NoPathExists) {
        path = temppath; // if found, update it.
        return totalDistance;
    }
    else {
        return CDijkstraPathRouter::NoPathExists; // if not path exists return nopathexists
    }
    return NoPathExists; // if not path exists return nopathexists :(
}




// Returns the time in hours for the fastest path between the src and dest  
// nodes of the if one exists. NoPathExists is returned if no path exists.  
// The transportation mode and nodes of the fastest path are filled in the  
// path parameter. 
double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) override {
    constexpr double NoPathExists = std::numeric_limits<double>::infinity();

    //check if the given src and dest are valid
    auto streetMap = DImplementation->config->StreetMap();
    if (!streetMap->NodeByID(src) || !streetMap->NodeByID(dest)) {
        return NoPathExists;
    }

    //just a psudocode, still need to determine the different method speed
    auto pathRouter = std::make_unique<CDijkstraPathRouter>();
    auto streetMap = DImplementation->config->StreetMap();
    auto busSystem = DImplementation->config->BusSystem();


    //find shortest path via pathrouter
    std::vector<TVertexID> vertexPath;
    double time = pathRouter->FindShortestPath(srcVertexId, destVertexId, vertexPath);
    if (time == CDijkstraPathRouter::NoPathExists) {
        return NoPathExists;
    }


    //Need to wait to see the AddVertex function
    for (auto vertexId : vertexPath) {
        
        auto nodeTag = std::any_cast<TNodeID>(pathRouter->GetVertexTag(vertexId));
        path.push_back({ ETransportationMode::Walk, nodeTag }); 
    }

    return time;

    
}




// Returns true if the path description is created. Takes the trip steps path 
// and converts it into a human readable set of steps.
bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector<TTripStep>& path, std::vector<std::string>& desc) const override {

    if (path.empty()) {
        return false;
    }

    auto streetMap = DImplementation->config->StreetMap();
    std::string lastStreetName;
    ETransportationMode lastMode = ETransportationMode::Walk; // initial mode walk

    for (size_t i = 0; i < path.size(); ++i) {


        const auto& [mode, nodeID] = path[i];
        std::string currentStreetName;
        std::string direction;

        auto node = streetMap->NodeByID(nodeID);
        if (!node) { continue; } // if invalid node, pass



        // get current node streetname
        if (node->HasAttribute("name")) {
            currentStreetName = node->GetAttribute("name");
        }



        // get direction
        if (i > 0) {
            auto prevNode = streetMap->NodeByID(path[i - 1].second);
            if (prevNode && node) {
                direction = SGeographicUtils::BearingToDirection(SGeographicUtils::CalculateBearing(prevNode->Location(), node->Location()));
            }
        }

        // create desc
        std::string stepDesc;
        if (!currentStreetName.empty() && mode != ETransportationMode::Bus) {
            stepDesc = "Walk along " + currentStreetName;
            if (!direction.empty()) {
                stepDesc += " towards " + direction;
            }
        }
        else if (mode == ETransportationMode::Bus && !lastStreetName.empty()) {
            stepDesc = "Take the bus towards " + lastStreetName; 
        }
        else if (!direction.empty()) {
            stepDesc = "Walk toward " + direction;
        }
        else {
            stepDesc = "Continue";
        }

        // update the last mode and street name
        lastStreetName = currentStreetName;
        lastMode = mode;

        
        desc.push_back(stepDesc);
    }

    return true;
}