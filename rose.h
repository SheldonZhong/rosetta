/*
 * Copyright (c) 2020  Zhong, Wenshao  <wzhong20@uic.edu>
 *
 * All rights reserved. No warranty, explicit or implicit, provided.
 */
#pragma once
#include "lib.h"

struct rose;

  void*
rose_create(u64 num);

  void
rose_destroy(struct rose* rose);

  void
rose_insert(struct rose* rose, u64 k);

  bool
rose_range(struct rose* rose, u64 low, u64 high);

