#ifndef CHAINEDHASH_H
#define CHAINEDHASH_H

#include <cstdlib>
#include <cassert>
#include <record.h>
#include "avl.h"

using namespace std;

template <typename T>
class ChainedHash
{
    //note: this typename is different so that the definition and implementation can be seperated.
    template <typename TT>
    friend ostream& operator<<(ostream& outs, const ChainedHash<TT>& table);

public:
    ChainedHash();                                          // cstr: set _capacity to 17
    ChainedHash(size_t maxCapacity);                        // cstr : set _capacity to maxCapacity

    //big 3
    ~ChainedHash();
    ChainedHash<T>& operator=(const ChainedHash<T>& other);
    ChainedHash(const ChainedHash<T>& other);

    bool insert(const T& entry);                //returns true if the record inserted, otherwise false.
    bool remove(int key);                       //returns true if the record with the key was removed, otherwise false.
    bool is_present(int key);                   //returns true if the key exists, otherwise false.
    void find(int key, bool& found, T& result); //returns found = true, result = record with key if the key exists.

    //preconditions: none
    //postconditions: returns the current _size.
    inline size_t size() const
    {
        return _size;
    }

    //preconditions: none
    //postconditions: returns the _capacity
    inline size_t capacity() const
    {
        return _capacity;
    }

private:
    AVL<T> **_data; //dynamic array of avls.
    size_t _size;
    size_t _capacity;

    //helper function to be used by copy constructor and assignment operator.
    void copyArray(const AVL<T> ** copyFrom, AVL<T> **& copyTo, const size_t & copyFromSize);

    inline size_t hash(int key) const
    {
        return key % _capacity;
    }
};

//preconditions: none
//postconditions: the hash table will be printed to the recieved output stream.
template <typename TT>
ostream& operator<<(ostream& outs, const ChainedHash<TT>& table)
{
    for(size_t i = 0; i < table._capacity; i++)
    {
        outs << "[" << setfill('0') << setw(3) << i << "] " << setfill(' ') << endl;

        //print the AVL if at least one item is stored there
        if(table._data[i]->size() > 0)
        {
            outs << *table._data[i]  << endl;
        }
        outs << endl << endl;
    }

    return outs;
}

//preconditions: none
//postconditions: constructs a new ChainedHash object with the default _capacity (17)
// initialize _data to be an array of AVL pointers, then allocate each AVL.
template<typename T>
ChainedHash<T>::ChainedHash()
{
    _size = 0;
    _capacity = 17;
    _data = new AVL<T>*[_capacity]; //allocate an array of AVL pointers.

    for(size_t i = 0; i < _capacity; i++)
        _data[i] = new AVL<T>();
}

//preconditions: none
//postconditions: constructs a new ChainedHash object with the recieved capacity.
// initialize _data to be an array of AVL pointers, then allocate each AVL.
template<typename T>
ChainedHash<T>::ChainedHash(size_t maxCapacity)
{
    _size = 0;
    _capacity = maxCapacity;
    _data = new AVL<T>*[_capacity]; //allocate an array of AVL pointers.

    for(size_t i = 0; i < _capacity; i++)
        _data[i] = new AVL<T>();
}

//preconditions: none
//postconditions: deallocate dynamic memory.
template<typename T>
ChainedHash<T>::~ChainedHash()
{
    for(size_t i = 0; i < _capacity; i++)
        delete _data[i];
}

//preconditions: none
//postconditions: deallocate this ChainedHash object
// and reassign it the contents of other.
template<typename T>
ChainedHash<T>& ChainedHash<T>::operator=(const ChainedHash<T>& other)
{
    _capacity() = other._capacity;
    _size = other._size;

    for(size_t i = 0; i < _capacity; i++)
        delete _data[i];

    _data = new AVL<T>*[_capacity];

    copyArray(other._data,_data,_capacity);
}

//preconditions: none
//postconditions: construct this ChainedHash with the contents of other.
template<typename T>
ChainedHash<T>::ChainedHash(const ChainedHash<T>& other)
{
    _capacity = other.capacity;
    _size = other._size;
    _data = new AVL<T>*[_capacity];

    copyArray(other._data,_data,_capacity);
}

//preconditions: copyFrom and copyTo must be initialized.
//postconditions: range: [0, copyFromSize) in copyFrom is coppied to copyTo,
// copyTo is returned by ref.
template<typename T>
void ChainedHash<T>::copyArray(const AVL<T> ** copyFrom, AVL<T> **& copyTo, const size_t & copyFromSize)
{
    for(size_t i = 0; i < copyFromSize; i++)
        copyTo[i] = copyFrom[i];
}

//preconditions: none
//postconditions: the entry will be inserted into the table at the hash of its key,
// if that position is already taken, the second hash function will be applied
// to the key until an index is found that is available. If the entry was inserted return true,
// otherwise if an entry with the same key already exists in the table, or the table is full, return false.
template<typename T>
bool ChainedHash<T>::insert(const T &entry)
{
    size_t index = hash(entry.key);
    bool inserted = _data[index]->insert(entry);

    if(inserted)
        _size++;

    return inserted;
}

//preconditions: key must be a non-negative integer.
//postconditions: first, obtain the index of the item to be removed, if it exists in the hashtable.
// returns true if the item with the key found and removed, otherwise false.
template<typename T>
bool ChainedHash<T>::remove(int key)
{
    assert(key >= 0);
    size_t index = hash(key);
    bool removed = _data[index]->erase(T(key));

    if(removed)
        _size--;

    return removed;
}

//preconditions: none
//postconditions: if the record with the recieved key exists in the table,
// returns true, otherwise returns false.
template<typename T>
bool ChainedHash<T>::is_present(int key)
{
    assert(key >= 0);

    size_t index = hash(key);
    tree_node<T>* found_ptr = nullptr;
    return _data[index]->search(T(key),found_ptr);
}

//preconditions: key must be a non-negative interger.
//postconditions: if the record with the recieved key exists in the table,
// found will be true and the record will be returned by ref.
template<typename T>
void ChainedHash<T>::find(int key, bool& found, T& result)
{
    assert(key >= 0);

    size_t index = hash(key);
    tree_node<T>* found_ptr = nullptr;
    T tempRecord(key);
    found = _data[index]->search(tempRecord,found_ptr);

    if(found)
    {
        result = found_ptr->_item;
    }
}

#endif // CHAINEDHASH_H
