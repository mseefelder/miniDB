
#ifndef BPLUSINDEX_H
#define BPLUSINDEX_H
#include "Index.h"
#include "BinFileHandler.h"

#include "bpt.h"

/**
  * class BplusIndex
  *
  */

class BplusIndex : virtual public Index
{
public:

  // Constructors/Destructors
  //


  /**
   * Empty Constructor
   */
  BplusIndex (string relName, unsigned int numT, unsigned short attrK, unsigned short tSize);

  /**
   * Empty Destructor
   */
  virtual ~BplusIndex ();

  //Override
  virtual bool build ();

  virtual bool load();

  virtual bool writeOnDisk();

  virtual pair<vector<unsigned int>,bool>  getRangeTuple (int a, int b);

  virtual vector<pair<unsigned int,bool>> getBatchTuple (vector<int> Ks);

  virtual pair<unsigned int,bool> getTuple (int K);
  //


protected:

  // Static Protected attributes
  //

  // Protected attributes
  //

public:


  // Protected attribute accessor methods
  //

protected:

public:


  // Protected attribute accessor methods
  //

protected:


private:

  // Static Private attributes
  //

  // Private attributes
  //

  bpt::bplus_tree *index;
public:


  // Private attribute accessor methods
  //

private:

public:


  // Private attribute accessor methods
  //


  /**
   * Set the value of index
   * @param new_var the new value of index
   */
  void setIndex (bpt::bplus_tree new_var)   {
      index = &new_var;
  }

  /**
   * Get the value of index
   * @return the value of index
   */
  bpt::bplus_tree* getIndex ()   {
    return index;
  }
private:


  void initAttributes () ;

};

#endif // BPLUSINDEX_H
