#include "TransportationPlannerCommandLine.h"
#include <string>
#include <sstream>


struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource;
    std::shared_ptr<CDataSink> OutSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    SImplementation(std::shared_ptr<CDataSource> commandsrc,std::shared_ptr<CDataSink> outsink,std::shared_ptr<CDataSink> errorsink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner): CmdSource(cmdsrc), OutSink(outsink), ErrSink(errsink), Results(results), Planner(planner) {}
};