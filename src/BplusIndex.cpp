#include "BplusIndex.h"

// Constructors/Destructors
//

BplusIndex::BplusIndex () {
initAttributes();
}

BplusIndex::~BplusIndex () { }

//
// Methods
//

bool BplusIndex::build(){
}

pair<vector<unsigned int>,bool>  BplusIndex::getRangeTuple (int a, int b){
}

vector<pair<unsigned int,bool>> BplusIndex::getBatchTuple (vector<int> Ks){
}

pair<unsigned int,bool> BplusIndex::getTuple (int K){
}

// Accessor methods
//


// Other methods
//

void BplusIndex::initAttributes () {
}

