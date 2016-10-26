#ifndef HELPERS_H
#define HELPERS_H

#include <string>

namespace MiniDB {
	/* 
	* Return true if the given files are equal, or false otherwise.
	* Based on http://stackoverflow.com/questions/15118661/in-c-whats-the-fastest-way-to-tell-whether-two-string-or-binary-files-are-di
	*/
	bool compare_files(const std::string& filename1, const std::string& filename2);
}

#endif
