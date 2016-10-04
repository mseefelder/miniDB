#include "HashIndex.h"
#include <vector>
#include <iostream>
// Constructors/Destructors
//

HashIndex::HashIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize) {
    initAttributes();
    relBinFilename = relName;
    attrKey = attrK;
    tupleSize = tSize;
    numTuples = numT;
}

HashIndex::~HashIndex () { }

//
// Methods
//

bool HashIndex::build(){
    BinFileHandler relFile(relBinFilename, true);

    if (numTuples == 0) return false;

    unsigned int i = 0;
    while (i < numTuples){
        relFile.input.seekg(i*tupleSize + HEADER_SIZE, relFile.input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
        int tKey;
        relFile.input.read((char*)&tKey, sizeof(tKey));
        unsigned int tupleBegByte = relFile.input.tellg()-(INT4+HEADER_SIZE);
        index.emplace(tKey, tupleBegByte);
        i++;
    }
    relFile.close();
}

pair<vector<unsigned int>,bool> HashIndex::getRangeTuple (int Ki, int Kf){
    pair<vector<unsigned int>,bool> IndexEntries;
    IndexEntries.second = false;

    for (int K = Ki; K <= Kf; K++){
      auto t = getTuple(K);
      if (t.second)
        IndexEntries.first.push_back(t.first);
    }

    if (IndexEntries.first.size() > 0) IndexEntries.second = true;

    return IndexEntries;
}

vector<pair<unsigned int,bool>> HashIndex::getBatchTuple (vector<int> Ks){
    vector<pair<unsigned int,bool>> IndexEntries;
    for (auto K: Ks){
      IndexEntries.push_back(getTuple(K));
    }
    return IndexEntries;
}

pair<unsigned int,bool> HashIndex::getTuple (int K){ //http://www.cplusplus.com/reference/unordered_map/unordered_map/find/
  auto IndexEntry = index.find (K);

  if ( IndexEntry == index.end() )
    return make_pair(0,false);
  else
    return make_pair((*IndexEntry).second,true);
}

void HashIndex::printIndex(){
    for(auto i: index) cout << i.first << " - " << i.second << endl;
}

// Accessor methods
//


// Other methods
//

void HashIndex::initAttributes () {
}

