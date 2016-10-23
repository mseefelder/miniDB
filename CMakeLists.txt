cmake_minimum_required(VERSION 2.8.12)
project(miniDB C CXX)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/src")

add_definitions("
	-std=c++11
	-Wall
	-Wextra
	-Wno-return-type
	-Wno-unused-parameter
")

include_directories(
	"${INCLUDE_DIR}"
)

set(
	PROJECT_HEADERS
	"${INCLUDE_DIR}/BinFileHandler.h"
	"${INCLUDE_DIR}/BplusIndex.h"
	"${INCLUDE_DIR}/BplusTree.h"
	"${INCLUDE_DIR}/DenseIndex.h"
	"${INCLUDE_DIR}/HashIndex.h"
	"${INCLUDE_DIR}/HashTable.h"
	"${INCLUDE_DIR}/Header.h"
	"${INCLUDE_DIR}/Index.h"
	"${INCLUDE_DIR}/Join.h"
	"${INCLUDE_DIR}/LinkedList.h"
	"${INCLUDE_DIR}/Relation.h"
	"${INCLUDE_DIR}/bpt.h"
	"${INCLUDE_DIR}/predefined.h"
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
	"${SRC_DIR}/main-join.cpp"
)

add_executable(
	main-query
	${PROJECT_HEADERS}
	${PROJECT_SOURCES}
	"${SRC_DIR}/main-query.cpp"
)