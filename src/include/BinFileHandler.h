#ifndef BINFILEHANDLER_H
#define BINFILEHANDLER_H

#include <Header.h>
#include <fstream>

using namespace std;

class BinFileHandler {
private: // properties
  bool isOpen;

private: // methods
  void initAttributes();

  void setOpened(bool new_var) { isOpen = new_var; }

public:
  ifstream input;
  ofstream output;
  BinFileHandler(string binFilename, bool readOnly);
  virtual ~BinFileHandler();
  void close() {
    input.close();
    output.close();
  }
  void writeHeader(header &tupleHeader);
  void writeStrongType(string &buffer, unsigned short dataType);

  bool isOpened() { return isOpen; }

protected:
};

#endif // BINFILEHANDLER_H
