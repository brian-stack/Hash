/*************************************************************************************************************************
 * Author: Brian Stack
 * Assignment: Hash Tables
 * Date: 10/22/18
 * Class: CS 8
 * CRN: 74231
 * ***********************************************************************************************************************
 * This program provides an interactive test that allows the user to test the current hashtable.
 *  Also a random test is defined, in which, a certain number of Records with random keys and data will be
 *  inserted into the hashtable,the keys will then be searched for, then keys that are known to not exist in the
 *  table are searched for. This will demonstrate that the records can be correctly stored and later
 *  retrieved from the hashtable, based on their key.
 *
 *  - Test flags:
 *      * RANDOM_CHAINED      : A chainedhash will be created with table size = 100517.
 *      * RANDOM_OPEN         : An openhash will be created with table size = 100517.
 *      * RANDOM_DOUBLE       : A doublehash will be created with table size = 100517.
 *      * INTERACTIVE_DOUBLE  : A doublehash will be created with table size = 17.
 *      * INTERACTIVE_CHAINED : A chainedhash will be created with table size = 5.
 *      * INTERACTIVE_OPEN    : A openhash will be created with table size = 17.
 *
 ************************************************************************************************************************/
#include "chainedhash.h"
#include "doublehash.h"
#include "openhash.h"
using namespace std;

//preconditions: hash must be initialized.
//postconditions: this interactive hash table test allows the
//       user to preform various operations on the hash table
template<typename T>
void testHashTableInteractive(T& hash);

//preconditions: hash must be initialized, items > 0.
//postconditions: items Records with random keys and data will be
// inserted into the recieved hashtable,the keys will then be
// searched for, then keys that are known to not exist in the
// table are searched for.
template<typename T>
void testHashTableRandom(T& hash, size_t items, string& str);

//preconditions: none
//postconditions: a valid menu selection from cin is returned.
char getMenuSelection(string &prompt, string &validEntries);

//preconditions: none
//postconditions: a number between min and max is obtained from cin and returned.
int getNumberInRange(int min, int max);

//Toggle what tests to run.
const bool RANDOM_CHAINED = true;
const bool RANDOM_DOUBLE = true;
const bool RANDOM_OPEN = true;
const bool INTERACTIVE_DOUBLE = true;
const bool INTERACTIVE_CHAINED = false;
const bool INTERACTIVE_OPEN = false;

//The table size for random tests.
const size_t TABLE_SIZE = 100517;

int main()
{
    cout<<endl<<endl<<endl<<"----------------------"<<endl<<endl<<endl;

    if (INTERACTIVE_DOUBLE){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        //. . . . . .  Double Hash Table . . . . . . . . . . .;
        DoubleHash<Record<int> > doubleHash(17);
        testHashTableInteractive(doubleHash);
    }
    if (INTERACTIVE_CHAINED){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        //. . . . . .  Chained Hash Table . . . . . . . . . . .;
        ChainedHash<Record<int> > chainedHash(5);
        testHashTableInteractive(chainedHash);
    }
    if (INTERACTIVE_OPEN){
        cout<<"-------  INTERACTIVE TESTS ---------------------------"<<endl;
        //. . . . . .  Open Hash Table . . . . . . . . . . .;
        OpenHash<Record<int> > openHash(17);
        testHashTableInteractive(openHash);
    }
    if (RANDOM_OPEN){
        //----------- RANDOM TEST ------------------------------
        //. . . . . .  Open Hash Table . . . . . . . . . . .;
        size_t itemsToInsert = TABLE_SIZE / 10;
        string message = "Open Hash: Table Size = " +
                to_string(TABLE_SIZE) + " : Insertions = " + to_string(itemsToInsert);
        OpenHash<Record<int> > openHash(TABLE_SIZE);
        testHashTableRandom(openHash, itemsToInsert,message);
    }
    if (RANDOM_CHAINED){
        //----------- RANDOM TEST ------------------------------
        //. . . . . .  Chained Hash Table . . . . . . . . . . .;

        size_t itemsToInsert = TABLE_SIZE / 10;
        string message = "Chained Hash: Table Size = " +
                to_string(TABLE_SIZE) + " : Insertions = " + to_string(itemsToInsert);
        ChainedHash<Record<int> > chained(TABLE_SIZE);
        testHashTableRandom(chained, itemsToInsert,message);
    }
    if (RANDOM_DOUBLE){
        //----------- RANDOM TEST ------------------------------
        //. . . . . .  Double Hash Table . . . . . . . . . . .;
        size_t itemsToInsert = TABLE_SIZE / 10;
        string message = "Double Hash: Table Size = " +
                to_string(TABLE_SIZE) + " : Insertions = " + to_string(itemsToInsert);
        DoubleHash<Record<int> > doubleHash(TABLE_SIZE);
        testHashTableRandom(doubleHash, itemsToInsert,message);
    }

    cout<<endl<<endl<<endl<<"---------------------------------"<<endl;
}

