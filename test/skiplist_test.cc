//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//
//  This file is a little different with original one in rocksdb As we use YCSB-generated data to test.
#define VALUE_SIZE 128
#include "port/port.h"
#include <gtest/gtest.h>
#include "InlineSkiplist.h"

#include <set>

#include "memory/concurrent_arena.h"
// #include "rocksdb/env.h"
#include "util/testharness.h"
// #include "util/hash.h"
#include "util/random.h"

namespace ROCKSDB_NAMESPACE {
// read dataset op key value
// void loadData(string file){
//    ifstream fin(file);
//    string op, key, field;
//    while(fin >> op >> key >> field){
//        char value[VALUE_SIZE+128];
//        fin.read(value, 1);
//        fin.getline(value, VALUE_SIZE+128, '\n');
//    }
//    fin.close();
// }

// read dataset op key
// vector<string> readSequence;
// void loadOperation(string file){
//   ifstream fin(file);
//   string op, key;
//   while(fin >> op >> key)
//       readSequence.push_back(key);
//   fin.close();
// }

using Key = uint64_t;

static const char* Encode(const uint64_t* key) {
  return reinterpret_cast<const char*>(key);
}

static Key Decode(const char* key) {
  Key rv;
  memcpy(&rv, key, sizeof(Key));
  return rv;
}

struct TestComparator {
  using DecodedType = Key;

  static DecodedType decode_key(const char* b) { return Decode(b); }

  int operator()(const char* a, const char* b) const {
    if (Decode(a) < Decode(b)) {
      return -1;
    } else if (Decode(a) > Decode(b)) {
      return +1;
    } else {
      return 0;
    }
  }

  int operator()(const char* a, const DecodedType b) const {
    if (Decode(a) < b) {
      return -1;
    } else if (Decode(a) > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

using TestInlineSkipList = InlineSkipList<TestComparator>;

class InlineSkipTest : public testing::Test {
 public:
  void Insert(TestInlineSkipList* list, Key key) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    list->Insert(buf);
    keys_.insert(key);
  }

  bool InsertWithHint(TestInlineSkipList* list, Key key, void** hint) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    bool res = list->InsertWithHint(buf, hint);
    keys_.insert(key);
    return res;
  }

  void Validate(TestInlineSkipList* list) {
    // Check keys exist.
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
      ASSERT_EQ(key, Decode(iter.key()));
      iter.Next();
    }
    ASSERT_FALSE(iter.Valid());
    // Validate the list is well-formed.
    list->TEST_Validate();
  }

 private:
  std::set<Key> keys_;
};

TEST_F(InlineSkipTest, InsertAndLookup) {
  const int N = 2000;
  const int R = 5000;
  Random rnd(1000);
  std::set<Key> keys;
  ConcurrentArena arena;
  TestComparator cmp;
  InlineSkipList<TestComparator> list(cmp, &arena);
  for (int i = 0; i < N; i++) {
    Key key = rnd.Next() % R;
    if (keys.insert(key).second) {
      char* buf = list.AllocateKey(sizeof(Key));
      memcpy(buf, &key, sizeof(Key));
      list.Insert(buf);
    }
  }

  for (Key i = 0; i < R; i++) {
    if (list.Contains(Encode(&i))) {
      ASSERT_EQ(keys.count(i), 1U);
    } else {
      ASSERT_EQ(keys.count(i), 0U);
    }
  }

  // Simple iterator tests
  {
    InlineSkipList<TestComparator>::Iterator iter(&list);
    ASSERT_TRUE(!iter.Valid());

    uint64_t zero = 0;
    iter.Seek(Encode(&zero));
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.begin()), Decode(iter.key()));

    uint64_t max_key = R - 1;
    iter.SeekForPrev(Encode(&max_key));
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.rbegin()), Decode(iter.key()));

    iter.SeekToFirst();
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.begin()), Decode(iter.key()));

    iter.SeekToLast();
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.rbegin()), Decode(iter.key()));
  }

  // Forward iteration test
  for (Key i = 0; i < R; i++) {
    InlineSkipList<TestComparator>::Iterator iter(&list);
    iter.Seek(Encode(&i));

    // Compare against model iterator
    std::set<Key>::iterator model_iter = keys.lower_bound(i);
    for (int j = 0; j < 3; j++) {
      if (model_iter == keys.end()) {
        ASSERT_TRUE(!iter.Valid());
        break;
      } else {
        ASSERT_TRUE(iter.Valid());
        ASSERT_EQ(*model_iter, Decode(iter.key()));
        ++model_iter;
        iter.Next();
      }
    }
  }

  // Backward iteration test
  for (Key i = 0; i < R; i++) {
    InlineSkipList<TestComparator>::Iterator iter(&list);
    iter.SeekForPrev(Encode(&i));

    // Compare against model iterator
    std::set<Key>::iterator model_iter = keys.upper_bound(i);
    for (int j = 0; j < 3; j++) {
      if (model_iter == keys.begin()) {
        ASSERT_TRUE(!iter.Valid());
        break;
      } else {
        ASSERT_TRUE(iter.Valid());
        ASSERT_EQ(*--model_iter, Decode(iter.key()));
        iter.Prev();
      }
    }
  }
}
}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  // report if errors happen
  ROCKSDB_NAMESPACE::port::InstallStackTraceHandler();
  // run gtest
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

