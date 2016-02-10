#ifndef BRANDONTYPES_H
#define BRANDONTYPES_H

#include <stdint.h>

typedef struct BT_Fraction8
{
	uint8_t n,m;
} fraction8;
typedef struct BT_Fraction16
{
	uint16_t n,m;
} fraction16;
typedef struct BT_Fraction32
{
	uint32_t n,m;
} fraction32;
typedef struct BT_Fraction64
{
	uint64_t n,m;
} fraction64;


//Decimal fixed point representation//
//The 64 bit fixed points are going to be buggy as hell in multiplies and divides. As such, no functions defined for them yet. They're pretty muched
//used only to prevent overflows in multiplication and divide functions

//.1 resolution
typedef uint8_t		ufixed8_1;		// [00.0, 25.5]
typedef uint16_t	ufixed16_1;		// [0000.0, 6553.5]
typedef uint32_t	ufixed32_1;		// [000000000.0, 429496729.5]
typedef uint64_t	ufixed64_1;		// BIG

typedef int8_t		fixed8_1;			// [-12.8, 12.7]
typedef int16_t		fixed16_1;		// [-3276.8, 3276.7]
typedef int32_t		fixed32_1;		// [-214748364.8, 214748364.7]
typedef int64_t		fixed64_1;		// BIG/2

//.01 resolution
typedef uint8_t		ufixed8_2;		// [0.00, 2.55]
typedef uint16_t	ufixed16_2;		// [000.00, 655.35]
typedef uint32_t	ufixed32_2;		// [00000000.00, 42949672.95]
typedef uint64_t	ufixed64_2;		// BIG

typedef int8_t		fixed8_2;			// [-1.28, 1.27]
typedef int16_t		fixed16_2;		// [-327.68, 327.67]
typedef int32_t		fixed32_2;		// [-21474836.48, 21474836.47]
typedef int64_t		fixed64_2;		// BIG/2

//.001 resolution
typedef uint8_t		ufixed8_3;		// [.000, .255]
typedef uint16_t	ufixed16_3;		// [00.000, 65.535]
typedef uint32_t	ufixed32_3;		// [0000000.000, 4294967.295]
typedef uint64_t	ufixed64_3;		// BIG

typedef int8_t		fixed8_3;		// [-.128, .127]
typedef int16_t		fixed16_3;		// [-32.768, 32.767]
typedef int32_t		fixed32_3;		// [-2147483.648, 2147483.647]
typedef int64_t		fixed64_3;		// BIG/2

//.0001 resolution
typedef uint8_t		ufixed8_4;		// [.0000, .0255]
typedef uint16_t	ufixed16_4;		// [0.0000, 6.5535]
typedef uint32_t	ufixed32_4;		// [000000.0000, 429496.7295]
typedef uint64_t	ufixed64_4;		// BIG

typedef int8_t		fixed8_4;		// [-.0128, .0127]
typedef int16_t		fixed16_4;		// [-3.2768, 3.2767]
typedef int32_t		fixed32_4;		// [-214748.3648, 214748.3647]
typedef int64_t		fixed64_4;		// BIG/2

//
//Functions
//
fixed32_1 mul32_1(fixed32_1 x1,fixed32_1 x2);
fixed32_2 mul32_2(fixed32_2 x1,fixed32_2 x2);
fixed32_3 mul32_3(fixed32_3 x1,fixed32_3 x2);
fixed32_4 mul32_4(fixed32_4 x1,fixed32_4 x2);
fixed32_1 mul32_1_lhp(fixed32_1 x1,fixed32_1 x2);
fixed32_2 mul32_2_lhp(fixed32_2 x1,fixed32_2 x2);
fixed32_3 mul32_3_lhp(fixed32_3 x1,fixed32_3 x2);
fixed32_1 mul32_1_llp(fixed32_1 x1,fixed32_1 x2);
fixed32_2 mul32_2_llp(fixed32_2 x1,fixed32_2 x2);
fixed32_3 mul32_3_llp(fixed32_3 x1,fixed32_3 x2);
fixed32_4 mul32_4_llp(fixed32_4 x1,fixed32_4 x2);
fixed32_1 div32_1(fixed32_1 x1,fixed32_1 x2);
fixed32_2 div32_2(fixed32_2 x1,fixed32_2 x2);
fixed32_3 div32_3(fixed32_3 x1,fixed32_3 x2);
fixed32_4 div32_4(fixed32_4 x1,fixed32_4 x2);
fixed32_1 div32_1_lhp(fixed32_1 x1,fixed32_1 x2);
fixed32_2 div32_2_lhp(fixed32_2 x1,fixed32_2 x2);
fixed32_3 div32_3_lhp(fixed32_3 x1,fixed32_3 x2);
fixed32_4 div32_4_lhp(fixed32_4 x1,fixed32_4 x2);
fixed32_1 div32_1_llp(fixed32_1 x1,fixed32_1 x2);
fixed32_2 div32_2_llp(fixed32_2 x1,fixed32_2 x2);
fixed32_3 div32_3_llp(fixed32_3 x1,fixed32_3 x2);
fixed32_4 div32_4_llp(fixed32_4 x1,fixed32_4 x2);

