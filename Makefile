CXX = g++
CXXFLAGS = -std=c++17 -I $(INC_DIR) -I /usr/include -g
LDFLAGS = -pthread -lexpat -lgtest -lgtest_main -L /usr/lib
MAKEFLAGS += --warn-undefined-variables


INC_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj
TEST_SRC_DIR = ./testsrc


all: directories runtests

runtests: $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testfiledatass $(BIN_DIR)/testdsv $(BIN_DIR)/testxml $(BIN_DIR)/testkml $(BIN_DIR)/testcsvbs $(BIN_DIR)/testosm $(BIN_DIR)/testdpr $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testtp #$(BIN_DIR)/testtpcl
	$(BIN_DIR)/teststrdatasource
	$(BIN_DIR)/teststrdatasink
	$(BIN_DIR)/testfiledatass
	$(BIN_DIR)/testdsv
	$(BIN_DIR)/testxml
	$(BIN_DIR)/testkml
	$(BIN_DIR)/testcsvbs
	$(BIN_DIR)/testosm
	$(BIN_DIR)/testdpr
	$(BIN_DIR)/testcsvbsi
	$(BIN_DIR)/testtp
	#$(BIN_DIR)/testtpcl
	

#strutils


$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o -c $(CXXFLAGS) $(SRC_DIR)/StringUtils.cpp







#strdatasink


$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/StringDataSink.cpp

$(OBJ_DIR)/StringDataSinkTest.o: $(TEST_SRC_DIR)/StringDataSinkTest.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSinkTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSinkTest.cpp


$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o 
	$(CXX) -o $(BIN_DIR)/teststrdatasink $(CXXFLAGS) $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o $(LDFLAGS)







#strdatasource


$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSource.o -c $(CXXFLAGS) $(SRC_DIR)/StringDataSource.cpp

$(OBJ_DIR)/StringDataSourceTest.o: $(TEST_SRC_DIR)/StringDataSourceTest.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSourceTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSourceTest.cpp

$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) -o $(BIN_DIR)/teststrdatasource $(CXXFLAGS) $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o $(LDFLAGS)









#testfiledatass


$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/FileDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataSink.cpp

$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/FileDataSource.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataSource.cpp

$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h $(INC_DIR)/DataFactory.h $(INC_DIR)/DataSink.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/FileDataFactory.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataFactory.cpp

$(OBJ_DIR)/FileDataSSTest.o: $(TEST_SRC_DIR)/FileDataSSTest.cpp $(INC_DIR)/FileDataFactory.h $(INC_DIR)/FileDataSink.h $(INC_DIR)/FileDataSource.h
	$(CXX) -o $(OBJ_DIR)/FileDataSSTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/FileDataSSTest.cpp


$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
	$(CXX) -o $(BIN_DIR)/testfiledatass $(CXXFLAGS) $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o $(LDFLAGS)








#testdsv


$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/DSVReader.o -c $(CXXFLAGS) $(SRC_DIR)/DSVReader.cpp

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/DSVWriter.o -c $(CXXFLAGS) $(SRC_DIR)/DSVWriter.cpp

$(OBJ_DIR)/DSVTest.o: $(TEST_SRC_DIR)/DSVTest.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DSVWriter.h $(INC_DIR)/DataSink.h $(INC_DIR)/DataSource.h $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSVTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/DSVTest.cpp


$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/testdsv $(CXXFLAGS) $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)








#testxml


$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/DataSource.h $(INC_DIR)/XMLEntity.h
	$(CXX) -o $(OBJ_DIR)/XMLReader.o -c $(CXXFLAGS) $(SRC_DIR)/XMLReader.cpp

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h $(INC_DIR)/DataSink.h $(INC_DIR)/XMLEntity.h
	$(CXX) -o $(OBJ_DIR)/XMLWriter.o -c $(CXXFLAGS) $(SRC_DIR)/XMLWriter.cpp

$(OBJ_DIR)/XMLTest.o: $(TEST_SRC_DIR)/XMLTest.cpp $(INC_DIR)/XMLWriter.h $(INC_DIR)/XMLEntity.h $(INC_DIR)/DataSink.h $(INC_DIR)/DataSource.h $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/XMLTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/XMLTest.cpp

