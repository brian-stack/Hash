#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <cstdlib>

using namespace std;

template <typename T>
struct Record
{
    T data;
    int key;

    friend ostream& operator<<(ostream& outs, const Record& rec)
    {
        outs << "(k:d) " << setfill('0') << setw(5) << rec.key << ":"
             << setw(4) << rec.data;

        outs.fill(' ');
        return outs;
    }

    friend bool operator<(const Record &LHS, const Record &RHS)
    {
        return (LHS.key < RHS.key);
    }

    friend bool operator>(const Record &LHS, const Record &RHS)
    {
        return (LHS.key > RHS.key);
    }

    friend bool operator==(const Record &LHS, const Record &RHS)
    {
        return (LHS.key == RHS.key);
    }

    friend bool operator!=(const Record &LHS, const Record &RHS)
    {
        return (LHS.key != RHS.key);
    }

    Record(int k = 0, T d = T()) : key(k), data(d) {}
};

#endif // RECORD_H
