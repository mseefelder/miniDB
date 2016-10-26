#include "Relation.h"

#include <fstream>
#include <typeinfo>

Relation::Relation (const std::string& schemaName, const std::vector<short>& tupleFormat) {
    initAttributes();
    setName(schemaName);
    setTupleFormat(tupleFormat);
    setNumAttr(tupleFormat.size());
    unsigned short sizeBytes = 0;
    for(auto i: tupleFormat) sizeBytes += i;
    setTupleSize(sizeBytes);
    binIn = new BinFileHandler(binFilename, true);
    binOut = new BinFileHandler(binFilename, false);
}

Relation::~Relation () {
    if (indexExists) {
        delete index;
    }
	//delete binIn;
    //delete binOut;
}

bool Relation::load (const std::string& csvFilename, char delimiter) {
    ifstream csvFile(csvFilename);
	
    header tHeader;
    strcpy(tHeader.relName, name);
    string line;
    unsigned int lineCounter = 0;

	if (!csvFile.good()) return false;

    while(csvFile.good()) {
        getline(csvFile, line);

        if (line.length() > 2) {    //ignoring empty lines with characters  \r and/or \n
            string buffer;
            unsigned short attrN = 0;

            tHeader.nTuple = ++lineCounter;
            //http://en.cppreference.com/w/cpp/chrono/c/time
            tHeader.timeStamp = time(nullptr);

            binOut->writeHeader(tHeader);

            for(const auto& c: line) {
                if(c != delimiter && c != '\r' && c!= '\n') buffer.push_back(c);
                else if (c == delimiter && (attrN < numAttr)){
                    buffer.push_back('\0');
                    binOut->writeStrongType(buffer,tupleFormat[attrN]);

                    //reseting buffer
                    buffer.clear();
                    attrN++;
                }
            }
            if (buffer.size() > 0 || attrN < numAttr){
                buffer.push_back('\0');
                binOut->writeStrongType(buffer,tupleFormat[attrN]);
            }
        }
   }
   csvFile.close();
   binOut->output.flush();
   setNumTuples(lineCounter);
   delete binIn;
   binIn = new BinFileHandler(binFilename, true);
}

bool Relation::open(){
    if (!binIn->isOpened()) {
        return false;
    }
    
    binIn->input.seekg(0, binIn->input.end);
    setNumTuples(binIn->input.tellg() / tupleSize);
    
    return true;
}

bool Relation::selAttr (int K, unsigned short attrN){
    //BinFileHandler binFile(binFilename, true);
    unsigned int i = 0;
    while (i < numTuples){
        //PT_BR: percorrendo a tabela, saltando registro a registro direto para 
        //o 1° atributo (considerado um inteiro que guarda chave primária K)
        binIn->input.seekg(i*tupleSize + HEADER_SIZE, binIn->input.beg);

        int tKey;
        binIn->input.read((char*)&tKey, sizeof(tKey));
        if (tKey == K) {  //registro com chave K encontrado
            int tupleBegByte = static_cast<int>(binIn->input.tellg())-(INT4+HEADER_SIZE);
            if (attrN == 0)
                printTuple(tupleBegByte);
            else
                printAttr(tupleBegByte, attrN);
            binIn->input.clear();
            return true;
        }
        i++;
    }
    binIn->input.clear();
    return false;
}

