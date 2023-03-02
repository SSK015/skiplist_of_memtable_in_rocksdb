#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "tcInst.h"
#include "translationCache.h"

using namespace std;

size_t nhit = 0;
/*
TranslationCacheTable tct;

string& fastGet(string& key){
    uint64_t x = tct.findTranslation(key);
    if(x != 0){
        nhit++;
        return ((pair<string, string>*)x)->second;
    }
    auto itr = hashTable.find(key);
    tct.insertTranslation(key, (uint64_t)&(*itr));
    return itr->second;
}


string& cacheGet(string& key){
    uint64_t rt = queryCache(key);
    //cout << rt << endl;
    if(!rt){
        auto itr = hashTable.find(key);
        rt = (uint64_t)&(*itr);
        insertCache(key, rt);
    }
    return ((pair<string, string>*)rt)->second;
}
*/
#ifdef dense_hash_code

template<typename T>
string& slowGet(string& key, T& hashTable){
    return hashTable[key]->second;
}
#else
template<typename T>
string& slowGet(string& key, T& hashTable){
    return hashTable[key];
}
#endif


const string target = "3Y#4Ni02v'\\+9^7-!f0N}'.n=F%0Kg<$*=5z-Jq7O%>Ws'9v?1,1:*6)l#Su?G=8";
const size_t sim_test=1<<17;

unordered_map<uint64_t, pair<uint64_t, uint64_t>> kcnt;


#ifdef dense_hash_code
template<typename T>
string& systemHashTableGet(string& key, T& hashTable){
    uint64_t hval;
    pair<string, string>* rt = (pair<string, string>*)querySystemHashTable(key, hval);
    //if(rt)
    //    cout << key << " " << rt->first << endl;
    if(!rt || rt->first != key){
        rt = hashTable[key];
        insertSystemHashTable(key, (uint64_t)rt, hval);
    }
    return rt->second;
}
#else
template<typename T>
string& systemHashTableGet(string& key, T& hashTable){
    uint64_t hval;
    uint64_t rt = querySystemHashTable(key, hval);

    if(!rt || ((pair<string, string>*)rt)->first != key){
        auto itr = hashTable.find(key);
        rt = (uint64_t)&(*itr);
        insertSystemHashTable(key, rt, hval);
    }
    return ((pair<string, string>*)rt)->second;
}
#endif



/*
template<typename T>
string& systemHashTableGet(string& key, T& hashTable){
    uint64_t hval;
    uint64_t rt = querySystemHashTableWithKey(key, hval);
    if(rt) return ((pair<string, string>*)rt)->second;
    else{
        auto itr = hashTable.find(key);
        rt = (uint64_t)&(*itr);
        insertSystemHashTableWithKey(key, rt, hval);
        return ((pair<string, string>*)rt)->second;
    }
}
*/

template<typename T>
size_t runWithoutCache(vector<string>& readSequence, T& hashTable, size_t ntest=-1){
    size_t s = 0;
    size_t cnt = 0;
    for(auto &i : readSequence){
        cnt++;
        string& str = slowGet(i, hashTable);
        for(size_t j = 0; j < VALUE_SIZE/64; j+=64){
            s += str[j];
        }
        if(cnt == ntest) break;
    }
    return s;
}

/*
size_t runWithCache(vector<string>& readSequence){
    size_t s = 0;
    for(auto &i : readSequence){
        s += fastGet(i).length();
    }
    return s;
}

size_t runWithTC(vector<string>& readSequence){
    size_t s = 0;
    for(auto &i :readSequence){
        s += cacheGet(i).length();
    }
    return s;
}
*/
template<typename T>
size_t runWithSystemHashTable(vector<string>& readSequence, T& hashTable, size_t ntest=-1){
    size_t s = 0;
    size_t cnt = 0;
    for(auto &i : readSequence){
        cnt++;
        string& str = systemHashTableGet(i, hashTable);
        for(size_t j = 0; j < VALUE_SIZE/64; j+=64){
            s += str[j];
        }
        if(cnt == ntest) break;
    }
    return s;
}

void resetCounter(){
    nSHTQuery =  nSHTInsert = nSHTMiss = 0;
}

template<typename T>
void loadData(string file, T& hashTable){
    ifstream fin(file);
    string op, key, field;
    while(fin >> op >> key >> field){
        char value[VALUE_SIZE+128];
        fin.read(value, 1);
        fin.getline(value, VALUE_SIZE+128, '\n');
#ifdef dense_hash_code
        hashTable[key] = new pair<string, string>(key, string(value));
#else
        hashTable[key] = value;
#endif
    }
    fin.close();
}

vector<string> readSequence;
template<typename T>
void loadOperation(string file, T& hashTable){
    ifstream fin(file);
    string op, key;
    while(fin >> op >> key)
        readSequence.push_back(key);
    fin.close();
}
