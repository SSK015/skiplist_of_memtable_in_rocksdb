//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//
//  This file is a little different with original one in rocksdb As we use YCSB-generated data to test.
#include "memtable/skiplist.h"

#include <set>

#include "memory/arena.h"
// #include "rocksdb/env.h"
// #include "test_util/testharness.h"
// #include "util/hash.h"
#include "util/random.h"

namespace ROCKSDB_NAMESPACE {

using Key = uint64_t;

struct TestComparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

class SkipTest : public testing::Test {};

TEST(SkipTest, InsertAndLookup) {
  const int N = 2000;
  const int R = 5000;
  Random rnd(1000);
  
  std::set<Key> keys;
  
  Arena arena;
  TestComparator cmp;

  SkipList<Key, TestComparator> list(cmp, &arena);
  
  for (int i = 0; i < N; i++) {
    // This sentence should be replaced
      Key key = rnd.Next() % R;

    if (keys.insert(key).second) {
      list.Insert(key);
    }
  }

  for (int i = 0; i < R; i++) {
    if (list.Contains(i)) {
      ASSERT_EQ(keys.count(i), 1U);
    } else {
      ASSERT_EQ(keys.count(i), 0U);
    }
  }

  // Simple iterator tests
  {
    SkipList<Key, TestComparator>::Iterator iter(&list);
    ASSERT_TRUE(!iter.Valid());

    iter.Seek(0);
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.begin()), iter.key());

    iter.SeekForPrev(R - 1);
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.rbegin()), iter.key());

    iter.SeekToFirst();
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.begin()), iter.key());

    iter.SeekToLast();
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(*(keys.rbegin()), iter.key());
  }

  // Forward iteration test
  for (int i = 0; i < R; i++) {
    SkipList<Key, TestComparator>::Iterator iter(&list);
    iter.Seek(i);

    // Compare against model iterator
    std::set<Key>::iterator model_iter = keys.lower_bound(i);
    for (int j = 0; j < 3; j++) {
      if (model_iter == keys.end()) {
        ASSERT_TRUE(!iter.Valid());
        break;
      } else {
        ASSERT_TRUE(iter.Valid());
        ASSERT_EQ(*model_iter, iter.key());
        ++model_iter;
        iter.Next();
      }
    }
  }

  // Backward iteration test
  for (int i = 0; i < R; i++) {
    SkipList<Key, TestComparator>::Iterator iter(&list);
    iter.SeekForPrev(i);

    // Compare against model iterator
    std::set<Key>::iterator model_iter = keys.upper_bound(i);
    for (int j = 0; j < 3; j++) {
      if (model_iter == keys.begin()) {
        ASSERT_TRUE(!iter.Valid());
        break;
      } else {
        ASSERT_TRUE(iter.Valid());
        ASSERT_EQ(*--model_iter, iter.key());
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

