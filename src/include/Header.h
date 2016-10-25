#ifndef HEADER_H
#define HEADER_H

#include <ctime>

#define INT4 4
#define CHAR256 256
#define CHAR32 32

/**
 * Set the value of header
 * This struct hold the  tuple header's format and data. The header format is
 * the
 * same for all tuples in relations present in this DBMS.
 * @param new_var the new value of header
 */

struct header {
  char relName[12];
  unsigned int nTuple;
  time_t timeStamp; // epoch Time;
};

const unsigned short HEADER_SIZE = sizeof(header);

#endif // HEADER_H
