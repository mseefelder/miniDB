#include "HashTable.h"

#define DEFAULT_SIZE 13

// Constructs the empty Hash Table object.
// Array length is set to 13 by default.
HashTable::HashTable( int tableLength )
{
    if (tableLength <= 0) tableLength = DEFAULT_SIZE;
    array = new LinkedList[ tableLength ];
    length = tableLength;
}

// Returns an array location for a given item key.
int HashTable::hash( int itemKey )
{
    return itemKey % length;
}

// Adds an item to the Hash Table.
void HashTable::insertItem( Item * newItem )
{
    int index = hash( newItem -> key );
    array[ index ].insertItem( newItem );
}

// Deletes an Item by key from the Hash Table.
// Returns true if the operation is successful.
bool HashTable::removeItem( int itemKey )
{
    int index = hash( itemKey );
    return array[ index ].removeItem( itemKey );
}

// Returns an item from the Hash Table by key.
// If the item isn't found, a null pointer is returned.
Item * HashTable::getItemByKey( int itemKey )
{
    int index = hash( itemKey );
    return array[index].getItem( itemKey );
}

// Display the contents of the Hash Table to console window.
void HashTable::printTable()
{
    cout << "\nHash Table:\n";
    for (int i = 0; i < length; i++)
    {
        cout << "Bucket " << i+1 << ": ";
        array[i].printList();
    }
}

// Prints a histogram illustrating the Item distribution.
void HashTable::printHistogram()
{
    cout << "\n\nHash Table Contains ";
    cout << getNumberOfItems() << " Items total\n";
    for (int i = 0; i < length; i++)
    {
        cout << i + 1 << ":\t";
        for (int j = 0; j < array[i].getLength(); j++)
            cout << " X";
        cout << "\n";
    }
}

// Returns the number of locations in the Hash Table.
int HashTable::getLength()
{
    return length;
}

// Returns the number of Items in the Hash Table.
int HashTable::getNumberOfItems()
{
    int itemCount = 0;
    for (int i = 0; i < length; i++)
        itemCount += array[i].getLength();
    return itemCount;
}

// De-allocates all memory used for the Hash Table.
HashTable::~HashTable()
{
    delete [] array;
}

//*****************************************************************
// End of File
//*****************************************************************

