CTransportationPlannerCommandLine.md Overview
CTransportationPlannerCommandLine provides a command-line interface to
interact with the transportation planner, processing commands, and
returning outputs.

Dependencies

DataFactory.h TransportationPlanner.h

Initialization CTransportationPlannerCommandLine CommandLine(cmdsrc,
outsink, errsink, results, planner);

Constructor and Destructor The constructor sets up the command line
interface with the necessary data sources, sinks, and the transportation
planner. The destructor cleans up the implementation.

Member Functions

ProcessCommands reads commands from the command source, processes them,
and writes outputs or errors.
