#ifndef QuotientFilter_Included
#define QuotientFilter_Included

#include <stddef.h>
#include <iostream>
#include <vector>
#include <bitset>

class QuotientFilter {
public:
    QuotientFilter(unsigned int q_t, unsigned int r_t);

    ~QuotientFilter();

    bool lookup(int key) const;
    void insert(int key);
    int lookup_and_count(int key);
private:
    unsigned int q;
    unsigned int r;
    unsigned int q_mask;
    unsigned int r_mask;
    
    std::vector<unsigned int> keys;
    std::hash<int> hash_func;

    bool isOccupied(unsigned int key) const;
    bool isContinuation(unsigned int key) const;
    bool isShifted(unsigned int key) const;
    unsigned int hash1(unsigned int x) const;
};

#endif