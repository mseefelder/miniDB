cmake_minimum_required(VERSION 2.8.12)
project(miniDB C CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

add_definitions("
	-std=c++11
	-Wall
	-Wextra
	-Wno-unused-parameter
	-Wno-return-type
")

include_directories(
	"${SRC_DIR}"
)

set(
	PROJECT_HEADERS
	"${SRC_DIR}/BinFileHandler.h"
	"${SRC_DIR}/BplusIndex.h"
	"${SRC_DIR}/BplusTree.h"
	"${SRC_DIR}/DenseIndex.h"
	"${SRC_DIR}/HashIndex.h"
	"${SRC_DIR}/HashTable.h"
	"${SRC_DIR}/Header.h"
	"${SRC_DIR}/Index.h"
	"${SRC_DIR}/Join.h"
	"${SRC_DIR}/LinkedList.h"
	"${SRC_DIR}/Relation.h"
	"${SRC_DIR}/bpt.h"
	"${SRC_DIR}/predefined.h"
)

set(PROJECT_SOURCES
	"${SRC_DIR}/BinFileHandler.cpp"
	"${SRC_DIR}/BplusIndex.cpp"
	"${SRC_DIR}/BplusTree.cpp"
	"${SRC_DIR}/DenseIndex.cpp"
	"${SRC_DIR}/HashIndex.cpp"
	"${SRC_DIR}/HashTable.cpp"
	"${SRC_DIR}/Index.cpp"
	"${SRC_DIR}/Join.cpp"
	"${SRC_DIR}/LinkedList.cpp"
	"${SRC_DIR}/Relation.cpp"
	"${SRC_DIR}/bpt.cc"
)

add_executable(
	main-join
	${PROJECT_HEADERS}
	${PROJECT_SOURCES}
	${SRC_DIR}/main-join.cpp
)

add_executable(
	main-query
	${PROJECT_HEADERS}
	${PROJECT_SOURCES}
	${SRC_DIR}/main-query.cpp
)