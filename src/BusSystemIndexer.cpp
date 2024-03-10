#include "BusSystemIndexer.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>

struct CBusSystemIndexer::SImplementation{
    struct pair_hash{
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const{
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    
    struct RouteCompare{
        bool operator()(std::shared_ptr<SRoute> left, std::shared_ptr<SRoute> right){
            return left->Name() < right->Name();
        }
    };
    
    
    std::shared_ptr<CBusSystem> DBusSystem;
    std::vector< std::shared_ptr<SStop> >DSortedStops;
    std::vector <std::shared_ptr<SRoute>> DSortedRoutes;
    std::unordered_map< TNodeID, std::shared_ptr<SStop> > DNodeIDToStop;
    std::unordered_map< std::pair<TNodeID, TNodeID>, std::unordered_set<std::shared_ptr<SRoute> >, pair_hash > DSrcDestToRoutes;

    static bool StopIDCompare(std::shared_ptr<SStop> left, std::shared_ptr<SStop> right){
        return left->ID() < right->ID();
    }

    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = bussystem;
        for (size_t Index = 0; Index < DBusSystem->StopCount(); Index++){
            auto CurrentStop = DBusSystem->StopByIndex(Index);
            DSortedStops.push_back(CurrentStop);
            DNodeIDToStop[CurrentStop->NodeID()] = CurrentStop;
        }//sort the stops by ID
        std::sort(DSortedStops.begin(), DSortedStops.end(), StopIDCompare);
        for (size_t Index = 0; Index < DBusSystem->RouteCount(); Index++){
            auto CurrentRoute = DBusSystem->RouteByIndex(Index);
            DSortedRoutes.push_back(CurrentRoute);
            for (size_t StopIndex = 1; StopIndex < CurrentRoute->StopCount(); StopIndex++){
                auto SourceNodeID = DBusSystem->StopByID(CurrentRoute->GetStopID(StopIndex-1))->NodeID();
                auto DestinationNodeID = DBusSystem->StopByID(CurrentRoute->GetStopID(StopIndex))->NodeID();
                auto SearchKey = std::make_pair(SourceNodeID, DestinationNodeID);
                auto Search = DSrcDestToRoutes.find(SearchKey);
                if (Search != DSrcDestToRoutes.end()){
                    Search->second.insert(CurrentRoute);
                }
                else{
                    DSrcDestToRoutes[SearchKey] = {CurrentRoute};
                }
        }
    }
    std::sort(DSortedRoutes.begin(), DSortedRoutes.end(), RouteCompare());//sort the routes by name
    }

    std::size_t StopCount() const noexcept{
        return DBusSystem->StopCount();
    }//return the number of stops

    std::size_t RouteCount() const noexcept{
        return DBusSystem->RouteCount();
    }//return the number of routes

    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept{
        if (index < DSortedStops.size()){
            return DSortedStops[index];
        }//return the stop at the given index
        return nullptr;//return nullptr if the index is out of range
    }

    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept{
        if (index < DSortedRoutes.size()){
            //std::cout << "Index: " << index << std::endl;
            //std::cout << "RouteCount: " << DBusSystem->RouteCount() << std::endl;
            //std::cout << "Route: " << DBusSystem->RouteByIndex(index)->Name() << std::endl;
            return DSortedRoutes[index];
        }
        //std::cout << "Index out of range" << std::endl;
        return nullptr;
    }//return the route at the given index

    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept{
        auto Search = DNodeIDToStop.find(id);//search for the stop with the given ID
        if (Search != DNodeIDToStop.end()){
            return Search->second;
        }//return the stop if found
        return nullptr;//return nullptr if not found
    }

    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
        auto SearchKey = std::make_pair(src, dest);
        auto Search = DSrcDestToRoutes.find(SearchKey);//search for the routes between the given source and destination
        if (Search != DSrcDestToRoutes.end()){
            routes = Search->second;
            return true;
        }//return the routes if found
        return false;//return false if not found
    }

    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
        auto SearchKey = std::make_pair(src, dest);
        auto Search = DSrcDestToRoutes.find(SearchKey);
        return Search!=DSrcDestToRoutes.end();
    }//return true if there is a route between the given source and destination
};


CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CBusSystemIndexer::~CBusSystemIndexer(){
}

std::size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedStopByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedRouteByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept{
    return DImplementation->StopByNodeID(id);
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
    return DImplementation->RoutesByNodeIDs(src, dest, routes);
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
    return DImplementation->RouteBetweenNodeIDs(src, dest);
}
