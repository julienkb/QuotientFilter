#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <set>

#include "quotient_filter.h"

int main(){
    srand (time(NULL));
    std::set<int> elems = std::set<int> ();
    QuotientFilter qf = QuotientFilter(16, 4);
    for (int i = 0; i < 50000; i++) {
        int next = rand();
        elems.insert(next);
        qf.insert(next);
        bool found = qf.lookup(next);
        if (!found) {
            std::cout << "Failed on lookup" <<std::endl;
        }
    }

    for (int i = 0; i < 50000; i++) {
        int next = rand();
        bool ground_truth = elems.count(next) > 0;
        bool found = qf.lookup(next);
        if (found != ground_truth) {
            if (ground_truth == true){
                std::cout << "Problems" << std::endl;
            }
        }
    }

    return 0;
}

