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

    std::cout << "Testando metodos de join." << std::endl << std::endl;

    // keys with keys ---------------------------------------------------------
    
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << " Chaves primarias com chaves estrangeiras." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl << std::endl;

    //----using index

    std::cout << "***************************************************" << std::endl;
    std::cout << " Usando indices existentes." << std::endl;
    std::cout << "***************************************************" << std::endl;
    std::pair<unsigned, unsigned> bFJoinWI = bruteForceJoin(Cidade, Nome, 0, 0, true);
    std::pair<unsigned, unsigned> mSJoinWI = mergeSortJoin(Cidade, Nome, 0, 0, true);
    std::pair<unsigned, unsigned> hsJoinWI = hashJoin(Cidade, Nome, 0, 0, true);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

    //----creating index

    std::cout << "***************************************************" << std::endl;
    std::cout << " Criando indices." << std::endl;
    std::cout << "***************************************************" << std::endl;

    DenseIndex *dRightRelation = new DenseIndex(Cidade.getBinFilename(),
                                               Cidade.getNumTuples(), 
                                               0, 
                                               Cidade.getTupleSize());
    DenseIndex *dLeftRelation = new DenseIndex(Nome.getBinFilename(),
                                              Nome.getNumTuples(), 
                                              0, 
                                              Nome.getTupleSize());

    auto rPair = dRightRelation->build();
    auto lPair = dLeftRelation->build();

    unsigned indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
    std::cout << "Hash Join" << std::endl;
    std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

    //----using bin file

    std::cout << "***************************************************" << std::endl;
    std::cout << " Usando arquivo binario." << std::endl;
    std::cout << "***************************************************" << std::endl;

    std::pair<unsigned, unsigned> bFJoinWOI = bruteForceJoin(Cidade, Nome, 0, 0, false);
    std::pair<unsigned, unsigned> mSJoinWOI = mergeSortJoin(Cidade, Nome, 0, 0, false);
    std::pair<unsigned, unsigned> hsJoinWOI = hashJoin(Cidade, Nome, 0, 0, false);


    std::cout << "Forca bruta " << std::endl;
    std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
    std::cout << "Merge Sort Join" << std::endl;
    std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
    std::cout << "Hash Join" << std::endl;
    std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

    // keys with non-keys -----------------------------------------------------

   //  std::cout << "---------------------------------------------------" << std::endl;
   //  std::cout << " Chaves primarias com atributos nao-chave." << std::endl;
   //  std::cout << "---------------------------------------------------" << std::endl << std::endl;

   //  //----using index

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Usando indices existentes." << std::endl;
   //  std::cout << "***************************************************" << std::endl;
   //  std::pair<unsigned, unsigned> bFJoinWI = bruteForceJoin(Cidade, Nome, 0, 1, true);
   //  std::pair<unsigned, unsigned> mSJoinWI = mergeSortJoin(Cidade, Nome, 0, 1, true);
   //  std::pair<unsigned, unsigned> hsJoinWI = hashJoin(Cidade, Nome, 0, 1, true);

   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

   //  //----creating index

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Criando indices." << std::endl;
   //  std::cout << "***************************************************" << std::endl;

   //  DenseIndex *dRightRelation = new DenseIndex(Cidade.getBinFilename(),
   //                                             Cidade.getNumTuples(), 
   //                                             0, 
   //                                             Cidade.getTupleSize());
   //  DenseIndex *dLeftRelation = new DenseIndex(Nome.getBinFilename(),
   //                                            Nome.getNumTuples(), 
   //                                            0, 
   //                                            Nome.getTupleSize());

   //  auto rPair = dRightRelation->build();
   //  auto lPair = dLeftRelation->build();

   //  unsigned indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

   //  //----using bin file

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Usando arquivo binario." << std::endl;
   //  std::cout << "***************************************************" << std::endl;

   //  std::pair<unsigned, unsigned> bFJoinWOI = bruteForceJoin(Cidade, Nome, 0, 0, false);
   //  std::pair<unsigned, unsigned> mSJoinWOI = mergeSortJoin(Cidade, Nome, 0, 0, false);
   //  std::pair<unsigned, unsigned> hsJoinWOI = hashJoin(Cidade, Nome, 0, 0, false);


   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

   //  std::cout << "---------------------------------------------------" << std::endl;
   //  std::cout << " Atributos nao-chave com atributos nao-chave." << std::endl;
   //  std::cout << "---------------------------------------------------" << std::endl << std::endl;

   // //----using index

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Usando indices existentes." << std::endl;
   //  std::cout << "***************************************************" << std::endl;
   //  std::pair<unsigned, unsigned> bFJoinWI = bruteForceJoin(Cidade, Nome, 0, 0, true);
   //  std::pair<unsigned, unsigned> mSJoinWI = mergeSortJoin(Cidade, Nome, 0, 0, true);
   //  std::pair<unsigned, unsigned> hsJoinWI = hashJoin(Cidade, Nome, 0, 0, true);

   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWI) << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWI) << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWI) << std::endl;

   //  //----creating index

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Criando indices." << std::endl;
   //  std::cout << "***************************************************" << std::endl;

   //  DenseIndex *dRightRelation = new DenseIndex(Cidade.getBinFilename(),
   //                                             Cidade.getNumTuples(), 
   //                                             0, 
   //                                             Cidade.getTupleSize());
   //  DenseIndex *dLeftRelation = new DenseIndex(Nome.getBinFilename(),
   //                                            Nome.getNumTuples(), 
   //                                            0, 
   //                                            Nome.getTupleSize());

   //  auto rPair = dRightRelation->build();
   //  auto lPair = dLeftRelation->build();

   //  unsigned indexTime = + computeTime(rPair.second) + computeTime(lPair.second);

   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWI) + indexTime << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWI) + indexTime << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWI) + indexTime << std::endl;

   //  //----using bin file

   //  std::cout << "***************************************************" << std::endl;
   //  std::cout << " Usando arquivo binario." << std::endl;
   //  std::cout << "***************************************************" << std::endl;

   //  std::pair<unsigned, unsigned> bFJoinWOI = bruteForceJoin(Cidade, Nome, 0, 0, false);
   //  std::pair<unsigned, unsigned> mSJoinWOI = mergeSortJoin(Cidade, Nome, 0, 0, false);
   //  std::pair<unsigned, unsigned> hsJoinWOI = hashJoin(Cidade, Nome, 0, 0, false);


   //  std::cout << "Forca bruta " << std::endl;
   //  std::cout << "Tempo = " << computeTime(bFJoinWOI) << std::endl; 
   //  std::cout << "Merge Sort Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(mSJoinWOI) << std::endl;
   //  std::cout << "Hash Join" << std::endl;
   //  std::cout << "Tempo = " << computeTime(hsJoinWOI) << std::endl;

    return 0;
}
