#ifndef BINFILEHANDLER_H
#define BINFILEHANDLER_H

#include <fstream>
#include <Header.h>

using namespace std;

class BinFileHandler
{
    private: //properties
        bool isOpen;

    private:    //methods
        void initAttributes();

        void setOpened(bool new_var){
            isOpen = new_var;
        }

        bool isOpened (){
            return isOpen;
        }

    public:
        ifstream input;
        ofstream output;
        BinFileHandler (string binFilename, bool readOnly);
        virtual ~BinFileHandler();
        void close(){ input.close(); output.close(); }
        void writeHeader(header &tupleHeader);
        void writeStrongType(string &buffer, unsigned short dataType);

    protected:

};

#endif // BINFILEHANDLER_H
