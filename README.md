# Project 4 README

Student 1: Dongmin Wu (918110396)

Student 2: Yingchun Yuan (919552555)

## Project Status
project 3 bugs were fixed
PathRouter and Indexer are working properly
Planner and PlannerCommand can't be tested since we are stucked with some issue with makefile. Couln't go any farther. 

the makefile without linkingplanner and plannercommand is working fine.

## Known Issues
make
mkdir -p ./obj
mkdir -p ./bin
g++ -std=c++17 -I./include -I/usr/local/include -c src/CDijkstraTransportationPlanner.cpp -o obj/CDijkstraTransportationPlanner.o
g++ obj/CSVOSMTransportationPlannerTest.o obj/CDijkstraTransportationPlanner.o obj/GeographicUtils.o obj/StringDataSource.o obj/StringDataSink.o obj/XMLReader.o obj/DSVReader.o obj/OpenStreetMap.o obj/CSVBusSystem.o -o bin/testtp -lgtest -lgtest_main -lgmock -lpthread -lexpat
/usr/bin/ld: obj/CDijkstraTransportationPlanner.o: in function `std::_MakeUniq<CDijkstraPathRouter>::__single_object std::make_unique<CDijkstraPathRouter>()':
CDijkstraTransportationPlanner.cpp:(.text._ZSt11make_uniqueI19CDijkstraPathRouterJEENSt9_MakeUniqIT_E15__single_objectEDpOT0_[_ZSt11make_uniqueI19CDijkstraPathRouterJEENSt9_MakeUniqIT_E15__single_objectEDpOT0_]+0x26): undefined reference to `CDijkstraPathRouter::CDijkstraPathRouter()'
collect2: error: ld returned 1 exit status
make: *** [Makefile:94: bin/testtp] Error 1

## Code References
pathrouter, indexer, pathrouter test, pathrouter.md, indexer.md, compiling without linking planner and planner command makefile are done by Yingchun

## Generative AI Use
I used ChatGPT for Prompt 1

### Prompt 1
Need AI to help me fix my make file and give me advice on FindFastestPath function 

### Response 1
CC=gcc
CXX=g++
OBJ_DIR = ./obj
SRC_DIR = ./src
BIN_DIR = ./bin
INC_DIR = ./include
TEST_SRC_DIR = ./testsrc
CXXFLAGS = -std=c++17 -I$(INC_DIR)
LDFLAGS = -lgtest -lgtest_main -lpthread -lexpat


all: directories runtests

runtests:  $(BIN_DIR)/testdpr $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testosm   #$(BIN_DIR)/testcsvbs 
	@echo "dpr tests..."
	@$(BIN_DIR)/testdpr
	@echo "bsi tests..."
	@$(BIN_DIR)/testcsvbsi
	$(BIN_DIR)/testosm
	$(BIN_DIR)/testtpcl $(BIN_DIR)/testtp
	@echo "Running TP CommandLine tests..."
	@$(BIN_DIR)/testtpcl
	@echo "Running Transportation Planner tests..."
	@$(BIN_DIR)/testtp




$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) $^ -o $@ $(LDFLAGS)


$(BIN_DIR)/testosm: $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) $^ -o $@ $(LDFLAGS)


$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouterTest.o $(OBJ_DIR)/DijkstraPathRouter.o 
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/BusSystemIndexer.o
	$(CXX) $^ -o $@ $(LDFLAGS)



$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)


$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/OSMTest.o: $(TEST_SRC_DIR)/OSMTest.cpp $(INC_DIR)/OpenStreetMap.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/OpenStreetMap.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DijkstraPathRouterTest.o: $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/PathRouter.o: $(SRC_DIR)/PathRouter.cpp $(INC_DIR)/PathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/DSVReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/DSVReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o
	$(CXX) -o $(BIN_DIR)/testcsvbsi $(CXXFLAGS) $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(LDFLAGS)

$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/BusSystem.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/BusSystemIndexer.o -c $(CXXFLAGS) $(SRC_DIR)/BusSystemIndexer.cpp

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/StreetMap.h $(INC_DIR)/DSVReader.h $(INC_DIR)/XMLReader.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp

