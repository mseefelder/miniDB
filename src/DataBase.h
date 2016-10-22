#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <vector>
#include "Relation.h"
#include <map>


//models a simple database system, with operations for creating and searching on tables
class DataBase
{


private:

	//Stores database tables, that can be searched for using their names
	std::map<std::string, Relation> tables;

	//Stores database indexes, that can be searched for using their names
	std::map<std::string, Index> indexes;

	//Stores tupleFormats, that can be searched for using their names
	std::map<std::string, Index> tupleFormats;


public:
	//constructor (not sure if it should do anything at all)
	DataBase();

	//destructor (send all these tables to nothingness)
	~DataBase();

	//Creates a table in the database and stores it in the "tables" map
	//Table creation through "csvFile" (relative path to csv file)
	bool createTable(std::string csvFile, char separator, vector<short> tupleFormat, std::string relationName);

	//Creates an indexing structure, according to the id supplied
	//1 for dense, 2 for b+ tree, 3 for hashing
	bool createIndex(std::string relationName, int indexTypeId);

	//does a search in the database, and can use two tables for joins
	//fields can be supplied through a string array (variable size)
	bool select(std::string firstRelation, std::string* fields, std::string desiredField, int condition_value, int join_id = 0, std::string secondRelation = NULL)



};

#endif