#include "DataBase.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "HashIndex.h"
#include "Relation.h"

using namespace std;

//Creates a table in the database and stores it in the "tables" map
//Table creation through "csvFile" (relative path to csv file)
bool DataBase::createTable(std::string csvFile, char separator, vector<short> tupleFormat, std::string relationName){

    Relation newRelation(relationName, tupleFormat);

    if(!newRelation.open() ){

        std::cout << "Carregando dados para base binaria..." << std::endl;

        if (IES.load(csvFile, separator)) std::cout << "Carregamento de dados efetuado com sucesso!" << std::endl;
        else { std::cout << "Falha no carregamento da base de dados." << std::endl; return 1; }

        //insert in the map
        tables.insert(std::pair<std::string, Relation>(relationName, newRelation));

        return true;

    }

    else std::cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << std::endl;

    return false;
}

//Creates an indexing structure, according to the id supplied
//1 for dense, 2 for b+ tree, 3 for hashing
bool DataBase::createIndex(std::string relationName, int indexTypeId){

	switch(indexTypeId){

		case 1:

		    std::cout << "Instanciando DenseIndex" << std::endl;
		    DenseIndex dPKEY(tables.at(relationName).getBinFilename(), 
		    				tables.at(relationName).getNumTuples(), 1, 
		    				tables.at(relationName).getTupleSize());
		    				//for while, DenseIndex works only for the 1st attribute, considering it as a INT4
		    if (!dPKEY.load()) {
		        std::cout << "Construindo DenseIndex" << std::endl;

		        if(dPKEY.build()) {

		        	std::cout <<"DenseIndex construido e gravado em disco com sucesso" std::endl;
		        	indexes.insert(std::pair<std::string, Index>(relationName, dPKEY));
		        	return true;

		        } 
		        else "Falha na construcao ou gravacao do DenseIndex";
		    }
		    else std::cout << "DenseIndex pre-existente no disco foi encontrado e carregado em memoria." << std::endl; // dPKEY.printIndex();
		    return false;

			break;

		case 2:

		    // B+ TREE CREATION
		    std::cout << "Instanciando BplusIndex" << std::endl;
		    BplusIndex bpi(tables.at(relationName).getBinFilename(), 
		    				tables.at(relationName).getNumTuples(), 1, 
		    				tables.at(relationName).getTupleSize());
		    std::cout << "Construindo BplusIndex" << std::endl;
		    bpi.build();
		    std::cout << "BplusIndex construido!" << std::endl;
		    indexes.insert(std::pair<std::string, Index>(relationName, bpi));
		    return true;	

			break;

		case 3:

		    std::cout << "Instanciando HashIndex" << std::endl;
		    HashIndex hPKEY(tables.at(relationName).getBinFilename(), 
		    				tables.at(relationName).getNumTuples(), 1, 
		    				tables.at(relationName).getTupleSize());
		    				
		    if (!hPKEY.load()) {
		        std::cout << "Construindo HashIndex" << std::endl;
		        if(hPKEY.build()) {

		        	std::cout << "HashIndex construido e gravado em disco com sucesso" << std::endl;
		        	indexes.insert(std::pair<std::string, Index>(relationName, hPKEY));	
		        	return true;

		        }
		        else "Falha na construcao ou gravacao do HashIndex";
		        return false;
		    }
		    else std::cout << "HashIndex pre-existente no disco foi encontrado e carregado em memoria." << std::endl; // hPKEY.printIndex();
		    return false;

			break;



		default:

			std::cout << "nao existe tipo de indexacao valida com esse id" << std::endl;
			return false;

	}

	return true;
    
}


//does a search in the database, and can use two tables for joins
//fields can be supplied through a string array (variable size)
bool DataBase::select(std::string firstRelation, int key, int join_id = 0, std::string secondRelation = NULL){

	//SELECT fields FROM firstRelation ([WHATEVER] JOIN) secondRelation
	//WHERE 

	//Check if the tables are indexed

	Index firstIndex = indexes.find (firstRelation);
	if ( firstIndex == indexes.end() ) { //didn't find
		//do direct access magic
	}

	Index secondIndex;

	if (secondRelation != NULL){
		secondIndex = indexes.find(secondRelation);

		if (secondIndex == indexes.end() ) { //didn't find

			//do direct access magic

		}
	}

	
}

