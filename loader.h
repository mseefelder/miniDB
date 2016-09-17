#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED
#include <fstream>
#include <vector>
#include <string.h>
#include <typeinfo>

//Codificação dos tipos de dados usada na interpreteção do argumento regFormat
// 1 = bool
// 4 = int
// 32 = char[31]
// 256 = char[255]

//Lê um arquivo .txt/.csv e grava um arquivo binário com o conteúdo

using namespace std;

void writeRightType(char* buffer, short dataType, ofstream& binFile){
    switch(dataType){
        case 1: {
            bool attr1;
            if ( strcasecmp(buffer,"true") == 0 ) attr1 = true;
            else attr1 = false;

            //DEBUG cout << attr1 << " " << typeid(attr1).name() << " " << sizeof(attr1) << endl;
            puts((char*)&attr1);
            binFile.write ((char*)&attr1, sizeof(attr1));
            break;
           }
        case 4: {
            int attr4 = atoi(buffer);

            //DEBUG cout << buffer << " " << attr4 << " " << typeid(attr4).name() << " " << sizeof(attr4) << endl;
            binFile.write ((char*)&attr4, sizeof(attr4));
            break;
           }
        case 32: {
            char attr32[32];
            strncpy(attr32, buffer, 31);
            attr32[31] = '\0';

            //DEBUG cout << attr32 << " " << typeid(attr32).name() << " " << sizeof(attr32) << endl;
            binFile.write (attr32, sizeof(attr32));
            break;
           }
        case 256:{
            char attr256[256];
            strncpy(attr256, buffer, 255);
            attr256[255] = '\0';

            //DEBUG cout << attr256 << " " << typeid(attr256).name() << " " << sizeof(attr256) << endl;
            binFile.write (attr256, sizeof(attr256));
            break;
           }
        default: ;
    }
}


//inFile -> arquivo de texto de entrada .txt/.csv;
//outFile -> arquivo binario de saida com registros estruturados;
//regsize = tamanho do registro
bool loadFile(string inFile, string outFile, short* regFormat, short attrCount) {

    ifstream stream(inFile);
    ofstream binFile (outFile, ios::out | ios::binary);

    if (stream.fail() || binFile.fail()) return false;

    string line;

    while(stream.good()) {

        getline(stream, line);

        char token='|';
        char buffer[256];
        short i = 0, attrN=0;

        for(auto c: line) {
            if(c != token && c!= '\r' && c!= '\n') buffer[i++] = c;
            else if (c == token && (attrN < attrCount)){
                buffer[i]='\0';
                writeRightType(buffer,regFormat[attrN],binFile);

                memset(buffer,0,sizeof(buffer));
                buffer[0]='\0';
                i = 0; attrN++;
            }
        }

        if ((i > 1) || attrN<attrCount) writeRightType(buffer,regFormat[attrN],binFile);
    }

    stream.close();
    binFile.close();

    return true;
}


//faz busca por chave
void selectAttribute(string inFile, short* regFormat, short regSize, int key, short attr){
 ifstream binFile (inFile, ios::in | ios::binary);

 if (binFile.good()){
    short regOffset = 0;
    for (short i=0;i<attr-1;i++) regOffset+=regFormat[i];

    binFile.seekg (regSize*(key-1) + regOffset, binFile.beg);
    switch(regFormat[attr-1]){
        case 1: {
            bool attr1;
            binFile.read ((char*)&attr1, sizeof(attr1));
            cout << attr1 << " " << typeid(attr1).name() << " " << sizeof(attr1) << endl;;
            break;
           }
        case 4: {
            int attr4;
            binFile.read ((char*)&attr4, sizeof(attr4));
            cout<< attr4 << " " << typeid(attr4).name() << " " << sizeof(attr4) << endl;
            break;
           }
        case 32: {
            char attr32[32];
            binFile.read (attr32, sizeof(attr32));
            cout << attr32 << " " << typeid(attr32).name() << " " << sizeof(attr32) << endl;
            break;
           }
        case 256:{
            char attr256[256];
             binFile.read (attr256, sizeof(attr256));
            cout << attr256 << " " << typeid(attr256).name() << " " << sizeof(attr256) << endl;
            break;
           }
        default: ;
    }
 }
 binFile.close();
}

#endif // LOADER_H_INCLUDED
