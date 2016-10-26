#ifndef DENSEINDEX_H
#define DENSEINDEX_H

#include "BinFileHandler.h"
#include "Index.h"

#include <algorithm>

class DenseIndex : public Index {
public:
  std::vector<std::pair<int, unsigned int>> index;

  DenseIndex(const std::string& relName, unsigned int numT, unsigned short attrK,
             unsigned short tSize);
  virtual ~DenseIndex();

  virtual std::pair<bool, std::pair<unsigned, unsigned> > build();

  virtual bool load();

  virtual bool writeOnDisk();

  virtual pair<vector<unsigned int>, bool> getRangeTuple(int a, int b);

  virtual vector<pair<unsigned int, bool>> getBatchTuple(const std::vector<int>& Ks);

  virtual pair<unsigned int, bool> getTuple(int K);

  void setIndex(vector<pair<int, unsigned int>> new_var) { index = new_var; }

  std::vector<std::pair<int, unsigned int>> getIndex() { return index; }

  void printIndex();

private:
  void initAttributes();
};

#endif // DENSEINDEX_H
