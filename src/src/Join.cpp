#include "Join.h"
#include <unordered_map>
#include <iostream>

#define BLOCK_SIZE 4096


std::pair<unsigned, unsigned>
bruteForceJoin (Relation& lRelation, Relation& rRelation,
                     unsigned lPosition, unsigned rPosition,
                     bool useIndex) {

	unsigned seek = 0, blocks = 0;

  std::string outName = generateJoinedSchemaName (rRelation.getName(), rPosition,
                                                  lRelation.getName(), lPosition,
                                                  "BF", useIndex);


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

      for (const auto & lElement: lRelation.getIndex()->index) {

          lRelation.binIn->input.seekg(lElement.second,
                                       lRelation.binIn->input.beg);
          std::vector<std::string> lTuple (lRelation.readTuple());
          blocks += lSize;
          ++seek;

          for (const auto & rElement: rRelation.getIndex()->index) {
              if (lElement.first == rElement.first) {

                  // handle binary files
                  rRelation.binIn->input.seekg(rElement.second,
                                              rRelation.binIn->input.beg);

                  const std::vector<std::string> rTuple (rRelation.readTuple(true, rPosition));

                  blocks += rSize;
                  ++seek;

                  // merge tuples for join
                  lTuple.insert(lTuple.end(), rTuple.begin(), rTuple.end());
                  outRelation.writeTuple(lTuple);
                  blocks += oSize;
              }
          }
      }
  }

  else {
      while (lRelation.binIn->input.good() &&
             lRelation.binIn->input.peek() != EOF) {

          // read outer tuple
          const std::vector<std::string> lTuple (lRelation.readTuple());
          blocks += lSize;
          ++seek;

          while (rRelation.binIn->input.good() &&
                 rRelation.binIn->input.peek() != EOF ) {

              // read inner tuple
              const std::vector<std::string> rTuple (rRelation.readTuple());
              blocks += rSize;
              ++seek;

              if (!lTuple[lPosition].compare(rTuple[rPosition])) {

                  // merge tuples for join
                  std::vector<std::string> oTuple (lTuple);
                  for (unsigned i = 0; i < rTuple.size(); ++i) {
                      if (i != rPosition) {
                          oTuple.push_back(rTuple[i]);
                      }
                  }
                  outRelation.writeTuple(oTuple);
                  blocks += oSize;
              }
          }
          rRelation.resetStream();
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

    std::string outName = generateJoinedSchemaName (rRelation.getName(), rPosition,
                                                    lRelation.getName(), lPosition,
                                                    "MS", useIndex);

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
                rRelation.binIn->input.seekg(rIndex[j].second,
                                             rRelation.binIn->input.beg);
                seek += 2;

                // read tuples from binary file
                std::vector<std::string> lTuple (lRelation.readTuple());
                const std::vector<std::string> rTuple (rRelation.readTuple(true, rPosition));
                blocks += lSize + rSize;

                // merge tuples for join
                lTuple.insert(lTuple.end(), rTuple.begin(), rTuple.end());
                outRelation.writeTuple(lTuple);
                blocks += oSize;
                ++j;
            }
        }
    }

    else {

        // read tuples from binary file
        std::vector<std::string> lTuple (lRelation.readTuple());
        std::vector<std::string> rTuple (rRelation.readTuple());
        blocks += rSize + lSize;
        seek += 2;

        int lAtt = std::stoi(lTuple[lPosition]);
        int rAtt = std::stoi(rTuple[rPosition]);

        while (lRelation.binIn->input.peek() != EOF &&
               rRelation.binIn->input.peek() != EOF) {


            if (lAtt < rAtt) {
                lTuple = lRelation.readTuple();
                ++seek;
                blocks += lSize;
            }

            else if (lAtt > rAtt) {
                rTuple = rRelation.readTuple();
                ++seek;
                blocks += rSize;
            }

            // find all matching tuples
            while (lAtt == rAtt) {

                // merge tuples for join
                std::vector<std::string> oTuple (lTuple);
                for (unsigned i = 0; i < rTuple.size(); ++i) {
                    if (i != rPosition) {
                        oTuple.push_back(rTuple[i]);
                    }
                }

                // write output tuple and proceed algorithm
                outRelation.writeTuple(oTuple);
                blocks += oSize;
                rTuple = rRelation.readTuple();
                rAtt = std::stoi(rTuple[rPosition]);
                blocks += rSize;
                ++seek;
            }
        }

    }

  // clean file pointers
  lRelation.resetStream();
  rRelation.resetStream();

  return std::make_pair(seek, blocks / BLOCK_SIZE);
}


