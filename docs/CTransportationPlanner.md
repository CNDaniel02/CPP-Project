CTransportationPlanner.md Overview CTransportationPlanner is a C++
interface for transportation planning involving calculating paths for
different transportation modes within a map.

Dependencies

StreetMap.h BusSystem.h PathRouter.h Initialization

Member Functions

NodeCount returns the number of nodes in the transportation network.
SortedNodeByIndex retrieves a node sorted by index. FindShortestPath
computes the shortest path between two nodes. FindFastestPath computes
the fastest path between two nodes based on transportation modes.
GetPathDescription provides a human-readable description of a path.
