#include "Join.h"
#include <unordered_map> 
#include <iostream>

#define BLOCK_SIZE 4096
std::pair<unsigned, unsigned>
bruteForceJoin(DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, Relation& lRelation,
               Relation& rRelation, unsigned rPosition) {

	unsigned seek = 0, blocks = 0;


	const unsigned bytes = lRelation.getTupleSize() + 
		rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

	int k = 0;
	for (const auto & lElement: leftEntity -> index) {
		for (const auto & rElement: rightEntity -> index) {

			if (lElement.first == rElement.first) {
				string buffer;

				// move binfile stream to element
				leftHandler.input.seekg(lElement.second, leftHandler.input.beg);
				rightHandler.input.seekg(rElement.second, rightHandler.input.beg);
				seek += 2;

				// create joined tuple
				buffer += lRelation.readRegistry(leftHandler);
				buffer += rRelation.readRegistry(rightHandler, true, rPosition);
				buffer += '\n';
				
				blocks += bytes;

				// write output to disk
				outHandler.output.write(buffer.c_str(), buffer.size());
			}
			++k;
		}

	}
	std::cout << k << std::endl;
 	return std::make_pair(seek, blocks / BLOCK_SIZE);
}

std::pair<unsigned, unsigned>
mergeSortJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, Relation& lRelation,
               Relation& rRelation, unsigned rPosition) {

	unsigned seek = 0, blocks = 0;
	unsigned i = 0, j = 0;

	const unsigned bytes = lRelation.getTupleSize() + 
		rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

	int k = 0;

	while (i < leftEntity -> index.size() && j < rightEntity -> index.size()) {

		if (leftEntity->index[i].first < rightEntity->index[j].first) {++i; ++k;}

		else {
			j += leftEntity->index[i].first > rightEntity->index[j].first;
			++k;
		}

		// find all matching entries
		while (j < rightEntity -> index.size() && 
			leftEntity->index[i].first == rightEntity->index[j].first) {

			string buffer;

			// move binfile stream to element
			leftHandler.input.seekg(leftEntity->index[i].second, leftHandler.input.beg);
			rightHandler.input.seekg(rightEntity->index[j].second, rightHandler.input.beg);
			seek += 2;

			// create joined tuple
			buffer += lRelation.readRegistry(leftHandler);
			buffer += rRelation.readRegistry(rightHandler, true, rPosition);
			buffer += '\n';			
			blocks += bytes;

			// write output to disk
			outHandler.output.write(buffer.c_str(), buffer.size());

			++j;
			++k;
		}
	}
	std::cout << k << std::endl;
	return std::make_pair(seek, blocks / BLOCK_SIZE);
}


std::pair<unsigned, unsigned>
hashJoin (DenseIndex *leftEntity, DenseIndex *rightEntity,
               BinFileHandler& leftHandler, BinFileHandler& rightHandler,
               BinFileHandler& outHandler, const Relation& lRelation,
               Relation& rRelation, unsigned rPosition) {

	unsigned seek = 0, blocks = 0;

	std::unordered_multimap	<int, unsigned> table;

	const unsigned bytes = lRelation.getTupleSize() + 
		rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

	// partioning phase
	for (const auto & lElement : leftEntity -> index)
		table.insert(std::pair<int, unsigned> (lElement.first, lElement.second));

	Relation outRelation ("outRelation", lRelation.getTupleFormat(), 
		                  rRelation.getTupleFormat(), rPosition);

	// probing phase
	for (const auto & rElement : rightEntity -> index) {

		size_t bucketIndex = std::hash<int>{} (rElement.first);
		std::string buffer;

		for (auto localIt = table.begin(bucketIndex); 
			localIt != table.end(bucketIndex); ++localIt) {

			// move binfile stream to element
			leftHandler.input.seekg((*localIt).second, leftHandler.input.beg);
			rightHandler.input.seekg(rElement.second, rightHandler.input.beg);
			seek += 2;

			// create joined tuple
			buffer += lRelation.readRegistry(leftHandler);
			buffer += rRelation.readRegistry(rightHandler, true, rPosition);
			buffer += '\n';
			
			blocks += bytes;
		}
		outHandler.output.write(buffer.c_str(), buffer.size());
	}

	return std::make_pair(seek, blocks / BLOCK_SIZE);
}

unsigned computeTime(const std::pair<unsigned, unsigned> values) {
	return 10 * values.first + values.second;
}
