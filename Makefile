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

#$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/BusSystemIndexer.o
#	$(CXX) $^ -o $@ $(LDFLAGS)



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

#$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/DSVReader.h
#	$(CXX) -c $< -o $@ $(CXXFLAGS)

#$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
#	$(CXX) -c $< -o $@ $(CXXFLAGS)

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