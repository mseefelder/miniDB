#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Relation.h"
#include "bpt.h"

using namespace std;

using namespace BPlusTree;

bool buildTree(string relBinFilename, unsigned int numTuples, unsigned int tupleSize) {
    BinFileHandler relFile(relBinFilename, true);
    cout << relBinFilename << endl;
    ifstream ifile;

    double key;
    string dataString;
    long count = 0;

    if (numTuples == 0) return false;

    unsigned int i = 0;
    while (i < numTuples){
        relFile.input.seekg(i*tupleSize + HEADER_SIZE, relFile.input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
        int tKey;
        relFile.input.read((char*)&tKey, sizeof(tKey));
        unsigned int tupleBegByte = relFile.input.tellg()-(INT4+HEADER_SIZE);

  cout << tKey << " - " << tupleBegByte << endl;

        // Insert the object into file
        insert(bRoot, DBObject(tKey, tupleBegByte));

        i++;
    }

    // Close the file
    relFile.close();
}

int main()
{
    vector<short> tF {INT4, CHAR256, CHAR32};
    string relationName = "IES";
    Relation IES(relationName, tF);
    IES.load("CSVs_de_Teste/lista_ies.csv",'|');
    cout << IES.getNumTuples() << endl;


    // DENSE INDEX CREATION

    /*DEBUG*/ //cout << IES.getName() << " " << IES.getNumAttr() << " " << IES.getTupleSize() <<  " " << IES.getNumTuples() << IES.getTupleFormat() << endl;
    DenseIndex dPKEY(IES.getBinFilename(), IES.getNumTuples(), 1, IES.getTupleSize()); //for while, DenseIndex works only for the 1st attribute, considering it as a INT4
    dPKEY.build();  //dPKEY.printIndex();
   // dPKEY.printIndex();
    // B+ TREE CREATION
    // Initialize the BPlusTree module
        cout << "Teste 1" << endl;
    Node::initialize();
    // Create a new tree
        cout << "Teste 2" << endl;
    bRoot = new Node();
    cout << "Teste 3" << endl;
    buildTree(IES.getBinFilename(), IES.getNumTuples(), IES.getTupleSize());

    //Almost half are keys known to be present on csvFile
    vector<int> randomKeys = {1988, 1305, 1518, 1817, 1119, 1753, 1329, 1154, 1159, 1895, 1633, 1627, 1593, 1013, 1191, 1560,
        1791, 1740, 1619, 1406, 1516, 1704, 1438, 1989, 1709, 1762, 1352, 1713, 1881, 1603, 1959, 1937, 1930, 1111, 1545,
        1847, 1156, 1279, 1827, 1846, 1667, 1568, 1947, 1882, 1786, 1144, 1614, 1043, 1590, 1094, 1272, 1102, 1832, 1522,
        1900, 1231, 1604, 1369, 1605, 1906, 1877, 1050, 1028, 1172, 1024, 1408, 1934, 1475, 1775, 1473, 1778, 1089, 1226,
        1994, 1030, 1072, 1513, 1444, 1020, 1412, 1160, 1580, 1341, 1335, 1147, 1419, 1984, 1661, 1165, 1997, 1120, 1491,
        1852, 1557, 1425, 1286, 1640, 1237, 1577, 1263};

    short TestCase=0, hits=0, miss=0;


    //1st test case: Direct Access //One select
    {
        cout << "Selecao de chave unica: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        if (IES.selAttr(586,0)) hits++; //when attrN = 0 in calling selAttr, ALL the attributes of the tuple are selected;
        else miss++;

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;


        hits = 0;
        miss = 0;
    }
    {
        cout << "Selecao de conjunto aleatorio de chaves: \n\n" << endl;

        auto begin = chrono::high_resolution_clock::now();

        for (short i = 0; i < randomKeys.size(); i++) {
                if (IES.selAttr(randomKeys[i],0)) hits++; //when attrN = 0 in calling selAttr, ALL the attributes of the tuple are selected;
                else miss++;
        }

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;


        cout << "Selecao de intervalo (faixa 4000 - 4100) de chaves: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        for (int K = 4000; K < 4100; K++){
            if (IES.selAttr(K,0)) hits++; //when attrN = 0 in calling selAttr, ALL the attributes of the tuple are selected;
            else miss++;
        }

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;


        cout << "Selecao de chave unica por Indice Denso: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        auto t = dPKEY.getTuple(586);
        if(t.second) {
            hits++;
            IES.printTuple(t.first);
        }
        else miss++;

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;


        cout << "Selecao de conjunto aleatorio de chaves por Indice Denso: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        for (auto t: dPKEY.getBatchTuple(randomKeys)){
            if (t.second){
                hits++;
                IES.printTuple(t.first);
            }
            else miss++;
        }

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;


        cout << "Selecao de intervalo (faixa 4000 - 4100) de chaves: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        auto ts = dPKEY.getRangeTuple(4000,4100);
        if (ts.second)
            for (auto tA: ts.first)
                IES.printTuple(tA);
        hits = ts.first.size();

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {

        hits = 0;
        miss = 0;



        cout << "Selecao de chave unica por Arvore B+: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        unsigned int loc = pointQuery(bRoot, 586);

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nEndereço da chave encontrada: " << loc << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;

        cout << "Selecao de conjunto aleatorio de chaves por Arvore B+: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        for (auto & it : randomKeys) {

            unsigned int hit = pointQuery(bRoot, it);

            if (hit == 0) miss++;

            else hits++;

        }

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << hits << endl;
        cout << "Numero de chaves nao encontradas: " << miss << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
    {
        hits = 0;
        miss = 0;


        cout << "Selecao de intervalo (faixa 4000 - 4100) de chaves: \n\n" << endl;
        auto begin = chrono::high_resolution_clock::now();

        vector<unsigned int> loc = windowQuery(bRoot, 4000,4100);

        auto end = chrono::high_resolution_clock::now();

        cout << "\n\nNumero de chaves encontradas: " << loc.size() << endl;
        cout << "A operacao durou: " << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " ms" << endl;
    }
        hits = 0;
        miss = 0;


    return 0;
}