std::pair<unsigned, unsigned>
hashJoin (Relation& lRelation, Relation& rRelation,
          unsigned lPosition, unsigned rPosition,
          bool useIndex) {

    unsigned seek = 0, blocks = 0;

    std::string outName = generateJoinedSchemaName (rRelation.getName(), rPosition,
                                                    lRelation.getName(), lPosition,
                                                    "Hs", useIndex);


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

        // hash with index
        std::unordered_multimap<int, unsigned> table;

        const auto lIndex = lRelation.getIndex()->index;
        const auto rIndex = rRelation.getIndex()->index;


        // partioning phase
        for (const auto &lElement : lIndex)
            table.insert(lElement);

        // probing phase
        for (const auto &rElement : rIndex) {

            // find element position in table
            const size_t bucketIndex = std::hash<int>{} (rElement.first);
            
            auto itRange = table.equal_range(bucketIndex);

            for(auto localIt = itRange.first; localIt != itRange.second; ++localIt) {

                // move binfile stream to element
                lRelation.binIn->input.seekg((*localIt).second,
                                             lRelation.binIn->input.beg);
                rRelation.binIn->input.seekg(rElement.second,
                                             rRelation.binIn->input.beg);
                seek += 2;

                // read tuples from binary file
                std::vector<std::string> lTuple (lRelation.readTuple());
                const std::vector<std::string> rTuple (rRelation.readTuple(true, rPosition));
                blocks += rSize + lSize;

                // merge tuples for join
                lTuple.insert(lTuple.end(), rTuple.begin(), rTuple.end());
                outRelation.writeTuple(lTuple);
                blocks += oSize;
            }
        }
    }

    else {

        // hash with tuple
        std::unordered_multimap<int, std::vector<std::string> > table;

        // partioning phase
        while (lRelation.binIn->input.peek() != EOF) {
            const std::vector<std::string> lTuple (lRelation.readTuple());
            table.insert(std::make_pair(std::stoi(lTuple[lPosition]),
                                        lTuple));
            blocks += lSize;
            ++seek;
        }

        // probing phase
        while (rRelation.binIn->input.peek() != EOF) {

            // find element position in table
            const std::vector<std::string> rTuple (rRelation.readTuple());
            blocks += rSize;
            ++seek;

            const size_t bucketIndex = std::hash<int>{} (std::stoi(rTuple[rPosition]));

            // iterate over all matching tuples
            auto itRange = table.equal_range(bucketIndex);

            for(auto localIt = itRange.first; localIt != itRange.second; ++localIt) {
                std::vector<std::string> oTuple ((*localIt).second);

                for (unsigned i = 0; i < rTuple.size(); ++i) {
                    if (i != rPosition) {
                        oTuple.push_back(rTuple[i]);
                    }
                }

                // merge tuples for join
                outRelation.writeTuple(oTuple);
                blocks += oSize;
            }
        }
    }
  lRelation.resetStream();
  rRelation.resetStream();

	return std::make_pair(seek, blocks / BLOCK_SIZE);
}

unsigned computeTime(const std::pair<unsigned, unsigned> values) {
	return 10 * values.first + values.second;
}

std::string
generateJoinedSchemaName(const std::string &rName, const int rPosition,
                         const std::string &lName, const int lPosition,
						             const std::string& joinType, bool &useIndex) {
	string name = lName + "_" + std::to_string(lPosition) + "_" + joinType;
	if (useIndex) name += "_INDEX_";
	else name += "_RAW_"; 
	name += rName + "_" + std::to_string(rPosition);
  return name;
}
