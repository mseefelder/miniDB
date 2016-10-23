#include "Join.h"

std::pair<unsigned, unsigned> bruteForceJoin (DenseIndex *leftEntity, DenseIndex *rightEntity
											  const binFileHandler leftHandler, const binFileHandler rightHandler,
											  binFileHandler outHandler,
											  Relation lRelation, Relation rRelation,
											  unsigned rPosition) {

	unsigned seek, blocks = 0;


	const unsigned bytes = lRelation.getTupleSize() + rRelation.getTupleSize() - rRelation.getAttSize(rPosition);

	for (const auto & lElement: leftEntity -> index) {
		for (const auto & rElement: rightEntity -> index) {

			if (lElement.first == rElement.first) {

				string buffer;

				// move binfile stream to element
				leftHandler.input.seekg(lElement.second, leftHandler.input.beg);
				rightHandler.input.seekg(rElement.second, rightHandler.input.beg);
				seek += 2;

				// create joined entry
				buffer += lRelation.readRegistry(leftHandler);
				buffer += rRelation.readRegistry(rightHandler, true, rPosition);
				blocks += bytes;

				// write output to disk
				outHandler.output.write(buffer.c_str(), buffer.size());
			}
		}
	}

	return std::make_pair(seek, blocks);
}

unsigned computeTime (const std::pair<unsigned, unsigned> values) {

	return 10 * values.first + values.second;
}