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

  // clean file pointers
  lRelation.resetStream();
  rRelation.resetStream();
  
 	return std::make_pair(seek, blocks / BLOCK_SIZE);
}

std::pair<unsigned, unsigned>
mergeSortJoin (Relation& lRelation, Relation& rRelation,
               unsigned lPosition, unsigned rPosition,
               bool useIndex) {

    unsigned seek = 0, blocks = 0;

    const unsigned bytes = lRelation.getTupleSize() +
        rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

    std::string outName = generateJoinedSchemaName (lRelation.getName(),
                                                    rRelation.getName(), "MS");

    Relation outRelation (outName, lRelation.getTupleFormat(),
                          rRelation.getTupleFormat(), rPosition);

    const unsigned rSize = rRelation.getTupleSize();
    const unsigned lSize = lRelation.getTupleSize();
    const unsigned oSize = outRelation.getTupleSize();

    if (useIndex) {
        if (!(rRelation.hasIndex(rPosition + 1) && lRelation.hasIndex(lPosition + 1))) {
            std::cout << "No index strutcture for the desired attributes!" << std::endl;
            return std::make_pair (0,0);
        }

        unsigned i = 0, j = 0;
        const auto lIndex = lRelation.getIndex()->index;
        const auto rIndex = rRelation.getIndex()->index;

        while (i < lIndex.size() && j < rIndex.size()) {

            if (lIndex[i].first < rIndex[j].first) ++i;

            else {
                j += lIndex[i].first > rIndex[j].first;
            }

            // find all matching entries
            while (j < rIndex.size() &&
                   lIndex[i].first == rIndex[j].first) {

                // move binfile stream to element
                lRelation.binIn->input.seekg(lIndex[i].second,
                                             lRelation.binIn->input.beg);
                lRelation.binIn->input.seekg(lIndex[j].second,
                                             rRelation.binIn->input.beg);
                seek += 2;

                // read tuples from binary file
                std::vector<std::string> lTuple (lRelation.readTuple());
                const std::vector<std::string> rTuple (rRelation.readTuple(true, rPosition));

                // merge tuples for join
                lTuple.insert(lTuple.end(), rTuple.begin(), rTuple.end());
                outRelation.writeTuple(lTuple);
                blocks += bytes;

                ++j;
            }
        }
    }

    else {

        // read tuples from binary file
        std::vector<std::string> lTuple (lRelation.readTuple());
        std::vector<std::string> rTuple (rRelation.readTuple());
        blocks += rSize + lSize;

        while (lRelation.binIn->input.peek() != EOF &&
               rRelation.binIn->input.peek() != EOF) {

            if (lTuple[lPosition].compare(rTuple[rPosition]) < 0) {
                lTuple = lRelation.readTuple();
                ++seek;
                blocks += lSize;
            }

            else if (lTuple[lPosition].compare(rTuple[rPosition]) > 0) {
                rTuple = rRelation.readTuple();
                ++seek;
                blocks += rSize;
            }

            // find all matching tuples
            while (lRelation.binIn->input.peek() != EOF &&
                   rRelation.binIn->input.peek() != EOF &&
                   !lTuple[lPosition].compare(rTuple[rPosition])) {

                // merge tuples for join
                std::vector<std::string> oTuple (lTuple);
                for (unsigned i = 0; i < rTuple.size(); ++i) {
                    if (i != rPosition) {
                        oTuple.push_back(rTuple[i]);
                    }
                }

                // write output tuple and proceed algorithm
                outRelation.writeTuple(oTuple);
                rTuple = rRelation.readTuple();
                blocks += oSize + rSize;
                ++seek;
            }
        }
    }

  // clean file pointers
  lRelation.resetStream();
  rRelation.resetStream();

	return std::make_pair(seek, blocks / BLOCK_SIZE);
}


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
//}

unsigned computeTime(const std::pair<unsigned, unsigned> values) {
	return 10 * values.first + values.second;
}

std::string
generateJoinedSchemaName(const std::string &rName, const std::string &lName, 
						 const std::string& joinType) {
	return rName + joinType + lName;
}
