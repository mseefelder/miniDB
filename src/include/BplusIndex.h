#ifndef BPLUSINDEX_H
#define BPLUSINDEX_H

#include "BinFileHandler.h"
#include "Index.h"

#include "bpt.h"

class BplusIndex : virtual public Index {
public:
  BplusIndex(const std::string& relName, unsigned int numT, unsigned short attrK,
             unsigned short tSize);

  virtual ~BplusIndex();

  virtual std::pair<bool, std::pair<unsigned, unsigned> > build();

  virtual bool load();

  virtual bool writeOnDisk();

  virtual pair<vector<unsigned int>, bool> getRangeTuple(int a, int b);

  virtual vector<pair<unsigned int, bool>> getBatchTuple(const std::vector<int>& Ks);

  virtual pair<unsigned int, bool> getTuple(int K);

public:
  void setIndex(bpt::bplus_tree new_var) { index = &new_var; }

  bpt::bplus_tree* getIndex() { return index; }

private:
  bpt::bplus_tree *index;

  void initAttributes();
};

#endif // BPLUSINDEX_H
