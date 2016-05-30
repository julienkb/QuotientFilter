#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <set>

#include "quotient_filter.h"

int main(){
    for (int i = 0; i < 20; i++) {
        srand (time(NULL) + i);
        std::set<int> elems = std::set<int> ();
        int q = 20;
        QuotientFilter qf = QuotientFilter(q, 4);
        unsigned int capacity = 1 << q;
        unsigned int upper_limit = 0.9*capacity;
        for (unsigned int i = 0; i < upper_limit; i++) {
            int next = rand();
            // int next = e[i];
            // std::cout << next << ", ";
            elems.insert(next);
            qf.insert(next);
            bool found = qf.lookup(next);
            if (!found) {
                std::cout << "Failed on lookup" <<std::endl;
            }
        }
        // std::cout << std::endl;
        std::cout << "Inserted all" << std::endl;
        // for (int i = 0; i < 200000; i++) {

        //     int next = rand();
        //     bool ground_truth = elems.count(next) > 0;
        //     bool found = qf.lookup(next);
        //     if (found != ground_truth) {
        //         if (ground_truth == true){
        //             std::cout << "Problems " << next << std::endl;
        //         }
        //     }
        // }
        std::set<int>::iterator it;
        for (it = elems.begin(); it != elems.end(); ++it)
        {
            int f = *it; // Note the "*" here
            if (!qf.lookup(f))
                std::cout << "Problems " << f << std::endl;
        }
    }
    return 0;
}

