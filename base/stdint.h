#ifndef __STDINT_H__
#define __STDINT_H__

#include <tamtypes.h>

#ifdef _EE
typedef u128 uint128_t;
typedef s128 int128_t;
#endif // _EE

typedef s32  ssize_t;

typedef u32  uintptr_t;
typedef s32  intptr_t;

typedef u64  uint64_t;
typedef u32  uint32_t;
typedef u16  uint16_t;
typedef u8   uint8_t;

typedef s64  int64_t;
typedef s32  int32_t;
typedef s16  int16_t;
typedef s8   int8_t;

#endif // __STDINT_H__

