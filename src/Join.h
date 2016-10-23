#ifndef JOIN_H
#define JOIN_H

#include "Relation.h"
#include "Index.h"
#include <utility>
/* A join receive two relations and returns the joined relation of both.*/

std::pair<unsigned, unsigned> bruteForceJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
											  BinFileHandler leftHandler, BinFileHandler rightHandler,
											  BinFileHandler outHandler,
											  Relation lRelation, Relation rRelation,
											  unsigned rPosition);

unsigned computeTime (const std::pair<unsigned, unsigned> values);

#endif // JOIN_H
