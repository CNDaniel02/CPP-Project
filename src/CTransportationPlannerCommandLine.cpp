#include "TransportationPlannerCommandLine.h"
#include "GeographicUtils.h"
#include "TransportationPlanner.h"


#include "DijkstraTransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "OpenStreetMap.h"
#include "TransportationPlannerConfig.h"
#include "BusSystem.h"
#include "StreetMap.h"
#include "DSVReader.h"
#include "CSVBusSystem.h"



#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

using TNodeID = CStreetMap::TNodeID;

static constexpr double NoPathExists = std::numeric_limits<double>::max(); //Define a constant for indicating that no path exists


// Implementation details for the CTransportationPlannerCommandLine class
struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource;
    std::shared_ptr<CDataSink> OutSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    // Constructor initializing all data members
    SImplementation(std::shared_ptr<CDataSource> commandsrc, std::shared_ptr<CDataSink> outsink, std::shared_ptr<CDataSink> errorsink, std::shared_ptr<CDataFactory> results, std::shared_ptr<CTransportationPlanner> planner) : CommandSource(commandsrc), OutSink(outsink), ErrorSink(errorsink), Results(results), Planner(planner) {}

};

// Constructor
CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> commandsrc,std::shared_ptr<CDataSink> outsink,std::shared_ptr<CDataSink> ErrorSink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner): DImplementation(std::make_unique<SImplementation>(commandsrc, outsink, ErrorSink, results, planner)) {}
// Destructor
CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}

// Help text
const std::string helpText =
"------------------------------------------------------------------------\n"
"help     Display this help menu\n"
"exit     Exit the program\n"
"count    Output the number of nodes in the map\n"
"node     Syntax \"node [0, count)\"\n"
"         Will output node ID and Lat/Lon for node\n"
"fastest  Syntax \"fastest start end\"\n"
"         Calculates the time for fastest path from start to end\n"
"shortest Syntax \"shortest start end\"\n"
"         Calculates the distance for the shortest path from start to end\n"
"save     Saves the last calculated path to file\n"
"print    Prints the steps for the last calculated path\n";


// Function to process user commands
bool CTransportationPlannerCommandLine::ProcessCommands() {
    std::string line;
    char ch;

    while (!DImplementation->CommandSource->End()) {
        DImplementation->OutSink->Put('>'); // Prompt for input
        DImplementation->OutSink->Put(' ');
        line.clear();

        // Read a line of input
        while (DImplementation->CommandSource->Get(ch) && ch != '\n') {
            line += ch;
        }

        std::istringstream stream(line);
        std::string command;
        stream >> command;

        if (command == "exit") {
            break; // Exit command
        }
        else if (command == "help") {
            // Print help information
            DImplementation->OutSink->Write(std::vector<char>(helpText.begin(), helpText.end()));
        }
        else if (command == "count") {
            // Output the number of nodes
            auto nodeCount = DImplementation->Planner->NodeCount();
            std::string countMessage = std::to_string(nodeCount) + " nodes\n";
            DImplementation->OutSink->Write(std::vector<char>(countMessage.begin(), countMessage.end()));
        }
        else if (command == "node") {
            size_t index;
            if (stream >> index) {
                auto node = DImplementation->Planner->SortedNodeByIndex(index);
                if (node) {
                    std::ostringstream os;
                    os << "Node " << index << ": id = " << node->ID() << " is at ";
                    auto loc = node->Location();
                    os << SGeographicUtils::ConvertLLToDMS(loc) << '\n';
                    DImplementation->OutSink->Write(std::vector<char>(os.str().begin(), os.str().end()));
                }
                else {
                    std::string erro = "Invalid node parameter, see help.\n";
                    DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
                }
            }
            else {
                std::string erro = "Invalid node command, see help.\n";
                DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
            }
        }
        else if (command == "shortest") {
            TNodeID src, dest;
            if (stream >> src >> dest) {
                std::vector<CTransportationPlanner::TNodeID> path;
                double distance = DImplementation->Planner->FindShortestPath(src, dest, path);//调用FindShortestPath函数
                if (distance != NoPathExists) {
                    std::ostringstream oss;
                    oss << "Shortest path is " << distance << " mi.\n";
                    DImplementation->OutSink->Write(std::vector<char>(oss.str().begin(), oss.str().end()));
                }
                else {
                    std::string erro = "Invalid shortest parameter, see help.\n";
                    DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
                }
            }
            else {
                std::string erro = "Invalid shortest command, see help.\n\n";
                DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
            }
        }

        else if (command == "fastest") {
            TNodeID src, dest;
            if (stream >> src >> dest) { 
                std::vector<CTransportationPlanner::TTripStep> path;
                double time = DImplementation->Planner->FindFastestPath(src, dest, path);//调用FindFastestPath函数
                if (time != NoPathExists) {
                    int hours = static_cast<int>(time);
                    int minutes = static_cast<int>((time - hours) * 60);
                    std::ostringstream oss;
                    oss << "Fastest path takes " << hours << " hour(s) and " << minutes << " minute(s).\n";
                    DImplementation->OutSink->Write(std::vector<char>(oss.str().begin(), oss.str().end()));
                }
                else {
                    std::string erro = "Invalid fastest parameter, see help.\n";
                    DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
                }
            }
            else {
                std::string erro = "Invalid fastest command, see help.\n";
                DImplementation->ErrorSink->Write(std::vector<char>(erro.begin(), erro.end()));
            }
        }
        else if (command == "save") {//This part is not finished
            std::string errorMessage = "Error: No path to save.\n";
            DImplementation->ErrorSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
        }
        else if (command == "print") {//This part is not finished
            std::string errorMessage = "Error: No path description to print.\n";
            DImplementation->ErrorSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
        }
        else {
            std::string errorMessage = "Unknown command: " + command + " type help for help.\n";
            DImplementation->ErrorSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
        }
        DImplementation->OutSink->Put('>'); // Prompt for input
        DImplementation->OutSink->Put(' ');
    }

    return true;
}