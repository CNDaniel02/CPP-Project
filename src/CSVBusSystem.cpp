#include "BusSystem.h"
#include "StreetMap.h"
#include "DSVReader.h"
#include "CSVBusSystem.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>


using TStopID = uint64_t;
using TNodeID = uint64_t;
using TWayID = uint64_t;
using TLocation = std::pair<double, double>;


class CStop : public CBusSystem::SStop {
    
    TStopID sID;
    CStreetMap::TNodeID sNodeID;

public:
    CStop(TStopID id, CStreetMap::TNodeID nodeId) : sID(id), sNodeID(nodeId) {}




    //Bus System Stop member functions Returns the stop id of the stop 
    TStopID ID() const noexcept override {
        return sID;
    }



    //Returns the node id of the bus stop 
    CStreetMap::TNodeID NodeID() const noexcept override {
        return sNodeID;
    }
};




class CRoute : public CBusSystem::SRoute {
    std::string rName;
    std::vector<TStopID> StopIDvec;

public:
    

    void AddStopID(TStopID id) {
        StopIDvec.push_back(id);
    }


    CRoute(const std::string& name, const std::vector<TStopID>& stid): rName(name), StopIDvec(stid) {}


    //Bus System Route member functions Returns the name of the route 
    std::string Name() const noexcept override {
        return rName;
    }


    //Returns the number of stops on the route
    std::size_t StopCount() const noexcept override {
        return StopIDvec.size();
    }



    //Returns the stop id specified by the index, returns InvalidStopID if index is greater than or equal to StopCount()
    TStopID GetStopID(std::size_t index) const noexcept override {
        if (index < StopIDvec.size()) {
            return StopIDvec[index];
        }
        return CBusSystem::InvalidStopID;
    }
};

struct CCSVBusSystem::SImplementation {
    
