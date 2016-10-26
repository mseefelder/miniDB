#include <gtest/gtest.h>

#include <fstream>

#include "Helpers.h"

const char* const DUMMY1 = "foo";
const char* const DUMMY2 = "bar";

const char* const FILENAME1 = "file1.txt";
const char* const FILENAME2 = "file2.txt";

TEST(HelpersTest, EqualFilesTest) {
	std::ofstream file1(FILENAME1);
	file1 << DUMMY1 << std::endl;
	file1.close();

	std::ofstream file2(FILENAME2);
	file2 << DUMMY1 << std::endl;
	file2.close();

	EXPECT_TRUE(MiniDB::compare_files(FILENAME1, FILENAME2));
}

TEST(HelpersTest, DifferentFilesTest) {
	std::ofstream file1(FILENAME1);
	file1 << DUMMY1 << std::endl;
	file1.close();

	std::ofstream file2(FILENAME2);
	file2 << DUMMY2 << std::endl;
	file2.close();

	EXPECT_FALSE(MiniDB::compare_files(FILENAME1, FILENAME2));
}
