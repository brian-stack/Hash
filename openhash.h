#ifndef OPENHASH_H
#define OPENHASH_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <record.h>

using namespace std;

template <typename T>
class OpenHash
{
    //note: this typename is different so that the definition and implementation can be seperated.
    template <typename TT>
    friend ostream& operator<<(ostream& outs, const OpenHash<TT>& table);

public:
    OpenHash();                                        //default constructor.
    OpenHash(size_t maxCapacity);                      //constructs this object with _CAPACITY = maxCapacity.

    //big 3
    ~OpenHash();
    OpenHash<T>& operator=(const OpenHash<T>& other);
    OpenHash(const OpenHash<T>& other);

    bool insert(const T& entry);                       //returns true if the record inserted, otherwise false.
    bool remove(int key);                              //returns true if the record with the key was removed, otherwise false.
    bool is_present(int key) const;                    //returns true if the key exists, otherwise false.
    void find(int key, bool& found, T& result) const;  //returns found = true, result = record with key if the key exists.

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
    static const int NEVER_USED = -1;
    static const int PREVIOUSLY_USED = -2;

    size_t _capacity;
    T *_data;
    size_t _size;

    void find_index(int key, bool &found, size_t &index) const;
    void copyArray(const T * copyFrom, T *& copyTo, const size_t & copyFromSize);

    //preconditions: none
    //postconditions: applies the first hash function to the key.
    inline size_t hash(int key) const
    {
        return (key % _capacity);
    }

    //preconditions: index must be in range.
    //postconditions: returns the next index for the given key and index
    inline size_t next_index(size_t index) const
    {
        assert(index < _capacity);
        return (index < _capacity-1) ? index+1 : 0;
    }

    //preconditions: index must be in range.
    //postconditions: returns true if _data[index] has never been used, otherwise false.
    inline bool never_used(size_t index) const
    {
        assert(index < _capacity);
        return (_data[index].key == NEVER_USED);
    }

    //preconditions: index must be in range.
    //postconditions: returns true if _data[index] is available, otherwise false.
    inline bool is_vacant(size_t index) const
    {
        assert(index < _capacity);
        return (_data[index].key == PREVIOUSLY_USED || _data[index].key == NEVER_USED);
    }

    //preconditions: index must be in range.
    //postconditions: returns true if _data[index] is flagged as previously used.
    inline bool previously_used(size_t index) const
    {
        assert(index < _capacity);
        return (_data[index].key == PREVIOUSLY_USED);
    }
};

//preconditions: none
//postconditions: the hash table will be printed to the recieved output stream.
template <typename TT>
ostream& operator<<(ostream& outs, const OpenHash<TT>& table)
{
    cout << "row # |" << "key |" << "data|" << endl;
    for(size_t i = 0; i < table._capacity; i++)
    {
        outs << "[" << setfill('0') << setw(3) << i << "] ";

        if(table.previously_used(i))
        {
            outs << "- - - - - -";
        }
        else if(!table.is_vacant(i))
        {
            size_t iHash = table.hash(table._data[i].key); //hash of the key stored at data[i]
            outs << setfill('0') << setw(5) << table._data[i].key << ":"
                 << setfill('0') << setw(4) << table._data[i].data
                 << "(" << setfill('0') << setw(3) << iHash << ") ";

            if(i != iHash)
                cout << "*";
        }
        cout << endl;
    }

    return outs;
}

//preconditions: none
//postconditions: constructs a new doublehash object with default capacity = 811
//  initialize keys in _data to NEVER_USED(-2) indicating that the index is available.
template<typename T>
OpenHash<T>::OpenHash()
{
    _size = 0;
    _capacity = 811;
    _data = new T[_capacity];

    for(size_t i = 0; i < _capacity; i++)
        _data[i].key = NEVER_USED;
}

//preconditions: none
//postconditions: constructs a new doublehash object with the recieved capacity
//  initialize keys in _data to NEVER_USED(-2) indicating that the index is available.
template<typename T>
OpenHash<T>::OpenHash(size_t maxCapacity)
{
    _size = 0;
    _capacity = maxCapacity;
    _data = new T[_capacity];

    for(size_t i = 0; i < _capacity; i++)
        _data[i].key = NEVER_USED;
}

//preconditions: none
//postconditions: deallocate dynamic memory.
template<typename T>
OpenHash<T>::~OpenHash()
{
    delete [] _data;
}

//preconditions: none
//postconditions: deallocate this DoubleHash object
// and reassign it the contents of other.
template<typename T>
OpenHash<T>& OpenHash<T>::operator=(const OpenHash<T>& other)
{
    _capacity() = other._capacity;
    _size = other._size;
    delete [] _data;
    _data = new T[_capacity];
    copyArray(other._data,_data,_capacity);
}

//preconditions: none
//postconditions: construct this DoubleHash with the contents of other.
template<typename T>
OpenHash<T>::OpenHash(const OpenHash& other)
{
    _capacity = other.capacity;
    _size = other._size;
    _data = new T[_capacity];
    copyArray(other._data,_data,_capacity);
}

//preconditions: copyFrom and copyTo must be initialized.
//postconditions: range: [0, copyFromSize) in copyFrom is coppied to copyTo,
// copyTo is returned by ref.
template<typename T>
void OpenHash<T>::copyArray(const T * copyFrom, T *& copyTo, const size_t & copyFromSize)
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
bool OpenHash<T>::insert(const T &entry)
{
    bool alreadyPresent = false;
    T tempItem;
    find(entry.key, alreadyPresent, tempItem); //ensure the entry is not already in the hashtable
    size_t index;

    if(!alreadyPresent && _size < _capacity)
    {
        index = hash(entry.key);
        while (!is_vacant(index))
            index = next_index(index);

        _data[index] = entry;
        _size++;
        return true;
    }
    else
    {
        return false;
    }
}

//preconditions: key must be a non-negative integer.
//postconditions: first, obtain the index of the item to be removed, if it exists in the hashtable.
// returns true if the item with the key found and removed, otherwise false.
template<typename T>
bool OpenHash<T>::remove(int key)
{
    assert(key >= 0);

    bool found;
    size_t index;

    find_index(key, found, index);

    if(found)
    {
        _data[index].key = PREVIOUSLY_USED;
        --_size;
        return true;
    }
    else
    {
        return false;
    }
}

//preconditions: key must be a non-negative integer.
//postconditions: the index of the item with the recieved key is located
// using next_index to apply the second hash if the item is not stored at its first hash.
// return true if the item was found along with the index, otherwise return false.
template<typename T>
void OpenHash<T>::find_index(int key, bool &found, size_t &index) const
{
    assert(key >= 0);
    size_t count = 0;
    index = hash(key);

    while(count < _capacity && !never_used(index) && _data[index].key != key)
    {
        ++count;
        index = next_index(index);
    }
    found = (_data[index].key == key);
}

//preconditions: none
//postconditions: if the record with the recieved key exists in the table,
// returns true, otherwise returns false.
template<typename T>
bool OpenHash<T>::is_present(int key) const
{
    bool found;
    size_t index;
    find_index(key,found,index);
    return found;
}

//preconditions: none
//postconditions: if the record with the recieved key exists in the table,
// found will be true and the record will be returned by ref.
template<typename T>
void OpenHash<T>::find(int key, bool& found, T& result) const
{
    size_t index;
    find_index(key,found,index);

    if(found)
        result = _data[index];
}

#endif // OPENHASH_H
