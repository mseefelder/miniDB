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
    vector<short> tF1 {INT4, INT4};
    string relationNameRelacao1 = "Relacao1";
    Relation Relacao1(relationNameRelacao1, tF1);
    if(!Relacao1.open()){
        cout << "Carregando dados de Relacao1 para base binaria..." << endl;
        if(Relacao1.load("../csv/r1.csv",',')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    Relacao1.loadOrBuildIndex(1);

    std::vector<short> tF2 {INT4, CHAR256, CHAR256};
    string relationNameRelacao2 = "Relacao2";
    Relation Relacao2(relationNameRelacao2, tF2);
    if(!Relacao2.open()){
        cout << "Carregando dados Relacao2 para base binaria..." << endl;
        if(Relacao2.load("../csv/r2.csv",',')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    Relacao2.loadOrBuildIndex(1);

    std::vector<short> tF3 {INT4, INT4, INT4, INT4};
    string relationNameRelacao3 = "Relacao3";
    Relation Relacao3(relationNameRelacao3, tF3);
    if(!Relacao3.open()){
        cout << "Carregando dados de Relacao3 para base binaria..." << endl;
        if(Relacao3.load("../csv/r3.csv",',')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    Relacao3.loadOrBuildIndex(1);

    std::cout << "Testando metodos de join." << std::endl << std::endl;

    // keys with keys ---------------------------------------------------------
    
    std::cout << std::endl << "---------------------------------------------------" << std::endl;
    std::cout << " Chaves primarias com chaves estrangeiras." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl << std::endl;

    DenseIndex *dRightRelation = new DenseIndex(Relacao1.getBinFilename(),
                                               Relacao1.getNumTuples(), 
                                               0, 
                                               Relacao1.getTupleSize());
    DenseIndex *dLeftRelation = new DenseIndex(Relacao2.getBinFilename(),
                                              Relacao2.getNumTuples(), 
                                              0, 
                                              Relacao2.getTupleSize());

    auto rPair = dRightRelation->build();
    auto lPair = dLeftRelation->build();

    unsigned indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

    //----using index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando indices existentes." << std::endl;
    std::cout << "***************************************************" << std::endl;
    std::pair<unsigned, unsigned> bFJoinWI = bruteForceJoin(Relacao1, Relacao2, 0, 0, true);
    std::pair<unsigned, unsigned> mSJoinWI = mergeSortJoin(Relacao1, Relacao2, 0, 0, true);
    //std::pair<unsigned, unsigned> hsJoinWI = hashJoin(Relacao1, Relacao2, 0, 0, true);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

    //----creating index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Criando indices." << std::endl;
    std::cout << "***************************************************" << std::endl;

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

    //----using bin file

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando arquivo binario." << std::endl;
    std::cout << "***************************************************" << std::endl;

    std::pair<unsigned, unsigned> bFJoinWOI = bruteForceJoin(Relacao1, Relacao2, 0, 0, false);
    std::pair<unsigned, unsigned> mSJoinWOI = mergeSortJoin(Relacao1, Relacao2, 0, 0, false);
    //std::pair<unsigned, unsigned> hsJoinWOI = hashJoin(Relacao1, Relacao2, 0, 0, false);


    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

    // keys with non-keys -----------------------------------------------------

    std::cout<< std::endl  << "---------------------------------------------------" << std::endl;
    std::cout << " Chaves primarias com atributos nao-chave." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl << std::endl;

    dRightRelation = new DenseIndex(Relacao1.getBinFilename(),
                                               Relacao1.getNumTuples(), 
                                               0, 
                                               Relacao1.getTupleSize());
    dLeftRelation = new DenseIndex(Relacao3.getBinFilename(),
                                              Relacao3.getNumTuples(), 
                                              1, 
                                              Relacao3.getTupleSize());

    rPair = dRightRelation->build();
    lPair = dLeftRelation->build();

    indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

    Relacao1.loadOrBuildIndex(1);
    Relacao3.loadOrBuildIndex(2);

    //----using index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando indices existentes." << std::endl;
    std::cout << "***************************************************" << std::endl;
    bFJoinWI = bruteForceJoin(Relacao1, Relacao3, 0, 1, true);
    // mSJoinWI = mergeSortJoin(Relacao1, Relacao3, 0, 1, true);
    //hsJoinWI = hashJoin(Relacao1, Relacao3, 0, 1, true);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

    //----creating index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Criando indices." << std::endl;
    std::cout << "***************************************************" << std::endl;

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

    //----using bin file

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando arquivo binario." << std::endl;
    std::cout << "***************************************************" << std::endl;

    bFJoinWOI = bruteForceJoin(Relacao1, Relacao3, 0, 1, false);
    // mSJoinWOI = mergeSortJoin(Relacao1, Relacao3, 0, 1, false);
    //hsJoinWOI = hashJoin(Relacao1, Relacao3, 0, 1, false);


    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

    std::cout << std::endl << "---------------------------------------------------" << std::endl;
    std::cout << " Atributos nao-chave com atributos nao-chave." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl << std::endl;


    Relacao1.loadOrBuildIndex(2);
    Relacao3.loadOrBuildIndex(4);

    // for (const auto & e : Relacao1.getIndex()->index) {
    //     std::cout << e.first << " " << e.second << std::endl;
    // }

    // for (const auto & e : Relacao3.getIndex()->index) {
    //     std::cout << e.first << " " << e.second << std::endl;
    // }
   //----using index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando indices existentes." << std::endl;
    std::cout << "***************************************************" << std::endl;

    bFJoinWI = bruteForceJoin(Relacao1, Relacao3, 1, 3, true);
    // mSJoinWI = mergeSortJoin(Relacao1, Relacao3, 1, 3, true);
    //hsJoinWI = hashJoin(Relacao1, Relacao3, 1, 3, true);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

    //----creating index

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Criando indices." << std::endl;
    std::cout << "***************************************************" << std::endl;

    dRightRelation = new DenseIndex(Relacao1.getBinFilename(),
                                               Relacao1.getNumTuples(), 
                                               2, 
                                               Relacao1.getTupleSize());
    dLeftRelation = new DenseIndex(Relacao3.getBinFilename(),
                                              Relacao3.getNumTuples(), 
                                              4, 
                                              Relacao3.getTupleSize());

    rPair = dRightRelation->build();
    lPair = dLeftRelation->build();

    indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

    //----using bin file

    std::cout << std::endl << "***************************************************" << std::endl;
    std::cout << " Usando arquivo binario." << std::endl;
    std::cout << "***************************************************" << std::endl;

    bFJoinWOI = bruteForceJoin(Relacao1, Relacao3, 1, 3, false);
    // mSJoinWOI = mergeSortJoin(Relacao1, Relacao3, 1, 3, false);
    //hsJoinWOI = hashJoin(Relacao1, Relacao3, 1, 3, false);


    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
    // std::cout << "Merge Sort Join" << std::endl;
    // std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    //std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

    return 0;
}
