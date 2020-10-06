/*
 * Copyright (c) 2020  Zhong, Wenshao  <wzhong20@uic.edu>
 *
 * All rights reserved. No warranty, explicit or implicit, provided.
 */
#include "lib.h"

#include "rose.h"

struct rose {
  struct bf** bfs;
  u64 lvls;
};

// argument: number of keys
// hardcoded 63 layers of BF assuming all keys are 63 bit
// one bit preserve for the range query logic
// hardcoded 10 bits per key
// TODO: lower levels could use fewer nums
  void*
rose_create(u64 num)
{
  u64 lvls = 63;
  struct bf** bfs = malloc(lvls * sizeof(struct bf*));
  assert(bfs);

  for (u64 i = 0; i < lvls; i++) {
    struct bf* bf = bf_create(10, num);
    assert(bf);
    bfs[i] = bf;
  }

  struct rose* rose = malloc(sizeof(*rose));
  assert(rose);
  rose->lvls = lvls;
  rose->bfs = bfs;
  return rose;
}

  void
rose_destroy(struct rose* rose)
{
  for (u64 i = 0; i < rose->lvls; i++) {
    bf_destroy(rose->bfs[i]);
  }
  free(rose->bfs);
  free(rose);
}

// 0 : 0......, 1......
// mask: 0x8000000
// 1 : 00......, 01......, 10......, 11......
// mask: 0xc000000
// 2 : .............
  void
rose_insert(struct rose* rose, u64 k)
{
  const u64 lvls = rose->lvls;
  for (u64 i = 0; i < lvls; i++) {
    struct bf* bf = rose->bfs[i];

    u64 mask = ~((1ul << (lvls - i - 1)) - 1);
    u64 ik = k & mask;
    u32 lo = crc32c_u64(0xDEADBEEF, ik);
    u64 ihash = (u64)lo << 32 | lo;

    bf_add(bf, ihash);
  }
}

  static bool
doubt(struct rose* rose, u64 p, u64 l)
{
  const u64 lvls = rose->lvls;
  struct bf* bf;
  /*
  if (l == lvls) {
    bf = rose->bfs[l-1];
  } else {
    bf = rose->bfs[l];
  }
  */
  if (l == 1) {
    return true;
  }
  if (l > (lvls + 1)) {
    return true;
  }
  bf = rose->bfs[l-2];

  u32 lo = crc32c_u64(0xDEADBEEF, p);
  u64 hash = (u64)lo << 32 | lo;

  if (!bf_test(bf, hash)) {
    return false;
  }

  if (l > lvls) {
    return true;
  }

  if (doubt(rose, p, l + 1)) {
    return true;
  }
  return doubt(rose, p + (1 << (lvls - l)), l + 1);
}

  static void
bin(u64 num, u64 n)
{
  u64 mask = 1lu << (n - 1);
  for (u64 i = 0; i < n; i++) {
    printf("%c", num & mask ? '1' : '0');
    mask >>= 1;
  }
  printf("\n");
}

  static bool
range_query(struct rose* rose, u64 low, u64 high, u64 p, u64 l)
{
  const u64 lvls = rose->lvls;
  // 2 ^ (L - l + 1)
  // u64 shift = (lvls - 1);
  const u64 pow = (1lu << (lvls - l + 1));

  if ((p > high) || ((p + (pow - 1)) < low)) {
    // p is not contained in the range
    return false;
  }

  if ((p >= low) && ((p + (pow - 1)) <= high)) {
    // p is contained in the range
    return doubt(rose, p, l);
  }

  if (range_query(rose, low, high, p, l + 1)) {
    return true;
  }

  return range_query(rose, low, high, p + (pow >> 1), l + 1);
}

  bool
rose_range(struct rose* rose, u64 low, u64 high)
{
  return range_query(rose, low, high, 0, 1);
}

