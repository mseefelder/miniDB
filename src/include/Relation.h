#ifndef RELATION_H
#define RELATION_H

#include "BinFileHandler.h"
#include "BplusIndex.h"
#include "DenseIndex.h"
#include "Header.h"
#include <algorithm>
#include <string.h>
#include <string>
#include <vector>

#include <iostream>

using namespace std;

/**
  * class Relation
  * This class models one table of the database.
  */

class Relation {

private: // properties
  char name[30];
  string nameStr;
  unsigned short numAttr;
  unsigned int numTuples;
  // Size in bytes of a tuple, considering the header.
  unsigned short tupleSize;
  // Tuple format, that is, datatype of each attribute.
  vector<short> tupleFormat;
  // Relative filesystem path of the binary file that contains the tuple data of
  // this relation.
  string binFilename;
  // This struct holds the tuple header's format. The header's format is the
  // same for all tuples in relations present in this DBMS.

  bool isLoaded;

public:
  vector<DenseIndex> denseIndex;
  vector<BplusIndex> bplusIndex;

  Relation(const std::string& schemaName, const std::vector<short>& tupleFormat);

  Relation (const std::string& schemaName, const std::vector<short>& lFormat, 
            const std::vector<short>& rFormat, unsigned rPosition);

  /**
   * Empty Destructor
   */
  virtual ~Relation();

  /**
   * This method fills a binary file (which will be a used as a database) with
   * data
   * present in a input CSV formatted file.
   * @return Boolean
   * @param  csvFile
   * @param  delimiter
   */
  bool load(const std::string& csvFilename, char delimiter);

  bool open();

  /**
   * This method selects a specific attribute of the tuple with the given key.
   * a is the "ath" attribute in the tuple.
   * When a is 0, all the tuple is selected.
   * @return Boolean
   * @param  K Numerical key index (for while, always the first attribute) of
   * the tuple.
   * @param  attrN Numerical position of the attribute in tuple
   */
  bool selAttr(int K, unsigned short attrN);

  /**
   * Get the value of name
   * @return the value of name
   */
  char *getName() { return name; }

  /**
   * Get the value of numAttr
   * @return the value of numAttr
   */
  unsigned short getNumAttr() const { return numAttr; }

  /**
   * Get the value of numTuple
   * @return the value of numTuple
   */
  unsigned int getNumTuples() const { return numTuples; }

  /**
   * Get the value of tupleSize
   * Size in bytes of a tuple, considering the header.
   * @return the value of tupleSize
   */
  unsigned short getTupleSize() const { return tupleSize; }

  /**
   * Get the value of tupleFormat
   * Tuple format, that is, datatype of each attribute.
   * @return the value of tupleFormat
   */
  vector<short> getTupleFormat() const {
    return tupleFormat;
  }

  /**
   * Get the value of binFilename
   * Relative filesystem path of the binary file that contains the tuple data of
   * this
   * relation.
   * @return the value of binFilename
   */
  string getBinFilename() { return binFilename; }

  void printTuple(unsigned int tupleBegByte); // prints a specific tuple,
                                              // requires the specific byte in
                                              // which the tuple begins

  void printAttr(unsigned int tupleBegByte, unsigned short attrN);

  void initAttributes();

  std::vector<string> readTuple (bool ignore, unsigned att) const;

  void writeTuple (std::vector<std::string> buffer);

  vector<short> excludeColumn(size_t i);

  unsigned getAttSize(unsigned position);

private: // methods
  void setIsLoaded(bool new_var) { isLoaded = new_var; }

  /**
   * Set the value of name
   * @param new_var the new value of name
   */
  void setName(const std::string& new_var) {
    memset(name, 0, sizeof(name));
    if (new_var != "")
      strncpy(name, new_var.c_str(), 11);
    else
      strncpy(name, string("UnknownData").c_str(), 11);
    name[11] = '\0';
    setBinFilename(string(name));
    nameStr = string(name);
  }

  /**
   * Set the value of tupleFormat
   * Tuple format, that is, datatype of each attribute.
   * @param new_var the new value of tupleFormat
   */
  void setTupleFormat(const std::vector<short>& new_var) { tupleFormat = new_var; }

  /**
   * Set the value of tupleSize
   * Size in bytes of a tuple, considering the header.
   * @param new_var the new value of tupleSize
   */
  void setTupleSize(unsigned short new_var) {
    tupleSize = new_var;
    tupleSize += HEADER_SIZE;
  }

  /**
  * Set the value of numAttr
  * @param new_var the new value of numAttr
  */
  void setNumAttr(unsigned short new_var) { numAttr = new_var; }

  /**
  * Set the value of numTuples
  * @param new_var the new value of numTuples
  */
  void setNumTuples(unsigned int new_var) { numTuples = new_var; }

  /**
  * Set the value of binFilename
  * Relative filesystem path of the binary file that contains the tuple data of
  * this
  * relation.
  * @param new_var the new value of binFilename
  */
  void setBinFilename(const std::string& new_var) { 
    binFilename = new_var + ".bin";
  }
};

#endif // RELATION_H
