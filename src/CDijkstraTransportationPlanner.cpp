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


struct CDijkstraTransportationPlanner::SImplementation {
    std::shared_ptr<SConfiguration> config;
    std::unique_ptr<CDijkstraPathRouter> pathRouter; // CDijkstraPathRouter


    SImplementation() : pathRouter(std::make_unique<CDijkstraPathRouter>()) {}
};


// CDijkstraTransportationPlanner member functions
// Constructor for the Dijkstra Transportation Planner 
CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config): DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->config = config; // store the configuration

    auto streetMap = config->StreetMap(); // access the street map from config

    //map NodeIDs to VertexIDs to build edges later
    std::unordered_map<CStreetMap::TNodeID, CPathRouter::TVertexID> nodeToVertexMap;

    // For each node in street map add a vertex to the pathrouter
    for (std::size_t i = 0; i < streetMap->NodeCount(); ++i) {
        auto node = streetMap->NodeByIndex(i);
        auto vertexId = DImplementation->pathRouter->AddVertex(node->ID()); // use nodeID as vertex tag
        nodeToVertexMap[node->ID()] = vertexId;
    }

    //create edges based on the ways in the street map
    for (std::size_t i = 0; i < streetMap->WayCount(); ++i) {
        auto way = streetMap->WayByIndex(i);

        // determine the flag
        bool bikable = way->GetAttribute("bicycle") != "no";
        bool bidirectional = way->GetAttribute("oneway") != "yes";

        // iterate each node in the way to connect them
        for (std::size_t j = 0; j < way->NodeCount() - 1; ++j) {
            auto srcNodeId = way->GetNodeID(j);
            auto destNodeId = way->GetNodeID(j + 1);

            //retrieve correspond vertexID
            auto srcVertexId = nodeToVertexMap[srcNodeId];
            auto destVertexId = nodeToVertexMap[destNodeId];

            // getdistance between nodes as edge weight
            auto srcNode = streetMap->NodeByID(srcNodeId);
            auto destNode = streetMap->NodeByID(destNodeId);
            double distance = GeographicUtils::HaversineDistanceInMiles(srcNode->Location(), destNode->Location());

            double speedLimit;

            if (way->HasAttribute("maxspeed")) {
                //if has maxspeed, use it
                speedLimit = std::stod(way->GetAttribute("maxspeed"));
            }
            else {
                // if not, use default speed
                speedLimit = config->DefaultSpeedLimit();
            }
            double time = distance / speedLimit;

            // add edge to pathrouter
            DImplementation->pathRouter->AddEdge(srcVertexId, destVertexId, time, bidirectional && bikable); 
        }
    }
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

    //convert node ID to vertex ID
    auto srcVertexId = DImplementation->nodeToVertexMap[src];
    auto destVertexId = DImplementation->nodeToVertexMap[dest];


    //create a vector to store vertex id as vertex path
    std::vector<CPathRouter::TVertexID> vertexPath;


    //use CDijkstraPathRouter find the shortest path
    double pathLength = DImplementation->pathRouter->FindShortestPath(srcVertexId, destVertexId, vertexPath);

    // if found path, convert back vertex to node ID
    if (pathLength != CDijkstraPathRouter::NoPathExists) {
        path.clear(); // clear previous path
        for (auto vertexId : vertexPath) {
            if (DImplementation->pathRouter->GetVertexTag(vertexId).has_value()) {
                TNodeID nodeId = std::any_cast<TNodeID>(DImplementation->pathRouter->GetVertexTag(vertexId));
                path.push_back(nodeId);
            }
        }
        return pathLength;
    }
    else {
        // if no return nopathexists
        return CDijkstraPathRouter::NoPathExists;
    }




// Returns the time in hours for the fastest path between the src and dest  
// nodes of the if one exists. NoPathExists is returned if no path exists.  
// The transportation mode and nodes of the fastest path are filled in the  
// path parameter. 
double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) override {

    constexpr double NoPathExists = std::numeric_limits<double>::infinity();

    // clear path result
    path.clear();

    
    std::vector<CPathRouter::TVertexID> walkPathVertexIDs, bikePathVertexIDs;
    double walkTime = DImplementation->DFastestPathRouterWalkBus.FindShortestPath(DImplementation->DNodeToVertexID[src], DImplementation->DNodeToVertexID[dest], walkPathVertexIDs);
    double bikeTime = DImplementation->DFastestPathRouterBike.FindShortestPath(DImplementation->DNodeToVertexID[src], DImplementation->DNodeToVertexID[dest], bikePathVertexIDs);


    // convert distance to hours
    walkTime /= DImplementation->config->WalkSpeed();
    bikeTime /= DImplementation->config->BikeSpeed();

    // choose the fastest path
    double fastestTime = std::min(walkTime, bikeTime);
    ETransportationMode fastestMode = (fastestTime == walkTime) ? ETransportationMode::Walk : ETransportationMode::Bike;

    // fill the final path
    std::vector<CPathRouter::TVertexID>& fastestPathVertexIDs = (fastestTime == walkTime) ? walkPathVertexIDs : bikePathVertexIDs;
    for (auto vertexID : fastestPathVertexIDs) {
        TNodeID nodeID = std::any_cast<TNodeID>(DImplementation->pathRouter->GetVertexTag(vertexID));
        path.push_back({ fastestMode, nodeID });
    }

    return fastestTime;

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

