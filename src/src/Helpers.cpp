#include "Helpers.h"

#include <algorithm>
#include <fstream>

namespace MiniDB {
	bool compare_files(const std::string& filename1, const std::string& filename2) {
		// open files at the end
		std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary);
		std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary);

		if (file1.tellg() != file2.tellg()) { // different file sizes
			return false;
		}

		// rewind
		file1.seekg(0);
		file2.seekg(0);

		std::istreambuf_iterator<char> begin1(file1);
		std::istreambuf_iterator<char> begin2(file2);

		return std::equal(begin1, std::istreambuf_iterator<char>(), begin2); //second argument is end-of-range iterator
	}
}
