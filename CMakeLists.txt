cmake_minimum_required(VERSION 2.8.12)
project(MINIDB C CXX)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/src")

option(TEST "Build all tests." OFF)

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

add_library(
	MINIDB_LIBRARY
	OBJECT
	${PROJECT_HEADERS}
	${PROJECT_SOURCES}
)

add_executable(
	main-join
	"${SRC_DIR}/main-join.cpp"
	$<TARGET_OBJECTS:MINIDB_LIBRARY>
)

add_executable(
	main-query
	"${SRC_DIR}/main-query.cpp"
	$<TARGET_OBJECTS:MINIDB_LIBRARY>
)

if(TEST)
	enable_testing()
	
	set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
	set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
	
	# Set-up Google Test
	add_subdirectory(${VENDOR_DIR}/googletest)
	include_directories("${gtest_SOURCE_DIR}/include")
	set(GTEST_LIBRARIES gtest gtest_main)
	
	add_executable(
		TestHashTable
		"${TEST_DIR}/TestHashTable.cpp"
		$<TARGET_OBJECTS:MINIDB_LIBRARY>
	)
	
	target_link_libraries(
		TestHashTable
		${GTEST_LIBRARIES}
	)
	
	# Integrate with ctest (`make test`)
	add_test(NAME TestHashTable COMMAND TestHashTable)
endif()