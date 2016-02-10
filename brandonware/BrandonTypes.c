#include <stdint.h>
#include "BrandonTypes.h"



//Multiplication and division functions due to scaling for fixed point

//General purpose faster, lower quantity multiplies.
//(x1 * x2 * (decimal places *10)) cannot exceed 2^(32-1)/(decimal points)
fixed32_1 mul32_1(fixed32_1 x1,fixed32_1 x2)
{
	fixed32_1 buffer = (x1 * x2) / 10;
	return buffer;
}
fixed32_2 mul32_2(fixed32_2 x1,fixed32_2 x2)
{
	fixed32_2 buffer = (x1 * x2) / 100;
	return buffer;
}
fixed32_3 mul32_3(fixed32_3 x1,fixed32_3 x2)
{
	fixed32_3 buffer = (x1 * x2) / 1000;
	return buffer;
}
fixed32_4 mul32_4(fixed32_4 x1,fixed32_4 x2)
{
	fixed32_4 buffer = (x1 * x2) / 10000;
	return buffer;
}

//MUCH slower multiplies for larger, higher precision multiplies
//lhp: large high precision
fixed32_1 mul32_1_lhp(fixed32_1 x1,fixed32_1 x2)
{
	fixed64_1 buffer = ((fixed64_1)x1 * (fixed64_1)x2) / 10;
	return (fixed32_1)buffer;
}
fixed32_2 mul32_2_lhp(fixed32_2 x1,fixed32_2 x2)
{
	fixed64_2 buffer = ((fixed64_2)x1 * (fixed64_2)x2) / 100;
	return (fixed32_2)buffer;
}
fixed32_3 mul32_3_lhp(fixed32_3 x1,fixed32_3 x2)
{
	fixed64_3 buffer = ((fixed64_3)x1 * (fixed64_3)x2) / 1000;
	return (fixed32_3)buffer;
}
fixed32_4 mul32_4_lhp(fixed32_4 x1,fixed32_4 x2)
{
	fixed64_4 buffer = ((fixed64_4)x1 * (fixed64_4)x2) / 10000;
	return (fixed32_4)buffer;
}


//llp: large low precision
fixed32_1 mul32_1_llp(fixed32_1 x1,fixed32_1 x2)
{
	fixed32_1 buffer = (x1/10) * x2;
	return buffer;
}
fixed32_2 mul32_2_llp(fixed32_2 x1,fixed32_2 x2)
{
	fixed32_2 buffer = (x1/10) * (x2/10);
	return buffer;
}
fixed32_3 mul32_3_llp(fixed32_3 x1,fixed32_3 x2)
{
	fixed32_3 buffer = (x1/100) * (x2/10);
	return buffer;
}
fixed32_4 mul32_4_llp(fixed32_4 x1,fixed32_4 x2)
{
	fixed32_4 buffer = (x1/100) * (x2/100);
	return buffer;
}


//Fast, general purpose divide. numerator * (number of decimal places) cannot exceed 2^(32-1)
fixed32_1 div32_1(fixed32_1 numerator,fixed32_1 denominator)
{
	fixed32_1 buffer = (numerator*10)/(denominator);
	return buffer;
}
fixed32_2 div32_2(fixed32_2 numerator,fixed32_2 denominator)
{
	fixed32_2 buffer = (numerator*100)/(denominator);
	return buffer;
}
fixed32_3 div32_3(fixed32_3 numerator,fixed32_3 denominator)
{
	fixed32_3 buffer = (numerator*1000)/(denominator);
	return buffer;
}
fixed32_4 div32_4(fixed32_4 numerator,fixed32_4 denominator)
{
	fixed32_4 buffer = (numerator*10000)/(denominator);
	return buffer;
}

//Slow, high precision divide for larger numerators
fixed32_1 div32_1_lhp(fixed32_1 numerator,fixed32_1 denominator)
{
	fixed64_1 buffer = ((fixed64_1)numerator*10)/(fixed64_1)denominator;
	return (fixed32_1)buffer;
}
fixed32_2 div32_2_lhp(fixed32_2 numerator,fixed32_2 denominator)
{
	fixed64_1 buffer = ((fixed64_2)numerator*100)/(fixed64_2)denominator;
	return (fixed32_2)buffer;
}
fixed32_3 div32_3_lhp(fixed32_3 numerator,fixed32_3 denominator)
{
	fixed64_1 buffer = ((fixed64_3)numerator*1000)/(fixed64_3)denominator;
	return (fixed32_3)buffer;
}
fixed32_4 div32_4_lhp(fixed32_4 numerator,fixed32_4 denominator)
{
	fixed64_1 buffer = ((fixed64_4)numerator*10000)/(fixed64_4)denominator;
	return (fixed32_4)buffer;
}

