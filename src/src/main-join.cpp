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

    //BinFileHandler bfh1 (Cidade.getIndex()->getRelBinFilename(), true);
    //BinFileHandler bfh2 (Nome.getIndex()->getRelBinFilename(), true);
    //BinFileHandler bfh3 ("bfJoin.bin",false);
    //BinFileHandler bfh4 ("msJoin.bin",false);
    //BinFileHandler bfh5 ("hsJoin.bin",false);

    // std::cout << std::endl << "bruteForceJoin" << std::endl;
    // pair<unsigned, unsigned> bFJoin = bruteForceJoin(Cidade, Nome, 0, 0, false);
    // std::cout << std::endl << "mergeSortJoin" << std::endl;

    std::cout << std::endl << "mergeSortJoin" << std::endl;
    pair<unsigned, unsigned> mSJoin = mergeSortJoin(Cidade, Nome, 0, 0, true);
    
    // pair<unsigned, unsigned> mSJoin = mergeSortJoin(Cidade.getIndex(), Nome.getIndex(), bfh1, bfh2, bfh4, Cidade, Nome, 0);
    // std::cout << std::endl << "hashJoin" << std::endl;
    // pair<unsigned, unsigned> hsJoin = hashJoin(Cidade.getIndex(), Nome.getIndex(), bfh1, bfh2, bfh5, Cidade, Nome, 0);

    // std::cout << computeTime(bFJoin) << std::endl;
    std::cout << computeTime(mSJoin) << std::endl;

    Relation joinRelation("joinOut", Cidade.getTupleFormat(), Nome.getTupleFormat(), 0);

    return 0;
}
