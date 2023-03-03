#include <iostream>
#include "raw_skiplist.h"
int main() {
    
    Timer();

    auto list = new SkipList(raw);
    for (uint64_t i = 1; i <= 10240; ++i) {
        list->insert();
    }

    Timer();

    for (uint64_t i = 1; i <= 10240; ++i) {
        list->query();
    }

    Timer();

}
