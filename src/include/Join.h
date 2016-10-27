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
bruteForceJoin (Relation& lRelation, Relation& rRelation,
                     unsigned lPosition, unsigned rPosition,
                     bool useIndex);

std::pair<unsigned, unsigned>
mergeSortJoin (Relation& lRelation, Relation& rRelation,
               unsigned lPosition, unsigned rPosition,
               bool useIndex);

std::pair<unsigned, unsigned>
    hashJoin (Relation& lRelation, Relation& rRelation,
              unsigned lPosition, unsigned rPosition,
              bool useIndex);

unsigned computeTime(const std::pair<unsigned, unsigned> values);

std::string
generateJoinedSchemaName(const std::string &rName, const std::string &lName, 
                               const std::string& joinType);

#endif // JOIN_H
