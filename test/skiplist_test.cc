//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//
//  This file is a little different with original one in rocksdb As we use YCSB-generated data to test.
#define VALUE_SIZE 648
#include "port/port.h"
#include <gtest/gtest.h>
#include "InlineSkiplist.h"

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "memory/concurrent_arena.h"
// #include "rocksdb/env.h"
#include "util/testharness.h"
// #include "util/hash.h"
#include "util/random.h"

namespace ROCKSDB_NAMESPACE {
// read dataset op key value
void loadData(std::string file){
    std::ifstream fin(file);
    std::string op, key, field;
    while(fin >> op >> key >> field){
  	char value[VALUE_SIZE+128];
	fin.read(value, 1);
	fin.getline(value, VALUE_SIZE+128, '\n');
    }
    fin.close();
}

// read dataset op key
std::vector<std::string> readSequence;
void loadOperation(std::string file){
    std::ifstream fin(file);
    std::string op, key;
    while(fin >> op >> key)
        readSequence.push_back(key);
    fin.close();
}

using Key = char*;
using Value = std::string;
using Key_new = std::string;

char combine_kv[1001];
//std::string combine_kv;
//struct combine_kv {
//   Key_new key;
//   Value value;
//};
static const char* Encode(const char* key) {
  return reinterpret_cast<const char*>(key);
}
char rv[25];
static char* Decode(const char* key) {
  memcpy(&rv, key, 24);
  rv[24] = '\0';
  return rv;
}

struct TestComparator {
  using DecodedType = char*;

  static DecodedType decode_key(const char* b) { 

      return Decode(b); }

  int operator()(const char* a, const char* b) const {
//    if (Decode(a) < Decode(b)) {
//      return -1;
//    } else if (Decode(a) > Decode(b)) {
//      return +1;
//    } else {
//      return 0;
//    }
      auto state = strcmp(a, b);
      if (state < 0) {
	      return -1;
  } else if (state > 0) {
  	return 1;
  } else {
	  return 0;
  }
  }

  
/*
  int operator()(const char* a, const DecodedType b) const {
    if (Decode(a) < b) {
      return -1;
    } else if (Decode(a) > b) {
      return +1;
    } else {
      return 0;
    }
  }
  */
};

using TestInlineSkipList = InlineSkipList<TestComparator>;

class InlineSkipTest : public testing::Test {
 public:
  void Insert(TestInlineSkipList* list, Key key, Key value) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    list->Insert(buf, value);
    keys_.insert(key);
  }

  bool InsertWithHint(TestInlineSkipList* list, Key key, Key value, void** hint) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    bool res = list->InsertWithHint(buf, value, hint);
    keys_.insert(key);
    return res;
  }

/*  void Validate(TestInlineSkipList* list) {
//    // Check keys exist.
    for (Key key : keys_) {
      ASSERT_TRUE(list->Contains(Encode(&key)));
    }
    // Iterate over the list, make sure keys appears in order and no extra
    // keys exist.
    TestInlineSkipList::Iterator iter(list);
    ASSERT_FALSE(iter.Valid());
    Key zero = 0;
    iter.Seek(Encode(&zero));
    for (Key key : keys_) {
      ASSERT_TRUE(iter.Valid());
//      ASSERT_EQ(key, Decode(iter.key()));
      iter.Next();
    }
    ASSERT_FALSE(iter.Valid());
    // Validate the list is well-formed.
    list->TEST_Validate();
  }
*/
 private:
  std::set<Key> keys_;
};

