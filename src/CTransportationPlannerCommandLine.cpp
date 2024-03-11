#include "TransportationPlannerCommandLine.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>


struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource;
    std::shared_ptr<CDataSink> OutSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    SImplementation(std::shared_ptr<CDataSource> commandsrc,std::shared_ptr<CDataSink> outsink,std::shared_ptr<CDataSink> errorsink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner): CmdSource(cmdsrc), OutSink(outsink), ErrSink(errsink), Results(results), Planner(planner) {}
};


CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> commandsrc,std::shared_ptr<CDataSink> outsink,std::shared_ptr<CDataSink> errorsink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner): DImplementation(std::make_unique<SImplementation>(cmdsrc, outsink, errsink, results, planner)) {}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}


bool CTransportationPlannerCommandLine::ProcessCommands() {
    char ch;
    std::string commandLine;
    // read command
    while (!DImplementation->CmdSource->End()) {
        if (DImplementation->CmdSource->Get(ch)) {
            if (ch == '\n') { // if command ends
                // execute command
                std::istringstream iss(commandLine);
                std::string command;
                iss >> command; //read command

                if (command == "quit") {
                    return false; // stop executing command
                }

                


                ///

                commandLine.clear(); // clear command line and ready to read next command
            }
            else {
                commandLine.push_back(ch); //construct command line
            }
        }
    }
    return true;
}