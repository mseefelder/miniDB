
#ifndef HASHINDEX_H
#define HASHINDEX_H
#include "Index.h"
#include "BinFileHandler.h"

#include <unordered_map>
#include <string>

/**
  * class HashIndex
  *
  */

class HashIndex : public Index
{

private:

  unordered_map<int,unsigned int> index;


public:

  // Constructors/Destructors
  //


  /**
   * Empty Constructor
   */
  HashIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize);

  /**
   * Empty Destructor
   */
  virtual ~HashIndex ();

  //Override
  virtual bool build ();

  virtual pair<vector<unsigned int>,bool>  getRangeTuple (int a, int b);

  virtual vector<pair<unsigned int,bool>> getBatchTuple (vector<int> Ks);

  virtual pair<unsigned int,bool> getTuple (int K);


public:

  /**
   * Get the value of index
   * @return the value of index
   */
  unordered_map<int,unsigned int> getIndex ()   {
    return index;
  }

  void printIndex();

private:


  void initAttributes () ;

};

#endif // HASHINDEX_H