$(OBJ_DIR)/CDijkstraTransportationPlanner.o: $(SRC_DIR)/CDijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp $(INC_DIR)/GeographicUtils.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/TransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/TPCommandLineTest.o: $(TEST_SRC_DIR)/TPCommandLineTest.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o $(LDFLAGS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/testtp: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/GeographicUtils.o $(LDFLAGS)
	$(CXX) $^ -o $@ $(LDFLAGS)


clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

这是我现在的makefile

以下是报错
make
Makefile:83: warning: overriding recipe for target 'obj/StringDataSource.o'
Makefile:46: warning: ignoring old recipe for target 'obj/StringDataSource.o'
Makefile:86: warning: overriding recipe for target 'bin/testcsvbsi'
Makefile:41: warning: ignoring old recipe for target 'bin/testcsvbsi'
Makefile:89: warning: overriding recipe for target 'obj/BusSystemIndexer.o'
Makefile:80: warning: ignoring old recipe for target 'obj/BusSystemIndexer.o'
Makefile:92: warning: overriding recipe for target 'obj/CSVBusSystemIndexerTest.o'
Makefile:77: warning: ignoring old recipe for target 'obj/CSVBusSystemIndexerTest.o'
mkdir -p ./obj
mkdir -p ./bin
g++ -c testsrc/DijkstraPathRouterTest.cpp -o obj/DijkstraPathRouterTest.o -std=c++17 -I./include
g++ -c src/DijkstraPathRouter.cpp -o obj/DijkstraPathRouter.o -std=c++17 -I./include
g++ obj/DijkstraPathRouterTest.o obj/DijkstraPathRouter.o -o bin/testdpr -lgtest -lgtest_main -lpthread -lexpat
g++ -o ./obj/BusSystemIndexer.o -c -std=c++17 -I./include ./src/BusSystemIndexer.cpp
g++ -c src/DSVReader.cpp -o obj/DSVReader.o -std=c++17 -I./include
g++ -c src/XMLReader.cpp -o obj/XMLReader.o -std=c++17 -I./include
g++ -c src/StringDataSource.cpp -o obj/StringDataSource.o -std=c++17 -I./include
g++ -c src/CSVBusSystem.cpp -o obj/CSVBusSystem.o -std=c++17 -I./include
g++ -o ./obj/CSVBusSystemIndexerTest.o -c -std=c++17 -I./include ./testsrc/CSVBusSystemIndexerTest.cpp
g++ -o ./bin/testcsvbsi -std=c++17 -I./include ./obj/BusSystemIndexer.o ./obj/DSVReader.o ./obj/XMLReader.o ./obj/StringDataSource.o ./obj/CSVBusSystem.o ./obj/CSVBusSystemIndexerTest.o -lgtest -lgtest_main -lpthread -lexpat
g++ -c testsrc/OSMTest.cpp -o obj/OSMTest.o -std=c++17 -I./include
g++ -c src/OpenStreetMap.cpp -o obj/OpenStreetMap.o -std=c++17 -I./include
g++ obj/OSMTest.o obj/OpenStreetMap.o obj/XMLReader.o obj/StringDataSource.o -o bin/testosm -lgtest -lgtest_main -lpthread -lexpat
dpr tests...
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from DijkstraPathRouter
[ RUN      ] DijkstraPathRouter.FindShortestPath
[       OK ] DijkstraPathRouter.FindShortestPath (0 ms)
[ RUN      ] DijkstraPathRouter.AddVertex
[       OK ] DijkstraPathRouter.AddVertex (0 ms)
[ RUN      ] DijkstraPathRouter.GetVertexTag
[       OK ] DijkstraPathRouter.GetVertexTag (0 ms)
[ RUN      ] DijkstraPathRouter.NoPathExists
[       OK ] DijkstraPathRouter.NoPathExists (0 ms)
[----------] 4 tests from DijkstraPathRouter (0 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 4 tests.
bsi tests...
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from CSVBusSystemIndexer
[ RUN      ] CSVBusSystemIndexer.SimpleTest
[       OK ] CSVBusSystemIndexer.SimpleTest (0 ms)
[ RUN      ] CSVBusSystemIndexer.StopTest
[       OK ] CSVBusSystemIndexer.StopTest (0 ms)
[ RUN      ] CSVBusSystemIndexer.RouteTest
[       OK ] CSVBusSystemIndexer.RouteTest (0 ms)
[----------] 3 tests from CSVBusSystemIndexer (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 3 tests.
./bin/testosm
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from OSMTest
[ RUN      ] OSMTest.SimpleTest
[       OK ] OSMTest.SimpleTest (0 ms)
[ RUN      ] OSMTest.SimpleNodeTest
[       OK ] OSMTest.SimpleNodeTest (0 ms)
[ RUN      ] OSMTest.NodeTest
[       OK ] OSMTest.NodeTest (0 ms)
[ RUN      ] OSMTest.SimpleWayTest
[       OK ] OSMTest.SimpleWayTest (0 ms)
[ RUN      ] OSMTest.WayTest
[       OK ] OSMTest.WayTest (0 ms)
[----------] 5 tests from OSMTest (1 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test suite ran. (1 ms total)
[  PASSED  ] 5 tests.
./bin/testtpcl ./bin/testtp
make: ./bin/testtpcl: No such file or directory
make: *** [Makefile:20: runtests] Error 127
CC=gcc
CXX=g++
OBJ_DIR = ./obj
SRC_DIR = ./src
BIN_DIR = ./bin
INC_DIR = ./include
TEST_SRC_DIR = ./testsrc
CXXFLAGS = -std=c++17 -I$(INC_DIR)
LDFLAGS = -lgtest -lgtest_main -lpthread -lexpat


all: directories runtests

runtests:  $(BIN_DIR)/testdpr $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testosm   #$(BIN_DIR)/testcsvbs 
	@echo "dpr tests..."
	@$(BIN_DIR)/testdpr
	@echo "bsi tests..."
	@$(BIN_DIR)/testcsvbsi
	$(BIN_DIR)/testosm





$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) $^ -o $@ $(LDFLAGS)


$(BIN_DIR)/testosm: $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) $^ -o $@ $(LDFLAGS)


$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouterTest.o $(OBJ_DIR)/DijkstraPathRouter.o 
	$(CXX) $^ -o $@ $(LDFLAGS)


$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)


$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/OSMTest.o: $(TEST_SRC_DIR)/OSMTest.cpp $(INC_DIR)/OpenStreetMap.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/OpenStreetMap.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DijkstraPathRouterTest.o: $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/PathRouter.o: $(SRC_DIR)/PathRouter.cpp $(INC_DIR)/PathRouter.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/DSVReader.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)



