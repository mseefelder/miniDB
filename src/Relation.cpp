#include "Relation.h"

#include <fstream>
#include <typeinfo>

// Constructors/Destructors
//

Relation::Relation (string schemaName, vector<short> tupleFormat) {
    initAttributes();
    setName(schemaName);
    setTupleFormat(tupleFormat);
    setNumAttr(tupleFormat.size());
    unsigned short sizeBytes = 0;
    for(auto i: tupleFormat) sizeBytes += i;
    setTupleSize(sizeBytes);
}

Relation::~Relation () { }

//
// Methods
//

bool Relation::load (string csvFilename, char delimiter)  {
   // header tupleHeader;
    //tupleHeader.relName = &name[0];

    ifstream csvFile(csvFilename);

    BinFileHandler binFile(binFilename, false);
    header tHeader;
    strcpy(tHeader.relName, name);
    string line;
    unsigned int lineCounter = 0;

    while(csvFile.good()) {
        getline(csvFile, line);
        if (line.length() > 2) {    //ignoring empty lines with characters  \r and/or \n
            string buffer;
            unsigned short attrN = 0;

            tHeader.nTuple = ++lineCounter;
            //http://en.cppreference.com/w/cpp/chrono/c/time
            tHeader.timeStamp = time(nullptr);

            binFile.writeHeader(tHeader);

            for(auto c: line) {
                if(c != delimiter && c != '\r' && c!= '\n') buffer.push_back(c);
                else if (c == delimiter && (attrN < numAttr)){
                    buffer.push_back('\0');
                    binFile.writeStrongType(buffer,tupleFormat[attrN]);

                    //reseting buffer
                    buffer.clear();
                    attrN++;
                }
            }
            if (buffer.size() > 0 || attrN < numAttr){
                buffer.push_back('\0');
                binFile.writeStrongType(buffer,tupleFormat[attrN]);
            }
        }
   }
   csvFile.close();
   binFile.close();
   setNumTuples(lineCounter);
}

bool Relation::open(){
    BinFileHandler binFile(binFilename, true);
    if (!binFile.isOpened()) return false;
    binFile.input.seekg (0, binFile.input.end);
    setNumTuples(binFile.input.tellg() / tupleSize);
    binFile.close();
    return true;
}


bool Relation::selAttr (int K, unsigned short attrN){
    BinFileHandler binFile(binFilename, true);
    unsigned int i = 0;
    while (i < numTuples){
        //PT_BR: percorrendo a tabela, saltando registro a registro direto para 
        //o 1° atributo (considerado um inteiro que guarda chave primária K)
        binFile.input.seekg(i*tupleSize + HEADER_SIZE, binFile.input.beg);

        int tKey;
        binFile.input.read((char*)&tKey, sizeof(tKey));
        if (tKey == K) {  //registro com chave K encontrado
            int tupleBegByte = static_cast<int>(binFile.input.tellg())-(INT4+HEADER_SIZE);
            if (attrN == 0)
                printTuple(tupleBegByte);
            else
                printAttr(tupleBegByte, attrN);
            binFile.close();
            return true;
        }
        i++;
    }
    binFile.close();
    return false;
}

void Relation::printAttr(unsigned int tupleBegByte, unsigned short attrN){
    BinFileHandler binFile(binFilename, true);
    int attrOffset = 0;
    for (short i=0;i < attrN-1;i++) attrOffset += tupleFormat[i];
    binFile.input.seekg(tupleBegByte+HEADER_SIZE+attrOffset, binFile.input.beg);
    switch(tupleFormat[attrN]){
        case INT4: {
            int attr4;
            binFile.input.read ((char*)&attr4, sizeof(attr4));
            cout<< attr4 /*<< " " << typeid(attr4).name() << " " << sizeof(attr4) */<< '\t';
            break;
        }
        case CHAR32: {
            char attr32[32];
            binFile.input.read (attr32, sizeof(attr32));
            cout<< attr32 /*<< " " << typeid(attr32).name() << " " << sizeof(attr32) */<< '\t';
            break;
        }
        case CHAR256:{
            char attr256[256];
            binFile.input.read (attr256, sizeof(attr256));
            cout<< attr256 /*<< " " << typeid(attr256).name() << " " << sizeof(attr256) */<< '\t';
            break;
        }
    }
    cout << endl;
    binFile.close();
 }

 void Relation::printTuple(unsigned int tupleBegByte){
    BinFileHandler binFile(binFilename, true);
    binFile.input.seekg(tupleBegByte+HEADER_SIZE, binFile.input.beg); //PT_BR: percorrendo a tabela, saltando registro a registro direto para o 1° atributo (considerado um inteiro que guarda chave primária K)
    for (int i=0; i < numAttr; i++){
        switch(tupleFormat[i]){
            case INT4: {
                int attr4;
                binFile.input.read ((char*)&attr4, sizeof(attr4));
                cout<< attr4 /*<< " " << typeid(attr4).name() << " " << sizeof(attr4) */<< '\t';
                break;
            }
            case CHAR32: {
                char attr32[32];
                binFile.input.read (attr32, sizeof(attr32));
                cout << attr32 /*<< " " << typeid(attr32).name() << " " << sizeof(attr32) */<< '\t';
                break;
            }
            case CHAR256:{
                char attr256[256];
                binFile.input.read (attr256, sizeof(attr256));
                cout << attr256 /*<< " " << typeid(attr256).name() << " " << sizeof(attr256) */<< '\t';
                break;
            }
        }
    }
    cout << endl;
    binFile.close();
 }


// Accessor methods
//


// Other methods
//

void Relation::initAttributes () {
    setIsLoaded(false);
    setNumAttr(0);
    setNumTuples(0);
}
