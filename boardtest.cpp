
#include "help/array2d.h"

bool test_fn(){
    assert(rep_int(123,5) == 5);
    assert(rep_int(123,0) == 0);
    assert(rep_int(12,24) == 0);
    assert(rep_int(12,23) == 11);
    assert(rep_int(5,-4) == 1);
    assert(rep_int(5,-5) == 0);
    assert(rep_int(5,-6) == 4);
    assert(rep_int(5,-12) == 3);
}

