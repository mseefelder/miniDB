
#ifndef DENSEINDEX_H
#define DENSEINDEX_H
#include "BinFileHandler.h"
#include "Index.h"

#include <algorithm> // std::sort

/**
  * class DenseIndex
  *
  */

class DenseIndex : public Index {

  // private:

public:
  vector<pair<int, unsigned int>> index;
  // Constructors/Destructors
  //

  /**
   * Empty Constructor
   */
  DenseIndex(string relName, unsigned int numT, unsigned short attrK,
             unsigned short tSize);

  /**
   * Empty Destructor
   */
  virtual ~DenseIndex();

  // Override
  virtual bool build();

  virtual bool load();

  virtual bool writeOnDisk();

  virtual pair<vector<unsigned int>, bool> getRangeTuple(int a, int b);

  virtual vector<pair<unsigned int, bool>> getBatchTuple(vector<int> Ks);

  virtual pair<unsigned int, bool> getTuple(int K);

public:
  /**
   * Set the value of index
   * @param new_var the new value of index
   */
  void setIndex(vector<pair<int, unsigned int>> new_var) { index = new_var; }

  /**
   * Get the value of index
   * @return the value of index
   */
  vector<pair<int, unsigned int>> getIndex() { return index; }

  void printIndex();

private:
  void initAttributes();
};

#endif // DENSEINDEX_H
