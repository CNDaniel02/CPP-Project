BusSystemIndexer Documentation

Overview
BusSystemIndexer is a C++ class used to index and manage bus system data. This class can be used to sort stops and routes, and to find routes between stops.

Dependencies
Standard library components, such as<vector> and <algorithm>

Initialization
CBusSystemIndexer Indexer(std::make_shared<CBusSystem>());

Constructor
CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

Destructor
CBusSystemIndexer::~CBusSystemIndexer(){
}

Member Functions
std::size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}
...

These functions are used to get the number of stops, get the number of routes, get a sorted stop by index, get a sorted route by index, get a stop by node ID, get routes by node IDs, and check if a route exists between node IDs respectively.