TEST_F(InlineSkipTest, InsertAndLookup) {
  const int N = 2000;
  const int R = 5000;
  Random rnd(1000);
//  std::set<std::string> keys;
std::set<std::string, std::less<>> keys;
//  std::set<char *> keys_s;
  ConcurrentArena arena;
  TestComparator cmp;
  InlineSkipList<TestComparator> list(cmp, &arena);

//  std::ofstream file_out1("../data/insert_performance.txt", std::ios::app);
//
//    // 检查文件是否成功打开
//  if (!file_out1.is_open()) {
//        std::cout << "Error opening file!" << std::endl;
//  }
//
//    // 写入数据
//    int value = 0;
//    for (int i = 0; i < 10; ++i) {
//        value = i * 10;
//        file_out1 << value << std::endl;
//    }
//
//    // 关闭文件
//
//
//
//    std::ifstream fin(file);
//    std::string op, key, field;
//    while(fin >> op >> key >> field){
//        char value[VALUE_SIZE+128];
//        fin.read(value, 1);
//        fin.getline(value, VALUE_SIZE+128, '\n');
//
//    }
//
//
//  file_out1.close();
//
//  
  
  char buff[101] = "I am Joe Biden";
//  list.Insert(buff);
  
  
//  for (int i = 0; i < N; i++) {
//    Key key = rnd.Next() % R;
//    if (keys.insert(key).second) {
//      <F2>char* buf = list.AllocateKey(sizeof(Key));
//      memcpy(buf, &key, sizeof(Key));
//      list.Insert(buf);
//    }
//  }
//
//  for (int i = 0; i < N; i++) {
//	
//    if (keys_s.insert(key).second) {
//      char* buf = list.AllocateKey(sizeof(Key));
//      memcpy(buf, &key, sizeof(Key));
//      list.Insert(buf);
//    }
// }
//
//

   std::ifstream fin("../data/input100K.txt");
   std::string op, field;
   char key[101];
//   char value[500];
   int qq = 0;
   auto start_time = std::chrono::high_resolution_clock::now()      ;
   char val[10001][101];
	while(fin >> op >> key >> field){
	++qq;
   //   while(fin >> op >> key >> field){
        char value[501];
        fin.read(value, 1);
//	auto neww = new conbine_kv;
//    strcpy(combine_kv, key);
//	combine_kv = key;
	char combine_kv[25]; 
        fin.getline(value, 501, '\n');
	strcpy(combine_kv, key);
	combine_kv[24] = '\0';
//        strcpy(combine_kv + 24, value);	
//	strcpy(combine_kv + 501, key);
//        combine_kv + 25 = value;
//        std::cout << key << " " << value << std::endl;
//        std::cout << sizeof(combine_kv) << std::endl;i
// 	std::cout << combine_kv << std::endl;
        if (keys.insert(combine_kv).second) {
		char* buf = list.AllocateKey(sizeof(combine_kv));
    		memcpy(buf, combine_kv, sizeof(combine_kv));  // 将复制后的内容写入 buf
    		buf[24] = '\0';  // 添加字符串结束符
		
 //  		std::cout << std::string(buf) << std::endl;
    		list.Insert(buf, value);  // 将 buf 的地址传递给 Insert() 函数
  //      char* buf = list.AllocateKey(sizeof(combine_kv));
//		std::cout << sizeof(combine_kv) << std::endl;

//	        memcpy(buff, combine_kv, sizeof(combine_kv));
//            std::cout << buf << std::endl;
//		buff[24] = '\0';
//		std::cout << buf << std::endl;	
//	        list.Insert(buf);
    		}
}
    fin.close();
  auto end_time = std::chrono::high_resolution_clock::now(); 
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Insert(micro seconds) : " << duration.count() / (double)qq << std::endl;

  int count_data = 0;

  std::ifstream fin0("../data/query1M_100KKey.txt");
//  InlineSkipList<TestComparator>::Iterator iter0(&list);
	list.TEST_Validate();
  while (fin0 >> op) {
        char key0[VALUE_SIZE + 128];
        fin0.read(key0, 1);	
        fin0.getline(key0, VALUE_SIZE + 128, '\n');
	count_data++;

        if(!list.Contains(key0)) std::cout << "error"<<std::endl;
  }
// here compute total_time / count_data = throughoutput
  fin0.close();
  std::cout << count_data <<std::endl;

//  std::cout << "aaaaa" << std::endl;
  std::ifstream fin1("../data/query1M_100KKey.txt");
//  InlineSkipList<TestComparator>::Iterator iter(&list);
//  start_time = std::chrono::high_resolution_clock::now(); 
  int cnnt = 0;
  char key00[20];
  while (fin1 >> op) {
// initialize a timer here to count every query opreation time
	char key1[VALUE_SIZE + 128];
//	std::cout << "sss" << std::endl;
	fin1.read(key00, 1);
	fin1.getline(key1, VALUE_SIZE + 128, '\n');
//	std::cout << key1 << std::endl;
    start_time = std::chrono::high_resolution_clock::now()      ;
 	InlineSkipList<TestComparator>::Iterator iter(&list);
  	
	iter.Seek(key1);
//	iter.Next();
//        iter.SeekToLast();
//
//        iter.RandomSeek();
	// timer end, and flush this time to an array;
	ASSERT_TRUE(iter.Valid());
//    std::cout << iter.value() << std::endl;
  	end_time = std::chrono::high_resolution_clock::now();
  	duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cnnt++;
    std::cout << "Query(micro seconds) : " << duration.count() << std::endl;
 }
  fin1.close(); 
// here open a new file under ../data, to store measured time array
// use plt to draw a picture  
// don not forget to close the file
  std::cout << cnnt << std::endl;

//  for (Key i = 0; i < R; i++) {
//    if (list.Contains(Encode(&i))) {
//      ASSERT_EQ(keys.count(i), 1U);
//    } else {
//      ASSERT_EQ(keys.count(i), 0U);
//    }
//  }
//
//
//  // Simple iterator tests
//  {
//    InlineSkipList<TestComparator>::Iterator iter(&list);
//    ASSERT_TRUE(!iter.Valid());
//
//    uint64_t zero = 0;
//    iter.Seek(Encode(&zero));
//    ASSERT_TRUE(iter.Valid());
//    ASSERT_EQ(*(keys.begin()), Decode(iter.key()));
//
//    uint64_t max_key = R - 1;
//    iter.SeekForPrev(Encode(&max_key));
//    ASSERT_TRUE(iter.Valid());
//    ASSERT_EQ(*(keys.rbegin()), Decode(iter.key()));
//
//    iter.SeekToFirst();
//    ASSERT_TRUE(iter.Valid());
//    ASSERT_EQ(*(keys.begin()), Decode(iter.key()));
//
//    iter.SeekToLast();
//    ASSERT_TRUE(iter.Valid());
//    ASSERT_EQ(*(keys.rbegin()), Decode(iter.key()));
//  }
//
//  // Forward iteration test
//  for (Key i = 0; i < R; i++) {
//    InlineSkipList<TestComparator>::Iterator iter(&list);
//    iter.Seek(Encode(&i));
//
//    // Compare against model iterator
//    std::set<Key>::iterator model_iter = keys.lower_bound(i);
//    for (int j = 0; j < 3; j++) {
//      if (model_iter == keys.end()) {
//        ASSERT_TRUE(!iter.Valid());
//        break;
//      } else {
//        ASSERT_TRUE(iter.Valid());
//        ASSERT_EQ(*model_iter, Decode(iter.key()));
//        ++model_iter;
//        iter.Next();
//      }
//    }
//  }
//
//  // Backward iteration test
//  for (Key i = 0; i < R; i++) {
//    InlineSkipList<TestComparator>::Iterator iter(&list);
//    iter.SeekForPrev(Encode(&i));
//
//    // Compare against model iterator
//    std::set<Key>::iterator model_iter = keys.upper_bound(i);
//    for (int j = 0; j < 3; j++) {
//      if (model_iter == keys.begin()) {
//        ASSERT_TRUE(!iter.Valid());
//        break;
//      } else {
//        ASSERT_TRUE(iter.Valid());
//        ASSERT_EQ(*--model_iter, Decode(iter.key()));
//        iter.Prev();
//      }
//    }
//  }
//}
//
}
}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  // report if errors happen
  ROCKSDB_NAMESPACE::port::InstallStackTraceHandler();
  // run gtest
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

