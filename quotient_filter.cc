#include "quotient_filter.h"
using namespace std;

#define OCCUPIED_FLAG (1 << 31)
#define CONTINUATION_FLAG (1 << 30)
#define SHIFTED_FLAG (1 << 29)

QuotientFilter::QuotientFilter(unsigned int q_t, unsigned int r_t){
    q = q_t;
    r = r_t;
    q_mask = (((1 << (q+r)) - 1) >> r) << r;
    r_mask = ((1 << r) - 1);
    size_t table_size = 1 << q;
    keys = std::vector<unsigned int> (table_size);
}

QuotientFilter::~QuotientFilter(){
    return;
}

bool QuotientFilter::lookup(int key) const {
    unsigned int hash_code = hash_func(key);
    unsigned int quotient  = (hash_code & q_mask) >> r;
    unsigned int remaining = (hash_code & r_mask);

    /* Scan backwards until reach the beginning of the cluster, marked by 
        isShifted being false. */
    unsigned int cur_pos = quotient;
    key = keys[cur_pos];
    if (!isOccupied(key)){
        return false;
    } 

    while (isShifted(key)){
        cur_pos--;
        if (cur_pos == 0) cur_pos = keys.size() - 1;
        key = keys[cur_pos];
    }

    /* Scan forward until arriving at the run for the current quotient, which
        we can do by keeping track of how many isOccupied slots we find before
        reaching the quotient position, and decrementing the count whenever a 
        new run begins (isContinuation is false). */
    if (cur_pos != quotient) {
        int runs_seen = 1;
        bool beforeQuotient = true;
        while (runs_seen > 0) {
            cur_pos++;
            if (cur_pos == keys.size()) cur_pos = 0;

            key = keys[cur_pos];
            if (cur_pos == quotient) beforeQuotient = false;
            if (isOccupied(key) && beforeQuotient) runs_seen++;
            if (!isContinuation(key)) runs_seen--;
        }
    }

    /* Should now be at the beginning of the correct run */
    if ((key & r_mask) == remaining){
        return true;
    } else if (((key & r_mask) < remaining)){
        cur_pos++;
        if (cur_pos == keys.size()) cur_pos = 0;
        key = keys[cur_pos];
        while (isContinuation(key)){
            if ((key & r_mask) == remaining){
                return true;
            } else if (((key & r_mask) > remaining)){
                return false;
            }
            cur_pos++;
            if (cur_pos == keys.size()) cur_pos = 0;
            key = keys[cur_pos];
        }
    }

    return false;
}


void QuotientFilter::insert(int key){
    unsigned int hash_code = hash_func(key);
    unsigned int quotient  = (hash_code & q_mask) >> r;
    unsigned int remaining = (hash_code & r_mask);

    // cout << "Quotient " << quotient << " remainder " << remaining << endl;

    /* Scan backwards until reach the beginning of the cluster, marked by 
        isShifted being false. */
    unsigned int cur_pos = quotient;
    key = keys[cur_pos];
    if (!isOccupied(key) && !isShifted(key)){
        keys[cur_pos] = remaining | OCCUPIED_FLAG;
        // cout << "Inserted " << endl;
        return;
    } else if (!isOccupied(key)) {
        /* Still mark this as occupied */
        keys[cur_pos] = keys[cur_pos] | OCCUPIED_FLAG;
    }

    while (isShifted(key)){
        cur_pos--;
        if (cur_pos == 0) cur_pos = keys.size() - 1;
        key = keys[cur_pos];
    }

    /* Scan forward until arriving at the run for the current quotient, which
        we can do by keeping track of how many isOccupied slots we find before
        reaching the quotient position, and decrementing the count whenever a 
        new run begins (isContinuation is false). */
    if (cur_pos != quotient) {
        int runs_seen = 1;
        bool beforeQuotient = true;
        while (runs_seen > 0) {
            cur_pos++;
            if (cur_pos == keys.size()) cur_pos = 0;

            key = keys[cur_pos];
            if (cur_pos == quotient) beforeQuotient = false;
            if (isOccupied(key) && beforeQuotient) runs_seen++;
            if (!isContinuation(key)) runs_seen--;
        }
    }

    // cout << "Starting search from " << cur_pos << endl;
    /* Should now be at the beginning of the correct run */
    bool beginningRun = true;
    if ((key & r_mask) == remaining){
        // cout << "Inserted " << endl;
        return;
    } else if (((key & r_mask) < remaining)){
        beginningRun = false;
        cur_pos++;
        if (cur_pos == keys.size()) cur_pos = 0;
        key = keys[cur_pos];
        while (isContinuation(key)){
            if ((key & r_mask) == remaining){
                // cout << "Inserted " << endl;
                return;
            } else if (((key & r_mask) > remaining)){
                break;
            }
            cur_pos++;
            if (cur_pos == keys.size()) cur_pos = 0;
            key = keys[cur_pos];
        }
    }
    
    /* Insert the remainder at the current position and shift other elements
        if necessary. */
    // cout << "Inserted at " << cur_pos << endl;
    unsigned int temp = keys[cur_pos];
    if (cur_pos == quotient){
        keys[cur_pos] = remaining | OCCUPIED_FLAG;
    } else if (beginningRun) {
        keys[cur_pos] = remaining | SHIFTED_FLAG | (temp & OCCUPIED_FLAG);
    } else {
        keys[cur_pos] = remaining | SHIFTED_FLAG | CONTINUATION_FLAG | (temp & OCCUPIED_FLAG);
    }
    while (isOccupied(temp) || isShifted(temp)){
        unsigned int prev_pos = cur_pos;
        cur_pos++;
        if (cur_pos == keys.size()) cur_pos = 0;
        
        unsigned int nextKey = keys[cur_pos];
        if (!isContinuation(nextKey) && prev_pos != quotient) {
            temp = (r_mask & temp) | SHIFTED_FLAG | (nextKey & OCCUPIED_FLAG);     
        } else {
            temp = (r_mask & temp) | SHIFTED_FLAG | CONTINUATION_FLAG | (nextKey & OCCUPIED_FLAG);           
        }
        keys[cur_pos] = temp;
        temp = nextKey;
    }
    // cout << "Inserted " << endl;

}

bool QuotientFilter::isOccupied(unsigned int key) const {
    return (key & OCCUPIED_FLAG) > 0;
}

bool QuotientFilter::isContinuation(unsigned int key) const {
    return (key & CONTINUATION_FLAG) > 0;
}

bool QuotientFilter::isShifted(unsigned int key) const {
    return (key & SHIFTED_FLAG) > 0;
}
