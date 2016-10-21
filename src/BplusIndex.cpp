#include "BplusIndex.h"

// Constructors/Destructors
//

BplusIndex::BplusIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize) {
	relBinFilename = relName;
    attrKey = attrK;
    tupleSize = tSize;
    numTuples = numT;
	initAttributes();
}

BplusIndex::~BplusIndex () { }

//
// Methods
//

bool BplusIndex::build(){
	BinFileHandler relFile(relBinFilename, true);

    if (numTuples == 0) return false;

    unsigned int i = 0;
    while (i < numTuples){
        relFile.input.seekg(i*tupleSize + HEADER_SIZE, relFile.input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
        int tKey;
        relFile.input.read((char*)&tKey, sizeof(tKey));

        unsigned int tupleBegByte = relFile.input.tellg()-(INT4+HEADER_SIZE);

        //index->insert(bpt::key_t((char*)&tKey), tupleBegByte);
        index->insert((to_string(tKey)).c_str(), tupleBegByte);
        i++;
    }
    relFile.close();
}


bool BplusIndex::writeOnDisk(){
}

bool BplusIndex::load(){
}

pair<vector<unsigned int>,bool>  BplusIndex::getRangeTuple (int a, int b){
    if (b < a) {
        int temp = a;
        a = b;
        b = temp;
    }

    b = b - 1; //exclude last index

    int diff = b-a;
    bpt::key_t left = bpt::key_t((to_string(a)).c_str());
    bpt::key_t right = bpt::key_t((to_string(b)).c_str());
    unsigned int *values = new unsigned int[diff];

    int amount = index->search_range(&left, right, values, diff);

    pair<vector<unsigned int>,bool> result;
    result.second = false;
    if (amount <= 0) {
        result.first.push_back(0);
        delete [] values;
        return result;
    }
    result.second = true;
    for (int i = 0; i < amount; ++i) {
        result.first.push_back(values[i]);
    }

    delete [] values;
    return result;
}

vector<pair<unsigned int,bool>> BplusIndex::getBatchTuple (vector<int> Ks){
    vector<pair<unsigned int,bool>> IndexEntries;
    for (auto K: Ks){
      IndexEntries.push_back(getTuple(K));
    }
    return IndexEntries;
}

pair<unsigned int,bool> BplusIndex::getTuple (int K){
    unsigned int value = 0;
    //if(index->search(bpt::key_t((char*)&K), &value) == 0) {
    if(index->search((to_string(K)).c_str(), &value) == 0) {
        return make_pair(value, true);
    }
    else {
        return make_pair(0, false);
    }
}

// Accessor methods
//


// Other methods
//

void BplusIndex::initAttributes () {
	isBuilt = false;
    isOpened = false;
    stringstream filename;
    filename << "bpI_" << attrKey << "_"<< relBinFilename;
    binFilename = filename.str();
    index = new bpt::bplus_tree(binFilename.c_str(), true);
}


