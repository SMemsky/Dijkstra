# Makefile for Dijkstra's algorithm visualisation

Target = dijkstra
TargetPath = ../bin/

CXX = g++-5
CXXFLAGS = -O0 -Wall -std=c++14 -D_USE_MATH_DEFINES
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window -lpthread

MAIN_OBJECTS = main.o \
	Model.o \
	View.o \
	Controller.o \
	GraphView.o \
	GraphController.o \
	GraphModel.o \
	Graph.o \
	Edge.o

all: dijkstra

dijkstra: $(MAIN_OBJECTS)
	$(CXX) $(MAIN_OBJECTS) -o $(TargetPath)$(Target) $(LDFLAGS)

main.o: main.cpp \
		View.h \
		GraphView.h \
		GraphModel.h \
		Observer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Model.o: Model.cpp Model.h \
		Observer.h
	$(CXX) $(CXXFLAGS) -c Model.cpp

View.o: View.cpp View.h \
		Observer.h \
		Model.h \
		Controller.h
	$(CXX) $(CXXFLAGS) -c View.cpp

Controller.o: Controller.cpp Controller.h \
		Observer.h \
		Model.h \
		View.h
	$(CXX) $(CXXFLAGS) -c Controller.cpp

GraphView.o: GraphView.cpp GraphView.h \
		Observer.h \
		View.h \
		Model.h \
		Controller.h \
		GraphController.h \
		GraphModel.h
	$(CXX) $(CXXFLAGS) -c GraphView.cpp

GraphController.o: GraphController.cpp GraphController.h \
		Observer.h \
		Controller.h \
		View.h \
		Model.h \
		GraphModel.h
	$(CXX) $(CXXFLAGS) -c GraphController.cpp

GraphModel.o: GraphModel.cpp GraphModel.h \
		Model.h \
		Observer.h
	$(CXX) $(CXXFLAGS) -c GraphModel.cpp

Graph.o: Graph.cpp Graph.h \
		GraphNode.h \
		GraphEdge.h \
		GraphNodeData.h
	$(CXX) $(CXXFLAGS) -c Graph.cpp

Edge.o: Edge.cpp Edge.h
	$(CXX) $(CXXFLAGS) -c Edge.cpp

clean:
	rm -r *.o

.PHONY: all dijkstra clean
