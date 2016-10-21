#include "BinFileHandler.h"

#include <iostream>
#include <typeinfo>
#include <string.h>
#include <cstdlib>
#include <cstring>

using namespace std;

BinFileHandler::BinFileHandler (string binFilename, bool readOnly) {
    initAttributes();
    if (readOnly) {
        input.open(binFilename, ifstream::in | ifstream::binary);
        setOpened(input.good());
    }
    else { //write only
        output.open(binFilename, ofstream::out | ofstream::binary);
        setOpened(output.good());
    }
}

BinFileHandler::~BinFileHandler(){
    input.close();
    output.close();
}

void BinFileHandler::writeHeader(header &tupleHeader){
    output.write ((char*)&tupleHeader, sizeof(tupleHeader));
}

void BinFileHandler::writeStrongType(string &buffer, unsigned short dataType){
    switch(dataType){
        case INT4: {
            int attr4 = atoi(buffer.c_str());
            output.write ((char*)&attr4, sizeof(attr4));
            break;
        }
        case CHAR32: {
            char attr32[32];
            strncpy(attr32, buffer.c_str(), 31);
            attr32[31] = '\0';
            output.write (attr32, sizeof(attr32));
            break;
        }
        case CHAR256:{
            char attr256[256];
            strncpy(attr256, buffer.c_str(), 255);
            attr256[255] = '\0';
            output.write (attr256, sizeof(attr256));
            break;
        }
    }
}

void BinFileHandler::initAttributes () {
    setOpened(false);
}