    std::vector<std::shared_ptr<CBusSystem::SStop>> Stops;
    std::vector<std::shared_ptr<CBusSystem::SRoute>> Routes;
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<CBusSystem::SStop>> StopsByID;
    std::unordered_map<std::string, std::shared_ptr<CBusSystem::SRoute>> RoutesByName;
};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopSrc, std::shared_ptr<CDSVReader> routeSrc)
    : DImplementation(std::make_unique<SImplementation>()) {
    

    std::vector<std::string> row;
    
    if (!stopSrc->End()) {//跳过标题
        stopSrc->ReadRow(row);
        row.clear();//Used 3 hours to find out this will solve the stoull error and segmentation error
        //My not perfect DSVReader.cpp might be the cause for have to put row.clear() here.
        while (!stopSrc->End()) {
            
            if (stopSrc->ReadRow(row) && row.size() >= 2) {
                
                try {//add try catch block for invalid input
                    //std::cout << row[0] << "  " << row[1] << std::endl;  //added for debugging
                    CBusSystem::TStopID id = std::stoull(row[0]);
                    CStreetMap::TNodeID nodeID = std::stoull(row[1]);
                    auto stop = std::make_shared<CStop>(id, nodeID);
                    DImplementation->Stops.push_back(stop);
                    DImplementation->StopsByID[id] = stop;
                    row.clear();
                }
                catch (const std::exception&) {
                    continue;
                }
            }
        }
    }
    // route

    if (!routeSrc->End()) {
        routeSrc->ReadRow(row);
        row.clear();
        while (!routeSrc->End()) {
            
            
            

            if (routeSrc->ReadRow(row) && row.size() >= 2) {
                try {
                    std::string name = row[0];
                    //std::cout << row[0] << std::endl;
                    //std::cout << row[0]<<""<<row[1] << "" << std::endl;
                    CBusSystem::TStopID stopID = std::stoull(row[1]);
                    //std::cout << row[0] << " " << row[1] << "\n" << std::endl;
                    auto it = DImplementation->RoutesByName.find(name);
                    if (it == DImplementation->RoutesByName.end()) {
                        auto route = std::make_shared<CRoute>(name, std::vector<CBusSystem::TStopID>{stopID});
                        DImplementation->Routes.push_back(route);
                        DImplementation->RoutesByName[name] = route;

                    }
                    else {
                        std::dynamic_pointer_cast<CRoute>(it->second)->AddStopID(stopID);
                    }
                    row.clear();
                }
                catch (const std::exception&) {
                    continue;
                }
            }
        }
    }

    // stop

    
    /*
    while (!stopSrc->End()) {
        if (stopSrc->ReadRow(row)) {
            CBusSystem::TStopID id = std::stoull(row[0]);
            CStreetMap::TNodeID nodeID = std::stoull(row[1]);
            auto stop = std::make_shared<CStop>(id, nodeID);
            DImplementation->Stops.push_back(stop);
            DImplementation->StopsByID[id] = stop;
        }
    }
    */

    /*
    while (!stopSrc->End()) {
        if (stopSrc->ReadRow(row) && row.size() >= 2) {
            try {
                CBusSystem::TStopID id = std::stoull(row[0]);
                CStreetMap::TNodeID nodeID = std::stoull(row[1]);
                auto stop = std::make_shared<CStop>(id, nodeID);
                DImplementation->Stops.push_back(stop);
                DImplementation->StopsByID[id] = stop;
            }
            catch (const std::exception& e) {
                std::cerr << "Error converting stop data: " << e.what() << std::endl;
                // Handle the error or skip this row
            }
        }
    }
    // route

    while (!routeSrc->End()) {
        if (routeSrc->ReadRow(row)) {
            std::string name = row[0];
            CBusSystem::TStopID stopID = std::stoull(row[1]);
            auto it = DImplementation->RoutesByName.find(name);
            if (it == DImplementation->RoutesByName.end()) {
                auto route = std::make_shared<CRoute>(name, std::vector<CBusSystem::TStopID>{stopID});
                DImplementation->Routes.push_back(route);
                DImplementation->RoutesByName[name] = route;
            } else {
                std::dynamic_pointer_cast<CRoute>(it->second)->AddStopID(stopID);
            }
        }
    }
    *///第二版


    /*
    while (!stopSrc->End()) {
        if (stopSrc->ReadRow(row)) {
            std::istringstream iss_id(row[0]);
            CBusSystem::TStopID id;
            if (!(iss_id >> id)) {
                std::cerr << "Failed to convert '" << row[0] << "' to TStopID number." << std::endl;
                continue; // 跳过当前行
            }

            std::istringstream iss_nodeID(row[1]);
            CStreetMap::TNodeID nodeID;
            if (!(iss_nodeID >> nodeID)) {
                std::cerr << "Failed to convert '" << row[1] << "' to TNodeID number." << std::endl;
                continue; // 跳过当前行
            }

            auto stop = std::make_shared<CStop>(id, nodeID);
            DImplementation->Stops.push_back(stop);
            DImplementation->StopsByID[id] = stop;
        }
    }

    // 处理路线数据
    while (!routeSrc->End()) {
        if (routeSrc->ReadRow(row)) {
            std::string name = row[0];
            std::istringstream iss_stopID(row[1]);
            CBusSystem::TStopID stopID;
            if (!(iss_stopID >> stopID)) {
                std::cerr << "Failed to convert '" << row[1] << "' to TStopID number." << std::endl;
                continue; // 跳过当前行
            }

            auto it = DImplementation->RoutesByName.find(name);
            if (it == DImplementation->RoutesByName.end()) {
                auto route = std::make_shared<CRoute>(name, std::vector<CBusSystem::TStopID>{stopID});
                DImplementation->Routes.push_back(route);
                DImplementation->RoutesByName[name] = route;
            }
            else {
                std::dynamic_pointer_cast<CRoute>(it->second)->AddStopID(stopID);
            }
        }
    }
    *///第三版debug

    



}

// Destructor implementation
CCSVBusSystem::~CCSVBusSystem() {
    
}

// Returns the number of stops in the system 
std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->Stops.size();
}


// Returns the number of routes in the system
std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->Routes.size();
}


// Returns the SStop specified by the index, nullptr is returned if index is greater than equal to StopCount() 
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    if (index >= DImplementation->Stops.size()) {
        return nullptr;
    }
    return DImplementation->Stops[index];
}


//Returns the SStop specified by the stop id, nullptr is returned if id is not in the stops 
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    auto it = DImplementation->StopsByID.find(id);
    if (it != DImplementation->StopsByID.end()) {
        return it->second;
    }
    return nullptr;
}


//Returns the SRoute specified by the index, nullptr is returned if index is greater than equal to RouteCount() 
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    if (index >= DImplementation->Routes.size()) {
        return nullptr;
    }
    return DImplementation->Routes[index];
}


//Returns the SRoute specified by the name, nullptr is returned if name is not in the routes 
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string& name) const noexcept {
    auto it = DImplementation->RoutesByName.find(name);
    if (it != DImplementation->RoutesByName.end()) {
        return it->second;
    }
    return nullptr;
}