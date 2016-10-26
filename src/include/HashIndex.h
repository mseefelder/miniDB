#ifndef HASHINDEX_H
#define HASHINDEX_H

#include "BinFileHandler.h"
#include "Index.h"

#include <unordered_map>

class HashIndex : public Index {
  std::unordered_map<int, unsigned int> index;

public:
  HashIndex(const std::string& relName, unsigned int numT, unsigned short attrK,
            unsigned short tSize);

  virtual ~HashIndex();

  virtual bool build();

  virtual bool writeOnDisk();

  virtual bool load();

  virtual pair<vector<unsigned int>, bool> getRangeTuple(int a, int b);

  virtual vector<pair<unsigned int, bool>> getBatchTuple(const std::vector<int>& Ks);

  virtual pair<unsigned int, bool> getTuple(int K);

public:
  /** Get the value of index. */
  unordered_map<int, unsigned int> getIndex() {
    return index;
  }

  void printIndex();

private:
  void initAttributes();
};

#endif // HASHINDEX_H