ufixed32_1 umul32_1(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 umul32_2(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 umul32_3(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 umul32_4(ufixed32_4 x1,ufixed32_4 x2);
ufixed32_1 umul32_1_lhp(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 umul32_2_lhp(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 umul32_3_lhp(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 umul32_4_lhp(ufixed32_4 x1,ufixed32_4 x2);
ufixed32_1 umul32_1_llp(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 umul32_2_llp(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 umul32_3_llp(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 umul32_4_llp(ufixed32_4 x1,ufixed32_4 x2);
ufixed32_1 udiv32_1(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 udiv32_2(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 udiv32_3(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 udiv32_4(ufixed32_4 x1,ufixed32_4 x2);
ufixed32_1 udiv32_1_lhp(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 udiv32_2_lhp(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 udiv32_3_lhp(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 udiv32_4_lhp(ufixed32_4 x1,ufixed32_4 x2);
ufixed32_1 udiv32_1_llp(ufixed32_1 x1,ufixed32_1 x2);
ufixed32_2 udiv32_2_llp(ufixed32_2 x1,ufixed32_2 x2);
ufixed32_3 udiv32_3_llp(ufixed32_3 x1,ufixed32_3 x2);
ufixed32_4 udiv32_4_llp(ufixed32_4 x1,ufixed32_4 x2);


fixed64_1 mul64_1(fixed64_1 x1, fixed64_1 x2);
fixed64_2 mul64_2(fixed64_2 x1, fixed64_2 x2);
fixed64_3 mul64_3(fixed64_3 x1, fixed64_3 x2);
fixed64_4 mul64_4(fixed64_4 x1, fixed64_4 x2);
ufixed64_1 umul64_1(ufixed64_1 x1, ufixed64_1 x2);
ufixed64_2 umul64_2(ufixed64_2 x1, ufixed64_2 x2);
ufixed64_3 umul64_3(ufixed64_3 x1, ufixed64_3 x2);
ufixed64_4 umul64_4(ufixed64_4 x1, ufixed64_4 x2);
fixed64_1 div64_1(fixed64_1 numerator, fixed64_1 denominator);
fixed64_2 div64_2(fixed64_2 numerator, fixed64_2 denominator);
fixed64_3 div64_3(fixed64_3 numerator, fixed64_3 denominator);
fixed64_4 div64_4(fixed64_4 numerator, fixed64_4 denominator);
ufixed64_1 udiv64_1(ufixed64_1 numerator, ufixed64_1 denominator);
ufixed64_2 udiv64_2(ufixed64_2 numerator, ufixed64_2 denominator);
ufixed64_3 udiv64_3(ufixed64_3 numerator, ufixed64_3 denominator);
ufixed64_4 udiv64_4(ufixed64_4 numerator, ufixed64_4 denominator);

//Conversion macros
//
#define toFixed_1(num) (num*10)
#define toFixed_2(num) (num*100)
#define toFixed_3(num) (num*1000)
#define toFixed_4(num) (num*10000)
#define toFixed_1_f(num,frac) (num*10 + frac)
#define toFixed_2_f(num,frac) (num*100 + frac)
#define toFixed_3_f(num,frac) (num*1000 + frac)
#define toFixed_4_f(num,frac) (num*10000 + frac)
#define toDec_1(num) (num/10)
#define toDec_2(num) (num/100)
#define toDec_3(num) (num/1000)
#define toDec_4(num) (num/10000)

#endif
