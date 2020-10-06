/*
 * Copyright (c) 2016--2020  Wu, Xingbo <wuxb45@gmail.com>
 *
 * All rights reserved. No warranty, explicit or implicit, provided.
 */
#pragma once
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#if defined(__x86_64__)
#include <x86intrin.h>
#elif defined(__aarch64__)
#include <arm_acle.h>
#include <arm_neon.h>
#endif

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef unsigned long   u64;

struct bf;

  extern struct bf *
bf_create(const u64 bpk, const u64 capacity);

  extern void
bf_add(struct bf * const bf, u64 hash64);

  extern bool
bf_test(const struct bf * const bf, u64 hash64);

  extern void
bf_clean(struct bf * const bf);

  extern void
bf_destroy(struct bf * const bf);

  extern u32
crc32c_u64(const u32 crc, const u64 v);