//Fast, low precision divide for larger numerators
fixed32_1 div32_1_llp(fixed32_1 numerator,fixed32_1 denominator)
{
	fixed32_1 buffer = (numerator/denominator)*10;
	return buffer;
}
fixed32_2 div32_2_llp(fixed32_2 numerator,fixed32_2 denominator)
{
	fixed32_2 buffer = (numerator/denominator)*100;
	return buffer;
}
fixed32_3 div32_3_llp(fixed32_3 numerator,fixed32_3 denominator)
{
	fixed32_3 buffer = (numerator/denominator)*1000;
	return buffer;
}
fixed32_4 div32_4_llp(fixed32_4 numerator,fixed32_4 denominator)
{
	fixed32_4 buffer = (numerator/denominator)*10000;
	return buffer;
}


//UNSIGNED

//General purpose faster, lower quantity multiplies.
//(x1 * x2 * (decimal places *10)) cannot exceed 2^(32-1)/(decimal points)
ufixed32_1 umul32_1(ufixed32_1 x1,ufixed32_1 x2)
{
	ufixed32_1 buffer = (x1 * x2) / 10;
	return buffer;
}
ufixed32_2 umul32_2(ufixed32_2 x1,ufixed32_2 x2)
{
	ufixed32_2 buffer = (x1 * x2) / 100;
	return buffer;
}
ufixed32_3 umul32_3(ufixed32_3 x1,ufixed32_3 x2)
{
	ufixed32_3 buffer = (x1 * x2) / 1000;
	return buffer;
}
ufixed32_4 umul32_4(ufixed32_4 x1,ufixed32_4 x2)
{
	ufixed32_4 buffer = (x1 * x2) / 10000;
	return buffer;
}

//MUCH slower multiplies for larger, higher precision multiplies
//lhp: large high precision
ufixed32_1 umul32_1_lhp(ufixed32_1 x1,ufixed32_1 x2)
{
	ufixed64_1 buffer = ((ufixed64_1)x1 * (ufixed64_1)x2) / 10;
	return (ufixed32_1)buffer;
}
ufixed32_2 umul32_2_lhp(ufixed32_2 x1,ufixed32_2 x2)
{
	ufixed64_2 buffer = ((ufixed64_2)x1 * (ufixed64_2)x2) / 100;
	return (ufixed32_2)buffer;
}
ufixed32_3 umul32_3_lhp(ufixed32_3 x1,ufixed32_3 x2)
{
	ufixed64_3 buffer = ((ufixed64_3)x1 * (ufixed64_3)x2) / 1000;
	return (ufixed32_3)buffer;
}
ufixed32_4 umul32_4_lhp(ufixed32_4 x1,ufixed32_4 x2)
{
	ufixed64_4 buffer = ((ufixed64_4)x1 * (ufixed64_4)x2) / 10000;
	return (ufixed32_4)buffer;
}


//llp: large low precision
ufixed32_1 umul32_1_llp(ufixed32_1 x1,ufixed32_1 x2)
{
	ufixed32_1 buffer = (x1/10) * x2;
	return buffer;
}
ufixed32_2 umul32_2_llp(ufixed32_2 x1,ufixed32_2 x2)
{
	ufixed32_2 buffer = (x1/10) * (x2/10);
	return buffer;
}
ufixed32_3 umul32_3_llp(ufixed32_3 x1,ufixed32_3 x2)
{
	ufixed32_3 buffer = (x1/100) * (x2/10);
	return buffer;
}
ufixed32_4 umul32_4_llp(ufixed32_4 x1,ufixed32_4 x2)
{
	ufixed32_4 buffer = (x1/100) * (x2/100);
	return buffer;
}

//Fast, general purpose divide. numerator * (number of decimal places) cannot exceed 2^(32-1)
ufixed32_1 udiv32_1(ufixed32_1 numerator,ufixed32_1 denominator)
{
	ufixed32_1 buffer = (numerator*10)/(denominator);
	return buffer;
}
ufixed32_2 udiv32_2(ufixed32_2 numerator,ufixed32_2 denominator)
{
	ufixed32_2 buffer = (numerator*100)/(denominator);
	return buffer;
}
ufixed32_3 udiv32_3(ufixed32_3 numerator,ufixed32_3 denominator)
{
	ufixed32_3 buffer = (numerator*1000)/(denominator);
	return buffer;
}
ufixed32_4 udiv32_4(ufixed32_4 numerator,ufixed32_4 denominator)
{
	ufixed32_4 buffer = (numerator*10000)/(denominator);
	return buffer;
}

