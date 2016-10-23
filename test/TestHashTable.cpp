#include <gtest/gtest.h>

#include "LinkedList.h"
#include "HashTable.h"

TEST(HashTableTest, LookupTest) {
	// Create 26 Items to store in the Hash Table.
	Item *A = new Item(1, false, NULL);
	Item *B = new Item(2, false, NULL);
	Item *C = new Item(3, false, NULL);

	// Create a Hash Table of 13 Linked List elements.
	HashTable table;

	// Add 3 Items to Hash Table.
	table.insertItem(A);
	table.insertItem(B);
	table.insertItem(C);
	EXPECT_EQ(table.getNumberOfItems(), 3);

	// Remove one item from Hash Table.
	table.removeItem(1);
	EXPECT_EQ(table.getNumberOfItems(), 2);
	
	// Look up an Item in the Hash Table.
	Item *result = table.getItemByKey(3);
	EXPECT_EQ(result->key, 3);
}
