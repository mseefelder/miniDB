#include "HashIndex.h"

// Constructors/Destructors
//

HashIndex::HashIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize) {
    relBinFilename = relName;
    attrKey = attrK;
    tupleSize = tSize;
    numTuples = numT;
    initAttributes();
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
        unsigned int tupleBegByte = static_cast<unsigned int>(relFile.input.tellg())-(INT4+HEADER_SIZE);
        index.emplace(tKey, tupleBegByte);
        i++;
    }
    relFile.close();
    isBuilt = true;
    writeOnDisk();
}


bool HashIndex::writeOnDisk() {
    if (isBuilt) {
        std::ofstream indexFile(binFilename, ios::out | ios::trunc | ios::binary);
        for (const auto& key : index)
            indexFile.write(reinterpret_cast<const char *>(&key), sizeof(pair<int,unsigned int>));
        return true;
    }
    else return false;
}

bool HashIndex::load() {
    if (!isBuilt) {
        ifstream indexFile(binFilename, ios::in | ios::binary);
        if (!indexFile.good()) return false;
        while (indexFile) {
            pair<int,unsigned int> key;
            indexFile.read(reinterpret_cast<char *>(&key), sizeof(pair<int,unsigned int>));
            if (indexFile.eof()) break;
            index.insert(key);
        }
        isOpened = true;
        return true;
    }
    else return false;
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

std::vector<std::pair<unsigned int,bool>> HashIndex::getBatchTuple (const std::vector<int>& Ks){
    std::vector<std::pair<unsigned int, bool>> IndexEntries;

    for (const auto& K: Ks) {
      IndexEntries.push_back(getTuple(K));
    }

    return IndexEntries;
}

std::pair<unsigned int, bool> HashIndex::getTuple (int K) {
  const auto IndexEntry = index.find(K);

  if (IndexEntry == index.end())
    return make_pair(0, false);
  else
    return make_pair((*IndexEntry).second, true);
}

void HashIndex::printIndex(){
    for(const auto& i: index) {
        std::cout << i.first << " - " << i.second << std::endl;
    }
}

// Accessor methods
//


// Other methods
//

void HashIndex::initAttributes () {
    isBuilt = false;
    isOpened = false;
    stringstream filename;
    filename << "hI_" << attrKey << "_"<< relBinFilename;
    binFilename = filename.str();

}