$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o
	$(CXX) -o $(BIN_DIR)/testcsvbsi $(CXXFLAGS) $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(LDFLAGS)

$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/BusSystem.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/BusSystemIndexer.o -c $(CXXFLAGS) $(SRC_DIR)/BusSystemIndexer.cpp

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/StreetMap.h $(INC_DIR)/DSVReader.h $(INC_DIR)/XMLReader.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp


clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

这是我写的makefile，但是还没有把我刚写的两个cpp加进去，帮我添加几行
link transportationPlannerCommandLine and TPCommandLineTest object files to make testtpcl executable


DijkstraTransportationPlanner.h
GeographicUtils.h
TransportationPlanner.h
TransportationPlannerCommandLine.h
TransportationPlannerConfig.h
CTransportationPlannerCommandLine.cpp
CDijkstraTransportationPlanner.cpp
GeographicUtils.cpp
CSVOSMTransportationPlannerTest.cpp
TPCommandLineTest.cpp
这是一些文件的文件名
ChatGPT
To integrate the new components into your existing Makefile while addressing the errors and avoiding duplication, let's refine your setup. The main focus will be on adding the compilation and linking steps for CTransportationPlannerCommandLine.cpp, CDijkstraTransportationPlanner.cpp, GeographicUtils.cpp, and their corresponding tests CSVOSMTransportationPlannerTest.cpp, TPCommandLineTest.cpp.

Here is the revised Makefile with comments to guide you through the additions and changes:

makefile
Copy code
CC=g++
CXXFLAGS=-std=c++17 -I$(INC_DIR)
LDFLAGS=-lgtest -lgtest_main -lpthread -lexpat

OBJ_DIR=./obj
SRC_DIR=./src
BIN_DIR=./bin
INC_DIR=./include
TEST_SRC_DIR=./testsrc

all: directories runtests testtpcl testtp

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) $(CXXFLAGS) -c $< -o $@

# Test object files
$(OBJ_DIR)/%Test.o: $(TEST_SRC_DIR)/%Test.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

# Specific test binaries
$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/testtp: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/GeographicUtils.o
	$(CC) $^ -o $@ $(LDFLAGS)

# Update the runtests target to include your new tests
runtests: $(BIN_DIR)/testdpr $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testosm $(BIN_DIR)/testtpcl $(BIN_DIR)/testtp
	@echo "Running DijkstraPathRouter tests..."
	@./$(BIN_DIR)/testdpr
	@echo "Running CSVBusSystemIndexer tests..."
	@./$(BIN_DIR)/testcsvbsi
	@echo "Running OpenStreetMap tests..."
	@./$(BIN_DIR)/testosm
	@echo "Running TransportationPlannerCommandLine tests..."
	@./$(BIN_DIR)/testtpcl
	@echo "Running CSVOSMTransportationPlanner tests..."
	@./$(BIN_DIR)/testtp

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
Here's a brief explanation of the changes:

The %.o: %.cpp pattern rule simplifies the compilation of source files into object files, removing the need to write individual rules for each file.
The runtests target has been updated to run the newly added tests for the transportation planner command line and CSV/OSM transportation planner.
Specific test binaries like testtpcl and testtp are compiled with their corresponding object files.
I've removed duplicate and conflicting rules that were causing the warnings you saw. Make sure each target is uniquely defined.
This setup assumes your source and test filenames are consistent with the names of their corresponding header files. Adjust the patterns as necessary to fit your project's structure.

...


// 计算最快路径
double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) override {
    // 与FindShortestPath类似，但是还需要考虑不同交通方式的速度
    return 0.0; // 返回计算得到的最快时间
}


...


### Chagnes 1
stucked on the make file error and comment out FindFastestPath function, giving up. 

