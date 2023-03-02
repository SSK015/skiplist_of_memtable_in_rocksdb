#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <map>
#include <cstring>
#include "../include/tcInst.h"
#include "translationCache.h"
#include "benchAux.h"

using namespace std;

map<string, string> hashTable;


int main(int argc, char** argv){
    if(argc != 3){
        cerr << "command dataFile opeartionFile" << endl;
        return 1;
    }

    loadData(argv[1], hashTable);
    loadOperation(argv[2], hashTable);    

    size_t s = 0;
    
    if(!useSystemHashTable){
        s  += runWithoutCache(readSequence, hashTable);
        SimRoiStart();
        s += runWithoutCache(readSequence, hashTable, sim_test);
        SimRoiEnd();
    }
    
    if(useSystemHashTable){
        s += runWithSystemHashTable(readSequence, hashTable);
        resetCounter();
        SimRoiStart();
        s += runWithSystemHashTable(readSequence, hashTable, sim_test);
        SimRoiEnd();
    }

    cout << s << endl;

    return 0;
}
