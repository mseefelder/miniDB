#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
  * Abstract class Index, with pure abstract methods
  * This "interface" ties properties and methods common to all diff types of
 * index.
  */
class Index {

public:
  /**
   * Returns pair with address of the tuple, with the specified index, in the
   * relation's binary
   * file and a bool telling if the key was found.
   * @return unsigned int
   * @param  K
   */
  virtual pair<unsigned int, bool> getTuple(int K) = 0;

  /**
   * Returns a vector containing address of the tuples selected.
   * @return vector<int>
   * @param  Ks vector with the random keys to be found
   */
  virtual vector<pair<unsigned int, bool>> getBatchTuple(const std::vector<int>& Ks) = 0;

  /**
   * Returns a vector containing address of the tuples selected: a <= K <= b. In
   * this
   * first version, the method is also responsible by printing the tuples to
   * STDOUT.
   * @return vector<int>
   * @param  a
   * @param  b
   */
  virtual pair<vector<unsigned int>, bool> getRangeTuple(int a, int b) = 0;

  virtual bool build() = 0;

  virtual bool writeOnDisk() = 0;

  virtual bool load() = 0;

  virtual ~Index();

protected:
  string relBinFilename;
  string name;
  unsigned int numTuples;
  // Size in bytes of a tuple, considering the header.
  unsigned short tupleSize;
  // Relative filesystem path of the binary file that contains the tuple data of
  // this relation.
  string binFilename;
  unsigned short attrKey;
  unsigned short attrFormat;
  bool isBuilt;
  bool isOpened;

public:
  void setBinFilename(const std::string& new_var) { binFilename = new_var; }

  string getBinFilename() const { return binFilename; }

  string getRelBinFilename() const { return relBinFilename; }

  unsigned int getNumTuples() const { return numTuples; }

  void setNumTuples(unsigned int new_var) { numTuples = new_var; }

  unsigned short getAttrPos() { return attrKey; }
};

#endif // INDEX_H
