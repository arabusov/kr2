#ifndef __TYPES_H__
#define __TYPES_H__
#include <stdint.h>

typedef char tchar;

typedef int16_t tint;
typedef uint16_t tuint;
typedef int32_t tlong;
typedef uint32_t tulong;

#include <inttypes.h>

#define TARGd PRId16
#define TARGu PRIX16
#define TARGld PRId32
#define TARGlu PRIX32

#endif /* __TYPES_H__ */