void Relation::printAttr(unsigned int tupleBegByte, unsigned short attrN){
    int attrOffset = 0;
    for (short i=0;i < attrN-1;i++) attrOffset += tupleFormat[i];
    binIn->input.seekg(tupleBegByte+HEADER_SIZE+attrOffset, binIn->input.beg);
    switch(tupleFormat[attrN]){
        case INT4: {
            int attr4;
            binIn->input.read ((char*)&attr4, sizeof(attr4));
            cout<< attr4 /*<< " " << typeid(attr4).name() << " " << sizeof(attr4) */<< '\t';
            break;
        }
        case CHAR32: {
            char attr32[32];
            binIn->input.read (attr32, sizeof(attr32));
            cout<< attr32 /*<< " " << typeid(attr32).name() << " " << sizeof(attr32) */<< '\t';
            break;
        }
        case CHAR256:{
            char attr256[256];
            binIn->input.read (attr256, sizeof(attr256));
            cout<< attr256 /*<< " " << typeid(attr256).name() << " " << sizeof(attr256) */<< '\t';
            break;
        }
    }
    std::cout << std::endl;
 }

 void Relation::printTuple(unsigned int tupleBegByte){
    binIn->input.seekg(tupleBegByte+HEADER_SIZE, binIn->input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
    for (int i=0; i < numAttr; i++){
        switch(tupleFormat[i]){
            case INT4: {
                int attr4;
                binIn->input.read ((char*)&attr4, sizeof(attr4));
                cout<< attr4 /*<< " " << typeid(attr4).name() << " " << sizeof(attr4) */<< '\t';
                break;
            }
            case CHAR32: {
                char attr32[32];
                binIn->input.read (attr32, sizeof(attr32));
                cout << attr32 /*<< " " << typeid(attr32).name() << " " << sizeof(attr32) */<< '\t';
                break;
            }
            case CHAR256:{
                char attr256[256];
                binIn->input.read (attr256, sizeof(attr256));
                cout << attr256 /*<< " " << typeid(attr256).name() << " " << sizeof(attr256) */<< '\t';
                break;
            }
        }
    }
    cout << endl;
 }


// Accessor methods
//


// Other methods
//

void Relation::initAttributes() {
    setIsLoaded(false);
    setNumAttr(0);
    setNumTuples(0);
    index = 0;
    indexExists = false;
}



//-----------------------------------------------------------------------------

Relation::Relation (const std::string& schemaName, const std::vector<short>& lFormat, 
                    const std::vector<short>& rFormat, unsigned rPosition) {
    initAttributes();
    setName(schemaName);

    setTupleFormat(lFormat);
    for (unsigned i = 0; i < rFormat.size(); ++i) {
        if (i != rPosition) tupleFormat.push_back(rFormat[i]);
    }

    setNumAttr(tupleFormat.size());
    unsigned short sizeBytes = 0;
    for(auto i: tupleFormat) sizeBytes += i;
    setTupleSize(sizeBytes);
    binOut = new BinFileHandler(binFilename, false);
}


std::vector<string>
Relation::readTuple (const bool &ignore, const unsigned &att) const {

    std::vector<std::string> buffer;

    binIn->input.seekg(HEADER_SIZE, binIn->input.cur);
    for (unsigned i = 0; i < numAttr; i++){

        switch(tupleFormat[i]){
            case INT4: {
                char attr4 [4];
                binIn->input.read (attr4, sizeof(attr4));
                if (!(ignore && i == att))
                    buffer.push_back(attr4);
                break;
            }
            case CHAR32: {
                char attr32 [32];
                binIn->input.read (attr32, sizeof(attr32));
                if (!(ignore && i == att))
                    buffer.push_back(attr32);
                break;
            }
            case CHAR256:{
                char attr256 [256];
                binIn->input.read (attr256, sizeof(attr256));
                if (!(ignore && i == att))
                    buffer.push_back(attr256);
                break;
            }
        }
    }
    return buffer;
}

void Relation::writeTuple (std::vector<std::string> buffer) {
    header tHeader;
    strcpy(tHeader.relName, name);
    tHeader.nTuple = ++numTuples;
    tHeader.timeStamp = time(nullptr);
    binOut->writeHeader(tHeader);
    for (unsigned i = 0; i < tupleFormat.size(); ++i)
        binOut->writeStrongType(buffer[i], tupleFormat[i]);
    binOut->output.flush();
}

vector<short> Relation::excludeColumn(size_t i) {
    auto tuple = tupleFormat;
    tuple.erase(tuple.begin() + i);
    return tuple;
}

unsigned Relation::getAttSize(unsigned position) {
    switch(tupleFormat[position]){
        case INT4:
            return 4;

        case CHAR32:
            return 32;

        case CHAR256: 
        default:
            return 256;
    }
}

bool Relation::loadOrBuildIndex(unsigned attrPos){
    if (indexExists) {
        delete index;
    }
    bool loaded = false;
    index = new DenseIndex(binFilename, numTuples, attrPos, tupleSize); //for while, DenseIndex works only for the 1st attribute, considering it as a INT4
    if (!index->load()) {
      cout << "Construindo DenseIndex" << endl;
      if(index->build().first) cout << "DenseIndex construido e gravado em disco com sucesso";
      else cout << "Falha na construcao ou gravacao do DenseIndex";
    }
    else {
        loaded = true;
        cout << "DenseIndex pre-existente no disco foi encontrado e carregado em memoria." << endl; // index.printIndex();
    }
    indexExists = true;
    return loaded;
}

bool Relation::hasIndex(unsigned attrPos){
    //std::cout << indexExists && (attrPos == index->getAttrPos()) << std::endl;
    return indexExists && (attrPos == index->getAttrPos());
}

DenseIndex* Relation::getIndex(){
    return index;
}
