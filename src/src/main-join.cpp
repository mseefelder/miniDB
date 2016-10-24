#include <iostream>
#include <vector>
#include <string>
#include <chrono>

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
        cout << "Carregando dados para base binaria..." << endl;
        if(Cidade.load("../csv/cidades.csv",'|')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    cout << "Instanciando DenseIndex" << endl;
    DenseIndex denseCidade(Cidade.getBinFilename(), Cidade.getNumTuples(), 1, Cidade.getTupleSize()); //for while, DenseIndex works only for the 1st attribute, considering it as a INT4
    if (!denseCidade.load()) {
        cout << "Construindo DenseIndex" << endl;
        if(denseCidade.build()) cout << "DenseIndex construido e gravado em disco com sucesso";
        else cout << "Falha na construcao ou gravacao do DenseIndex" <<endl;
    }
    else cout << "DenseIndex pre-existente no disco foi encontrado e carregado em memoria." << endl; // denseCidade.printIndex();

    string relationNameNome = "Nome";
    Relation Nome(relationNameNome, tF);
    if(!Nome.open()){
        cout << "Carregando dados para base binaria..." << endl;
        if(Nome.load("../csv/nomes.csv",'|')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    cout << "Instanciando DenseIndex" << endl;
    DenseIndex denseNome(Nome.getBinFilename(), Nome.getNumTuples(), 1, Nome.getTupleSize()); //for while, DenseIndex works only for the 1st attribute, considering it as a INT4
    if (!denseNome.load()) {
        cout << "Construindo DenseIndex" << endl;
        if(denseNome.build()) cout << "DenseIndex construido e gravado em disco com sucesso";
        else cout << "Falha na construcao ou gravacao do DenseIndex" <<endl;
    }
    else cout << "DenseIndex pre-existente no disco foi encontrado e carregado em memoria." << endl; // denseNome.printIndex();

BinFileHandler bfh1 (denseCidade.getRelBinFilename(), true);
BinFileHandler bfh2 (denseNome.getRelBinFilename(), true);
BinFileHandler bfh3 ("bfJoin.bin",false);
BinFileHandler bfh4 ("msJoin.bin",false);
BinFileHandler bfh5 ("hsJoin.bin",false);

    pair<unsigned, unsigned> bFJoin = bruteForceJoin(&denseCidade, &denseNome, bfh1, bfh2, bfh3, Cidade, Nome, 0);
    pair<unsigned, unsigned> msJoin = bruteForceJoin(&denseCidade, &denseNome, bfh1, bfh2, bfh4, Cidade, Nome, 0);
    pair<unsigned, unsigned> hsJoin = bruteForceJoin(&denseCidade, &denseNome, bfh1, bfh2, bfh5, Cidade, Nome, 0);

    //cout<<bFJoin<<endl;

    return 0;
}