$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testxml $(CXXFLAGS) $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)









#testkml


$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h $(INC_DIR)/XMLWriter.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/KMLWriter.o -c $(CXXFLAGS) $(SRC_DIR)/KMLWriter.cpp

$(OBJ_DIR)/KMLTest.o: $(TEST_SRC_DIR)/KMLTest.cpp $(INC_DIR)/KMLWriter.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/KMLTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/KMLTest.cpp


$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/testkml $(CXXFLAGS) $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)







#testcsvbs


$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystem.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystem.o -c $(CXXFLAGS) $(SRC_DIR)/CSVBusSystem.cpp

$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DataSource.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemTest.cpp


$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testcsvbs $(CXXFLAGS) $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)







#testosm


$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/OpenStreetMap.o -c $(CXXFLAGS) $(SRC_DIR)/OpenStreetMap.cpp

$(OBJ_DIR)/OSMTest.o: $(TEST_SRC_DIR)/OSMTest.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/StreetMap.h $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/OSMTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/OSMTest.cpp

$(BIN_DIR)/testosm: $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testosm $(CXXFLAGS) $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)









#testdpr


$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h $(INC_DIR)/PathRouter.h
	$(CXX) -o $(OBJ_DIR)/DijkstraPathRouter.o -c $(CXXFLAGS) $(SRC_DIR)/DijkstraPathRouter.cpp

$(OBJ_DIR)/DijkstraPathRouterTest.o: $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h $(INC_DIR)/PathRouter.h
	$(CXX) -o $(OBJ_DIR)/DijkstraPathRouterTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp

$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o
	$(CXX) -o $(BIN_DIR)/testdpr $(CXXFLAGS) $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o $(LDFLAGS)





#testcsvbsi



$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/BusSystem.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/BusSystemIndexer.o -c $(CXXFLAGS) $(SRC_DIR)/BusSystemIndexer.cpp

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/StreetMap.h $(INC_DIR)/DSVReader.h $(INC_DIR)/XMLReader.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp

$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o
	$(CXX) -o $(BIN_DIR)/testcsvbsi $(CXXFLAGS) $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(LDFLAGS)





#testtp



$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp $(INC_DIR)/GeographicUtils.h
	$(CXX) -o $(OBJ_DIR)/GeographicUtils.o -c $(CXXFLAGS) $(SRC_DIR)/GeographicUtils.cpp

$(OBJ_DIR)/CDijkstraTransportationPlanner.o: $(SRC_DIR)/CDijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/TransportationPlannerConfig.h $(INC_DIR)/TransportationPlanner.h $(INC_DIR)/DijkstraPathRouter.h $(INC_DIR)/GeographicUtils.h $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $(OBJ_DIR)/CDijkstraTransportationPlanner.o -c $(CXXFLAGS) $(SRC_DIR)/CDijkstraTransportationPlanner.cpp

$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(INC_DIR)/DijkstraPathRouter.h $(INC_DIR)/XMLReader.h $(INC_DIR)/DSVReader.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystemIndexer.h $(INC_DIR)/TransportationPlannerConfig.h $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/TransportationPlanner.h $(INC_DIR)/GeographicUtils.h
	$(CXX) -o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp


$(BIN_DIR)/testtp: $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o
	$(CXX) -o $(BIN_DIR)/testtp $(CXXFLAGS) $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(LDFLAGS)





#testtpcl
# TransportationPlannerCommandLine
#$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/CTransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
#	$(CXX) -o $(OBJ_DIR)/TransportationPlannerCommandLine.o -c $(CXXFLAGS) $(SRC_DIR)/CTransportationPlannerCommandLine.cpp

# TPCommandLineTest
#$(OBJ_DIR)/TPCommandLineTest.o: $(TEST_SRC_DIR)/TPCommandLineTest.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
#	$(CXX) -o $(OBJ_DIR)/TPCommandLineTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/TPCommandLineTest.cpp

# Linking testtpcl executable
#$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o
#	$(CXX) -o $(BIN_DIR)/testtpcl $(CXXFLAGS) $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o $(LDFLAGS)



clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
