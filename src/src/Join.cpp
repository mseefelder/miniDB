#include "Join.h"
#include <unordered_map>
#include <iostream>

#define BLOCK_SIZE 4096


std::pair<unsigned, unsigned>
bruteForceJoin (Relation& lRelation, Relation& rRelation,
                     unsigned lPosition, unsigned rPosition,
                     bool useIndex) {

	unsigned seek = 0, blocks = 0;

	const unsigned bytes = lRelation.getTupleSize() +
		rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

  std::string outName = generateJoinedSchemaName (lRelation.getName(),
                                                  rRelation.getName(), "BF");

	Relation outRelation (outName, lRelation.getTupleFormat(),
                        rRelation.getTupleFormat(), rPosition);



  if (useIndex) {
      if (!(rRelation.hasIndex(rPosition + 1) && lRelation.hasIndex(lPosition + 1))) {
              std::cout << "No index strutcture for the desired attributes!" << std::endl;
              return std::make_pair (0,0);
          }
      for (const auto & lElement: lRelation.getIndex()->index) {
          for (const auto & rElement: rRelation.getIndex()->index) {
              if (lElement.first == rElement.first) {
                  // move binfile stream to element
                  lRelation.binIn->input.seekg(lElement.second,
                                              lRelation.binIn->input.beg);
                  lRelation.binIn->input.seekg(rElement.second,
                                              rRelation.binIn->input.beg);
                  seek += 2;
                  // read tuples from binary file
                  std::vector<std::string> lTuple (lRelation.readTuple());
                  const std::vector<std::string> rTuple (rRelation.readTuple(true, rPosition));

                  // merge tuples for join
                  lTuple.insert(lTuple.end(), rTuple.begin(), rTuple.end());

                  ///*DEBUG*/cout << " checgou aqui 4.5 "<< *(lTuple.begin()) << " " << *(lTuple.end()) << endl;		  

                  outRelation.writeTuple(lTuple);

                  blocks += bytes;
              }
          }
      }
  }

  else {
      while (lRelation.binIn->input.good() &&
             lRelation.binIn->input.peek() != EOF) {
          while (rRelation.binIn->input.good() &&
                 rRelation.binIn->input.peek() != EOF ) {

              // read tuples from binary file
              const std::vector<std::string> lTuple (lRelation.readTuple());
              const std::vector<std::string> rTuple (rRelation.readTuple());

              if (!lTuple[lPosition].compare(rTuple[rPosition])) {

                  // merge tuples for join
                  std::vector<std::string> oTuple (lTuple);
                  for (unsigned i = 0; i < rTuple.size(); ++i) {
                      if (i != rPosition) {
                          oTuple.push_back(rTuple[i]);
                      }
                  }
                  outRelation.writeTuple(oTuple);
              }
          }
          rRelation.binIn->input.seekg(0, rRelation.binIn->input.beg);
      }
  }
 	return std::make_pair(seek, blocks / BLOCK_SIZE);
}

// std::pair<unsigned, unsigned>
// mergeSortJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
//                BinFileHandler& leftHandler, BinFileHandler& rightHandler,
//                BinFileHandler& outHandler, Relation& lRelation,
//                Relation& rRelation, unsigned rPosition) {

// 	unsigned seek = 0, blocks = 0;
// 	unsigned i = 0, j = 0;

// 	const unsigned bytes = lRelation.getTupleSize() +
// 		rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

// 	int k = 0;

// 	while (i < leftEntity -> index.size() && j < rightEntity -> index.size()) {

// 		if (leftEntity->index[i].first < rightEntity->index[j].first) {++i; ++k;}

// 		else {
// 			j += leftEntity->index[i].first > rightEntity->index[j].first;
// 			++k;
// 		}

// 		// find all matching entries
// 		while (j < rightEntity -> index.size() &&
// 			leftEntity->index[i].first == rightEntity->index[j].first) {

// 			string buffer;

// 			// move binfile stream to element
// 			leftHandler.input.seekg(leftEntity->index[i].second, leftHandler.input.beg);
// 			rightHandler.input.seekg(rightEntity->index[j].second, rightHandler.input.beg);
// 			seek += 2;

// 			// create joined tuple
// 			buffer += lRelation.readRegistry(leftHandler);
// 			buffer += rRelation.readRegistry(rightHandler, true, rPosition);
// 			buffer += '\n';
// 			blocks += bytes;

// 			// write output to disk
// 			outHandler.output.write(buffer.c_str(), buffer.size());

// 			++j;
// 			++k;
// 		}
// 	}
// 	std::cout << k << std::endl;
// 	return std::make_pair(seek, blocks / BLOCK_SIZE);
// }


// std::pair<unsigned, unsigned>
// hashJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
//                BinFileHandler& leftHandler, BinFileHandler& rightHandler,
//                BinFileHandler& outHandler, const Relation& lRelation,
//                Relation& rRelation, unsigned rPosition) {

// 	unsigned seek = 0, blocks = 0;

// 	std::unordered_multimap<int, unsigned> table;

// 	const unsigned bytes = lRelation.getTupleSize() + rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

// 	// partioning phase
// 	for (const auto &lElement : leftEntity->index)
// 		table.insert(std::pair<int, unsigned> (lElement.first, lElement.second));

// 	Relation outRelation ("outRelation", lRelation.getTupleFormat(),
// 		                  rRelation.getTupleFormat(), rPosition);

// 	// probing phase
// 	for (const auto &rElement : rightEntity->index) {
// 		const size_t bucketIndex = std::hash<int>{} (rElement.first);
// 		std::string buffer;

// 		for (auto localIt = table.begin(bucketIndex);
// 			localIt != table.end(bucketIndex); ++localIt) {

// 			// move binfile stream to element
// 			leftHandler.input.seekg((*localIt).second, leftHandler.input.beg);
// 			rightHandler.input.seekg(rElement.second, rightHandler.input.beg);
// 			seek += 2;

// 			// create joined tuple
// 			buffer += lRelation.readRegistry(leftHandler);
// 			buffer += rRelation.readRegistry(rightHandler, true, rPosition);
// 			buffer += '\n';

// 			blocks += bytes;
// 		}
// 		outHandler.output.write(buffer.c_str(), buffer.size());
// 	}

// 	return std::make_pair(seek, blocks / BLOCK_SIZE);
// }

unsigned computeTime(const std::pair<unsigned, unsigned> values) {
	return 10 * values.first + values.second;
}

std::string
generateJoinedSchemaName(const std::string &rName, const std::string &lName, 
						 const std::string& joinType) {
	return rName + joinType + lName;
}
