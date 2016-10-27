#include <chrono>
#include <iostream>
#include <vector>
#include <string>

#include "Relation.h"
#include "bpt.h"
#include "HashIndex.h"
#include "Join.h"

using namespace std;

int main()
{
    vector<short> tF {INT4, CHAR32};
    string relationNameCidade = "Cidade";
    Relation Cidade(relationNameCidade, tF);
    if(!Cidade.open()){
        cout << "Carregando dados de Cidade para base binaria..." << endl;
        if(Cidade.load("../csv/cidades.csv",'|')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    Cidade.loadOrBuildIndex(1);

    string relationNameNome = "Nome";
    Relation Nome(relationNameNome, tF);
    if(!Nome.open()){
        cout << "Carregando dados Nome para base binaria..." << endl;
        if(Nome.load("../csv/nomes.csv",'|')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    Nome.loadOrBuildIndex(1);

    std::cout << std::endl << "bruteForceJoin without index" << std::endl;
    pair<unsigned, unsigned> bFJoinWOI = bruteForceJoin(Cidade, Nome, 0, 0, false);


    std::cout << std::endl << "bruteForceJoin with index" << std::endl;
    pair<unsigned, unsigned> bFJoinWI = bruteForceJoin(Cidade, Nome, 0, 0, true);

// Only when we implement relations physically ordered by key
//    std::cout << std::endl << "mergeSortJoin" << std::endl;
//    pair<unsigned, unsigned> mSJoinWOI = mergeSortJoin(Cidade, Nome, 0, 0, false); 
    
    std::cout << std::endl << "mergeSortJoin with index" << std::endl;
    pair<unsigned, unsigned> mSJoinWI = mergeSortJoin(Cidade, Nome, 0, 0, true);

    std::cout << std::endl << "hashJoin without index" << std::endl;
    pair<unsigned, unsigned> hsJoinWOI = hashJoin(Cidade, Nome, 0, 0, false);

	std::cout << std::endl << "hashJoin without index" << std::endl;
    pair<unsigned, unsigned> hsJoinWI = hashJoin(Cidade, Nome, 0, 0, true);


    // std::cout << computeTime(bFJoin) << std::endl;
    // std::cout << computeTime(mSJoin) << std::endl;

    //Relation joinRelation("joinOut", Cidade.getTupleFormat(), Nome.getTupleFormat(), 0);

    return 0;
}
