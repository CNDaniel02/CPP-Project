#include "OpenStreetMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct COpenStreetMap::SImplementation {
    std::shared_ptr<CXMLReader> src;

    struct SNode : public CStreetMap::SNode {
        TNodeID DID;
        TLocation DLocation;
        std::unordered_map<std::string, std::string> DAttributes;
        std::vector<std::string> DAttributeKeys;

        SNode(TNodeID id, TLocation loc) {
            DID = id;
            DLocation = loc;
        }

        ~SNode() = default;

        TNodeID ID() const noexcept override {
            return DID;
        }

        TLocation Location() const noexcept override {
            return DLocation;
        }

        std::size_t AttributeCount() const noexcept override {
            return DAttributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override {
            if (index < DAttributeKeys.size()) {
                return DAttributeKeys[index];
            }
            return std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept override {
            auto Search = DAttributes.find(key);
            return DAttributes.end() != Search;
        }//determine if the attribute exists

        std::string GetAttribute(const std::string &key) const noexcept override {
            auto Search = DAttributes.find(key);
            if (DAttributes.end() != Search) {
                return Search->second;
            }
            return std::string();
        }//get the key of attribute at index. return empty string if index is out of range

        void SetAttribute(const std::string &key, const std::string &value) {
            DAttributeKeys.push_back(key);
            DAttributes[key] = value;
        }
    };

    struct SWay : public CStreetMap::SWay {
        TNodeID DID;
        TWayID WID;
        std::unordered_map<std::string, std::string> DAttributes;
        std::vector<std::string> DAttributeKeys;
        std::vector<TNodeID> Nodes;

        SWay(TNodeID nid, TWayID wayid) {
            DID = nid;
            WID = wayid;
        }

        ~SWay() = default;

        TWayID ID() const noexcept override {
            return WID;
        }

        std::size_t NodeCount() const noexcept override {
            return Nodes.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept override {
            if (index < Nodes.size()) {
                return Nodes[index];
            }
            return InvalidNodeID;
        }

        std::size_t AttributeCount() const noexcept override {
            return DAttributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override {
            if (index < DAttributeKeys.size()) {
                return DAttributeKeys[index];
            }
            return std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept override {
            auto Search = DAttributes.find(key);
            return DAttributes.end() != Search;
        }

        std::string GetAttribute(const std::string &key) const noexcept override {
            auto Search = DAttributes.find(key);
            if (DAttributes.end() != Search) {
                return Search->second;
            }
            return std::string();
        }
        void SetAttribute(const std::string& key, const std::string& value) {
            DAttributes[key] = value;
        }
    };

    std::unordered_map<TNodeID, std::shared_ptr<CStreetMap::SNode>> DNodeIDToNode;
    std::vector< std::shared_ptr<CStreetMap::SNode>> DNodesByIndex;
    std::unordered_map<TWayID, std::shared_ptr<CStreetMap::SWay>> DWayIDToWay;
    std::vector< std::shared_ptr<CStreetMap::SWay>> DWayByIndex;

    SImplementation(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;

        while (src->ReadEntity(TempEntity, true)) {
            if ((TempEntity.DNameData == "osm") && (SXMLEntity::EType::EndElement == TempEntity.DType)) {
                break;
            }//if the entity is the end of the file, break the loop
            else if ((TempEntity.DNameData == "node") && (SXMLEntity::EType::StartElement == TempEntity.DType)) {
                std::cout << "node" << std::endl;
                TNodeID NewNodeID = std::stoull(TempEntity.AttributeValue("id"));
                double Lat = std::stod(TempEntity.AttributeValue("lat"));
                double Lon = std::stod(TempEntity.AttributeValue("lon"));
                TLocation NewNodeLocation = std::make_pair(Lat, Lon);
                auto NewNode = std::make_shared<SNode>(NewNodeID, NewNodeLocation);
                DNodesByIndex.push_back(NewNode);
                DNodeIDToNode[NewNodeID] = NewNode;
                while (src->ReadEntity(TempEntity, true)) {
                    if ((TempEntity.DNameData == "node") && (SXMLEntity::EType::EndElement == TempEntity.DType)) {
                        break;
                    }
                    else if ((TempEntity.DNameData == "tag") && (SXMLEntity::EType::StartElement == TempEntity.DType)) {
                        NewNode->SetAttribute(TempEntity.AttributeValue("k"), TempEntity.AttributeValue("v"));
                    }
                }
            }//if the entity is a node, record the node's id, location and attributes
            else if((TempEntity.DNameData == "way")&&(SXMLEntity::EType::StartElement == TempEntity.DType)){
                TWayID NewWayID = std::stoull(TempEntity.AttributeValue("id"));
                TNodeID NewNodeID = 0;
                auto NewWay = std::make_shared<SWay>(NewNodeID, NewWayID);
                DWayByIndex.push_back(NewWay);
                DWayIDToWay[NewWayID] = NewWay;
                while (src->ReadEntity(TempEntity, true)) {
                    if ((TempEntity.DNameData == "way") && (SXMLEntity::EType::EndElement == TempEntity.DType)) {
                        break;
                    }
                    else if ((TempEntity.DNameData == "nd") && (SXMLEntity::EType::StartElement == TempEntity.DType)) {
                        NewNodeID = std::stoull(TempEntity.AttributeValue("ref"));
                        NewWay->Nodes.push_back(NewNodeID);
                    }
                    else if ((TempEntity.DNameData == "tag") && (SXMLEntity::EType::StartElement == TempEntity.DType)) {
                        NewWay->SetAttribute(TempEntity.AttributeValue("k"), TempEntity.AttributeValue("v"));
                    }
                }
            }//if the entity is a way, record the way's id, nd and tags
        }
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) : DImplementation(std::make_unique<SImplementation>(std::move(src))) {}

COpenStreetMap::~COpenStreetMap() = default;

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->DNodesByIndex.size();
}//return the number of nodes

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    if (index >= DImplementation->DNodesByIndex.size()) {
        return nullptr;
    }
    return DImplementation->DNodesByIndex[index];
}//return the node with the given index

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    auto Search = DImplementation->DNodeIDToNode.find(id);
    if (Search == DImplementation->DNodeIDToNode.end()) {
        return nullptr;
    }
    return Search->second;
}//return the node with the given id

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    if (index >= DImplementation->DWayByIndex.size()) {
        return nullptr;
    }
    return DImplementation->DWayByIndex[index];
}//return the way with the given index

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    auto Search = DImplementation->DWayIDToWay.find(id);
    if (Search == DImplementation->DWayIDToWay.end()) {
        return nullptr;
    }
    return Search->second;
}//return the way with the given id

std::size_t COpenStreetMap::WayCount() const noexcept {
    return DImplementation->DWayByIndex.size();
}//return the number of ways