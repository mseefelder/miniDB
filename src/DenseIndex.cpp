#include "DenseIndex.h"

// Constructors/Destructors
//

DenseIndex::DenseIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize)
 {
    relBinFilename = relName;
    attrKey = attrK;
    tupleSize = tSize;
    numTuples = numT;
    initAttributes();
}

DenseIndex::~DenseIndex () { }

//
// Methods
//

bool DenseIndex::build(){
    BinFileHandler relFile(relBinFilename, true);

    if (numTuples == 0) return false;

    unsigned int i = 0;
    while (i < numTuples){
        relFile.input.seekg(i*tupleSize + HEADER_SIZE, relFile.input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
        int tKey;
        relFile.input.read((char*)&tKey, sizeof(tKey));
        unsigned int tupleBegByte = static_cast<unsigned int>(relFile.input.tellg()) - (INT4+HEADER_SIZE);
        index.push_back(make_pair(tKey, tupleBegByte));
        i++;
    }
    relFile.close();
    sort(index.begin(), index.end()); //sorting index, by the first element and then, by the second. This is the behavior of DenseIndex, diff address that refs same key are grouped in index;
    isBuilt = true;
    return writeOnDisk();
}

bool DenseIndex::writeOnDisk(){ /*REF http://stackoverflow.com/questions/37676381/saving-vectorint-to-file */
    if (isBuilt) {
        ofstream indexFile(binFilename, ios::out | ios::trunc | ios::binary);
        for (auto key : index)
            indexFile.write(reinterpret_cast<const char *>(&key), sizeof(pair<int,unsigned int>));
        if (indexFile.good())
            return true;
    }
    return false;
}

bool DenseIndex::load(){ /* REF http://stackoverflow.com/questions/37676381/saving-vectorint-to-file */
    if (!isBuilt) {
        ifstream indexFile(binFilename, ios::in | ios::binary);
        if (!indexFile.good()) return false;
        while (indexFile) {
            pair<int,unsigned int> key;
            indexFile.read(reinterpret_cast<char *>(&key), sizeof(pair<int,unsigned int>));
            if (indexFile.eof()) break;
            index.push_back(key);
        }
        isOpened = true;
        return true;
    }
    else return false;
}

pair<vector<unsigned int>,bool> DenseIndex::getRangeTuple (int Ki, int Kf){
    pair<vector<unsigned int>,bool> IndexEntries;
    IndexEntries.second = false;

    pair<int,unsigned int> minIndexEntry = {Ki,0};
    auto it1 = lower_bound(index.begin(), index.end(), minIndexEntry); //STL lower_bound: binary search, equals or greater
    if (it1 != index.end() && ((*it1).first >= Ki)){
        IndexEntries.second=true;
    }

    pair<int,unsigned int> maxIndexEntry = {Kf,0};
    auto it2 = lower_bound(index.begin(), index.end(), maxIndexEntry); //STL lower_bound: binary search, equals or greater
    if (it2 != index.end() && ((*it2).first >= Kf)){
        for (auto i = it1; i != it2;i++)
            IndexEntries.first.push_back((*i).second);
    }
    else{
        for (auto i = it1; i != index.end();i++)
            IndexEntries.first.push_back((*i).second);
    }


    return IndexEntries;
}

vector<pair<unsigned int,bool>> DenseIndex::getBatchTuple (vector<int> Ks){
    vector<pair<unsigned int,bool>> IndexEntries;
    for (auto K: Ks){
      IndexEntries.push_back(getTuple(K));
    }
    return IndexEntries;
}

pair<unsigned int,bool> DenseIndex::getTuple (int K){ //http://www.cplusplus.com/reference/algorithm/binary_search/ http://www.cplusplus.com/reference/algorithm/lower_bound/
    pair<int,unsigned int> minIndexEntry = {K,0};
    auto it1 = lower_bound(index.begin(), index.end(), minIndexEntry);
    if (it1 != index.end() && ((*it1).first == K))
        return make_pair((*it1).second,true);
    else return make_pair(0,false);
}

void DenseIndex::printIndex(){
    for(auto i: index) cout << i.first << " - " << i.second << endl;
}

// Accessor methods
//


// Other methods
//

void DenseIndex::initAttributes () {
    isBuilt = false;
    isOpened = false;
    stringstream filename;
    filename << "dI_" << attrKey << "_"<< relBinFilename;
    binFilename = filename.str();

}

