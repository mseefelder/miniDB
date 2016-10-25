#ifndef BINFILEHANDLER_H
#define BINFILEHANDLER_H

#include <fstream>

#include "Header.h"

class BinFileHandler {
  bool isOpen;

  void initAttributes();
  void setOpened(bool new_var) { isOpen = new_var; }

public:
  std::ifstream input;
  std::ofstream output;
  BinFileHandler(const std::string& binFilename, bool readOnly);
  virtual ~BinFileHandler();
  
  void close() {
    input.close();
    output.close();
  }

  void writeHeader(header &tupleHeader);
  void writeStrongType(const std::string &buffer, unsigned short dataType);
  bool isOpened() const { return isOpen; }
};

#endif // BINFILEHANDLER_H