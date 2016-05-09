#!/bin/bash

cd ./bin/
valgrind --leak-check=full ./dijkstra
cd ../