//Slow, high precision divide for larger numerators
ufixed32_1 udiv32_1_lhp(ufixed32_1 numerator,ufixed32_1 denominator)
{
	fixed64_1 buffer = ((fixed64_1)numerator*10)/(fixed64_1)denominator;
	return (ufixed32_1)buffer;
}
ufixed32_2 udiv32_2_lhp(ufixed32_2 numerator,ufixed32_2 denominator)
{
	fixed64_1 buffer = ((fixed64_2)numerator*100)/(fixed64_2)denominator;
	return (ufixed32_2)buffer;
}
ufixed32_3 udiv32_3_lhp(ufixed32_3 numerator,ufixed32_3 denominator)
{
	fixed64_1 buffer = ((fixed64_3)numerator*1000)/(fixed64_3)denominator;
	return (ufixed32_3)buffer;
}
ufixed32_4 udiv32_4_lhp(ufixed32_4 numerator,ufixed32_4 denominator)
{
	fixed64_1 buffer = ((fixed64_4)numerator*10000)/(fixed64_4)denominator;
	return (ufixed32_4)buffer;
}

//Fast, low precision divide for larger numerators
ufixed32_1 udiv32_1_llp(ufixed32_1 numerator,ufixed32_1 denominator)
{
	ufixed32_1 buffer = (numerator/denominator)*10;
	return buffer;
}
ufixed32_2 udiv32_2_llp(ufixed32_2 numerator,ufixed32_2 denominator)
{
	ufixed32_2 buffer = (numerator/denominator)*100;
	return buffer;
}
ufixed32_3 udiv32_3_llp(ufixed32_3 numerator,ufixed32_3 denominator)
{
	ufixed32_3 buffer = (numerator/denominator)*1000;
	return buffer;
}
ufixed32_4 udiv32_4_llp(ufixed32_4 numerator,ufixed32_4 denominator)
{
	ufixed32_4 buffer = (numerator/denominator)*10000;
	return buffer;
}


/////////
//64 bit fixed

fixed64_1 mul64_1(fixed64_1 x1, fixed64_1 x2)
{
  return (x1 * x2 /10);
}

fixed64_2 mul64_2(fixed64_2 x1, fixed64_2 x2)
{
  return (x1 * x2 /100);
}

fixed64_3 mul64_3(fixed64_3 x1, fixed64_3 x2)
{
  return (x1 * x2 /1000);
}

fixed64_4 mul64_4(fixed64_4 x1, fixed64_4 x2)
{
  return (x1 * x2 /10000);
}


ufixed64_1 umul64_1(ufixed64_1 x1, ufixed64_1 x2)
{
  return (x1 * x2 /10);
}

ufixed64_2 umul64_2(ufixed64_2 x1, ufixed64_2 x2)
{
  return (x1 * x2 /100);
}

ufixed64_3 umul64_3(ufixed64_3 x1, ufixed64_3 x2)
{
  return (x1 * x2 /1000);
}

ufixed64_4 umul64_4(ufixed64_4 x1, ufixed64_4 x2)
{
  return (x1 * x2 /10000);
}


fixed64_1 div64_1(fixed64_1 numerator, fixed64_1 denominator)
{
  return (numerator * 10)/denominator;
}

fixed64_2 div64_2(fixed64_2 numerator, fixed64_2 denominator)
{
  return (numerator * 100)/denominator;
}

fixed64_3 div64_3(fixed64_3 numerator, fixed64_3 denominator)
{
  return (numerator * 1000)/denominator;
}

fixed64_4 div64_4(fixed64_4 numerator, fixed64_4 denominator)
{
  return (numerator * 10000)/denominator;
}


ufixed64_1 udiv64_1(ufixed64_1 numerator, ufixed64_1 denominator)
{
  return (numerator * 10)/denominator;
}

ufixed64_2 udiv64_2(ufixed64_2 numerator, ufixed64_2 denominator)
{
  return (numerator * 100)/denominator;
}

ufixed64_3 udiv64_3(ufixed64_3 numerator, ufixed64_3 denominator)
{
  return (numerator * 1000)/denominator;
}

ufixed64_4 udiv64_4(ufixed64_4 numerator, ufixed64_4 denominator)
{
  return (numerator * 10000)/denominator;
}
