/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VPX_PORTS_MEM_H_
#define VPX_PORTS_MEM_H_

#include <string.h>
#include "vpx_config.h"
#include "vpx/vpx_integer.h"

#if (defined(__GNUC__) && __GNUC__) || defined(__SUNPRO_C)
#define DECLARE_ALIGNED(n, typ, val) typ val __attribute__((aligned(n)))
#elif defined(_MSC_VER)
#define DECLARE_ALIGNED(n, typ, val) __declspec(align(n)) typ val
#else
#warning No alignment directives known for this compiler.
#define DECLARE_ALIGNED(n, typ, val) typ val
#endif

/* Use for unaligned load/stores that get flagged by the sanitizer.
 * All modern compilers compile this into simple moves.
 */
static inline void vpx_store_unaligned_uint32(void* dst, uint32_t v) {
    memcpy(dst, &v, sizeof(v));
}

static inline uint32_t vpx_load_unaligned_uint32(const void* src) {
    uint32_t v;
    memcpy(&v, src, sizeof(v));
    return v;
}

/* Indicates that the usage of the specified variable has been audited to assure
 * that it's safe to use uninitialized. Silences 'may be used uninitialized'
 * warnings on gcc.
 */
#if defined(__GNUC__) && __GNUC__
#define UNINITIALIZED_IS_SAFE(x) x = x
#else
#define UNINITIALIZED_IS_SAFE(x) x
#endif

#if HAVE_NEON && defined(_MSC_VER)
#define __builtin_prefetch(x)
#endif

/* Shift down with rounding */
#define ROUND_POWER_OF_TWO(value, n) (((value) + (1 << ((n)-1))) >> (n))
#define ROUND64_POWER_OF_TWO(value, n) (((value) + (1ULL << ((n)-1))) >> (n))

#define ALIGN_POWER_OF_TWO(value, n) \
  (((value) + ((1 << (n)) - 1)) & ~((1 << (n)) - 1))

#define CONVERT_TO_SHORTPTR(x) ((uint16_t *)(((uintptr_t)(x)) << 1))
#if CONFIG_VP9_HIGHBITDEPTH
#define CONVERT_TO_BYTEPTR(x) ((uint8_t *)(((uintptr_t)(x)) >> 1))
#endif  // CONFIG_VP9_HIGHBITDEPTH

#if !defined(__has_feature)
#define __has_feature(x) 0
#endif  // !defined(__has_feature)

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#define VPX_WITH_ASAN 1
#else
#define VPX_WITH_ASAN 0
#endif  // __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)

#endif  // VPX_PORTS_MEM_H_
