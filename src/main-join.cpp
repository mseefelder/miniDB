#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Relation.h"
#include "bpt.h"
#include "HashIndex.h"

using namespace std;

int main()
{
    vector<short> tF {INT4, CHAR256, CHAR32};
    string relationName = "IES";
    Relation IES(relationName, tF);
    if(!IES.open()){
        cout << "Carregando dados para base binaria..." << endl;
        if(IES.load("../csv/lista_ies.csv",'|')) cout << "Carregamento de dados efetuado com sucesso!" << endl;
        else { cout << "Falha no carregamento da base de dados." << endl; return 1; }
    }
    else cout << "Base de dados binaria pre-existente no disco foi encontrada. O carregamento e desnecessario." << endl;

    cout << "Instanciando DenseIndex" << endl;
    DenseIndex dPKEY(IES.getBinFilename(), IES.getNumTuples(), 1, IES.getTupleSize()); //for while, DenseIndex works only for the 1st attribute, considering it as a INT4
    if (!dPKEY.load()) {
        cout << "Construindo DenseIndex" << endl;
        if(dPKEY.build()) cout << "DenseIndex construido e gravado em disco com sucesso";
        else cout << "Falha na construcao ou gravacao do DenseIndex" <<endl;
    }
    else cout << "DenseIndex pre-existente no disco foi encontrado e carregado em memoria." << endl; // dPKEY.printIndex();

    return 0;
}
