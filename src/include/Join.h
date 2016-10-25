#ifndef JOIN_H
#define JOIN_H

#include <utility>

#include "Index.h"
#include "Relation.h"

/* All join functions must be called passing the smallest relation or
	the relation with the higher join factor as the left relation. This is
	better to perform joins due to disk block access when dealing with bin files
	strictly.*/
std::pair<unsigned, unsigned>
bruteForceJoin(DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, Relation& lRelation,
               Relation& rRelation, unsigned rPosition);

std::pair<unsigned, unsigned>
mergeSortJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, Relation& lRelation,
               Relation& rRelation, unsigned rPosition);

std::pair<unsigned, unsigned>
hashJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, const Relation& lRelation,
               Relation& rRelation, unsigned rPosition);

unsigned computeTime(const std::pair<unsigned, unsigned> values);

#endif // JOIN_H