//preconditions: hash must be initialized.
//postconditions: this interactive hash table test allows the user to preform various operations on the hash table
template<typename T>
void testHashTableInteractive(T& hash)
{
    bool exit = false;
    static string validSelections = "sirdf?x";
    static string theMenu = "[S]ize() [I]nsert [R]andom [D]elete [F]ind value [?]exists?   e[X]it: ";
    const int MAX_KEY = 10000;
    const int MAX_VAL = 1000;

    while(!exit)
    {
        //obtain a valid menu selection from cin.
        switch(getMenuSelection(theMenu,validSelections))
        {
        case 's':
        {
            cout << "Size: " << hash.size() << endl;
            break;
        }
        case 'i':
        {
            cout << "Key: ";
            int theKey = getNumberInRange(1,MAX_KEY);
            cout << "Value: ";
            int theValue = getNumberInRange(1,MAX_VAL);
            Record<int> theRecord(theKey,theValue);
            cout << "-- Inserting " <<  theRecord << endl;
            hash.insert(theRecord);
            break;
        }
        case 'r':
        {
            Record<int> theRecord((rand() % MAX_KEY) + 1 ,(rand() % MAX_VAL) + 1 );
            cout << "-- Inserting " <<  theRecord << endl;
            hash.insert(theRecord);
            break;
        }
        case 'd':
        {
            cout << "Enter the key of the item to delete: ";
            int theKey = getNumberInRange(1,MAX_KEY);

            if(hash.remove(theKey))
                cout << theKey << " removed." << endl;
            else
                cout << theKey << " not found." << endl;
            break;
        }
        case 'f':
        {
            cout << "Enter the key of the item to find: ";
            int theKey = getNumberInRange(1,MAX_KEY);
            Record<int> result;
            bool foundItem;
            hash.find(theKey,foundItem,result);

            if(foundItem)
                cout << result << " found." << endl;
            else
                cout << "item with key: " << theKey << " not found." << endl;
            break;
        }
        case '?':
        {
            cout << "Enter the key of the item to find: ";
            int theKey = getNumberInRange(1,MAX_KEY);
            if(hash.is_present(theKey))
                cout << theKey << " exists." << endl;
            else
                cout <<"not found." << endl;
            break;
        }
        case 'x':
        {
           exit = true;
           break;
        }
        }

        //Print the Hash to reflect any changes that may have occured.
       cout << hash << endl << "=========================" << endl;
    }
}

//preconditions: hash must be initialized, items > 0.
//postconditions: items Records with random keys and data wll be inserted into the recieved hashtable,
// the keys will then be searched for, then keys that are known to not exist in the table are searched for.
template<typename T>
void testHashTableRandom(T& hash, size_t items, string& str)
{
    const int MAX_VAL = 1000; //Define the max value.
    size_t MAX_KEY = items * 10; //Define the max key to be 10 * the number of items to be generated.
    Record<int> * records = new Record<int>[items]; //The list of records inserted to the table.

    cout << "********************************************************************************" << endl
         << "                    R A N D O M   H A S H   T E S T:                            " << endl
         << "********************************************************************************" << endl;
    cout << str << endl;

    size_t index = 0;
    while(hash.size() < items)
    {
        //generate a random record, and attempt to insert it to the hash table.
        // duplicates are not permitted, do not store duplicate records in the array.
        Record<int> rec(((rand() % MAX_KEY) + 1), ((rand() % MAX_VAL) + 1));
        if(hash.insert(rec))
        {
            records[index] = rec;
            index++;
        }
    }
    cout << "---- [" << items << "] keys inserted." << endl
         << "- - - - - - - - - Search for existing keys ----------------" << endl
         << "Search for existing keys: " << items << " keys to be tested:" << endl;

    for(index = 0; index < items; index++)
    {
        if(!hash.is_present(records[index].key))
            cout << "Error: item with key = " << records[index].key << " could not be found." << endl;
    }

    cout << "EXISTING KEYS LOOKUP: VERIFIED. EXISTING KEYS EXAMINED: "<< items << endl
         << endl << " - - - - - - - - - Search for non-existent keys ----------------" << endl
         << "Search for nonexistent keys: " << items << " keys to be tested:" << endl;


    //search for invalid keys outside the random number space used for valid keys.
    for(index = MAX_KEY + 1; index < MAX_KEY + 1 + items; index++)
        if(hash.is_present(index))
            cout << "Error: item with key = " << index << " found when it should not exist." << endl;

    cout << "NONEXISTENT KEYS LOOKUP: VERIFIED. NONEXISTENT KEYS EXAMINED: " << items << endl
         << "------------------ END RANDOM TEST ----------------------" << endl;


    delete [] records;

}

//preconditions: none
//postconditions: a number between min and max is obtained from cin and returned.
int getNumberInRange(int min, int max)
{
    int myNum = 0;
    do
    {
        cout  << "Enter value in range: [" << min << " , " << max << "] : ";
        cin >> myNum;
        cin.clear();
        cin.ignore(INT_MAX,'\n');

    }while(myNum < min || myNum > max);

    return myNum;
}

//preconditions: none
//postconditions: a valid menu selection from cin is returned.
char getMenuSelection(string& prompt, string& validEntries)
{
    char selection = ' ';
    validEntries.find(selection,0);

    do
    {
        cout << prompt;
        cin >> selection;

        selection = tolower(selection);
        cin.clear();
        cin.ignore(INT_MAX,'\n');

    }while(validEntries.find(selection,0) == string::npos);

    return selection;
}
