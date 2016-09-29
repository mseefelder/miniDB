
#ifndef BPLUSINDEX_H
#define BPLUSINDEX_H
#include "Index.h"

#include <string>
#include "BplusTree.h"

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
  BplusIndex ();

  /**
   * Empty Destructor
   */
  virtual ~BplusIndex ();

  //Override
  virtual bool build ();

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

  BplusTree index;
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
  void setIndex (BplusTree new_var)   {
      index = new_var;
  }

  /**
   * Get the value of index
   * @return the value of index
   */
  BplusTree getIndex ()   {
    return index;
  }
private:


  void initAttributes () ;

};

#endif // BPLUSINDEX_H
