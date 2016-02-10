#include "BrandonTypes.h"
#include "BrandonMath.h"

//This is a sine lookup table regarding angles 0-90
//This is an array of the numerator portion of a fraction struct with denominator 1000, or the fractional bits of a fixed point notation
const int16_t sinTable[] =
{
  0,17,35,52,70,87,105,122,139,156,174,
  191,208,225,242,259,276,292,309,326,342,
  358,375,391,407,423,438,454,470,485,500,
  515,530,545,560,574,588,602,616,629,643,
  656,669,682,695,707,719,731,743,755,766,
  777,788,799,809,919,829,839,848,857,866,
  875,883,891,899,906,914,921,927,934,940,
  946,951,956,961,966,970,974,978,982,985,
  988,990,923,995,996,998,999,999,999,1000
};
const int16_t tanTable[] =
{
  0,18,35,52,70,88,105,123,141,158,176,
  194,213,231,249,268,287,306,325,344,364,
  384,404,425,445,466,488,510,532,554,577,
  601,625,649,675,700,727,754,781,810,839,
  869,900,833,966,1000,1036,1072,1111,1150,1192,
  1235,1280,1327,1376,1428,1483,1540,1600,1664,1732,
  1804,1891,1963,2050,2145,2246,2356,2475,2605,2748,
  2904,3078,3271,3487,3732,4011,4332,4705,5145,5671,6314,7115,8144,9514,11430,14301,19081,28636,32767
  //57290
};
const fixed32_3 atanTable[] =
{
  0,703,1406,2109,2813,3516,4219,4922,5625,6328,7031,
  7734,8438,9141,9844,10547,11250,11953,12656,13359,14063,
  14766,15469,16172,16875,17578,18281,18984,19688,20391,21094,
  21797,22500,23203,23906,24609,25313,26016,26719,27422,28125,
  28828,29531,30234,30938,31641,32344,33047,33750,34453,35156,
  35859,36563,37266,37969,38672,39375,40078,40781,41484,42188,
  42891,43594,44297,45000,45703,46406,47109,47813,48516,49219,
  49922,50625,51328,52031,52734,53438,54141,54844,55547,56250,
  56953,57656,58359,59063,59766,60469,51172,61875,62578,63281,
  63984,64688,65391,66094,66797,67500,68203,68906,69609,70313,
  71016,71719,72422,73125,73828,74531,75234,75938,76641,77344,
  78047,78750,79453,80156,80859,81563,82266,82969,83672,84375,
  85078,85781,86484,87188,87891,88594,89297,90000
};


//---- Normalize Angle ----
// Inputs : fixed point angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
int32_t normalizeAngle(int32_t angle)
{
  if(angle >= 360 || angle < 0)
  {
    int32_t times = angle /360; //how many 360's are past
    angle -=  360 * times; //If the angle is already negative, it already adds
    if(angle < 0)
        {
            angle += 360;
        }
  }
  return angle;
}

//---- Normalize Angle Fix ----
// Inputs : fixed point angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
fixed32_3 normalizeAngleFix(fixed32_3 angle)
{
  if(angle >= 360000 || angle < 0000)
  {
    int32_t times = angle / 360000; //number of int times to move it
    angle -= 360000 * times;
    if(angle < 0000)
        {
            angle += 360000;
        }
  }
  return angle;
}


//---- Normalize Angle ----
// Inputs : fixed point angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
void normalizeAnglePtr(int32_t* angleIn)
{
  int32_t angle = *angleIn;
  if(angle >= 360 || angle < 0)
  {
    int32_t times = angle /360; //how many 360's are past
    angle -=  360 * times; //If the angle is already negative, it already adds
    if(angle < 0)
        {
            angle += 360;
        }
  }
  *angleIn = angle;
}

//---- Normalize Angle Fix ----
// Inputs : fixed point angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
void normalizeAngleFixPtr(fixed32_3* angleIn)
{
  fixed32_3 angle = *angleIn;
  if(angle >= 360000 || angle < 0000)
  {
    int32_t times = angle / 360000; //number of int times to move it
    angle -= 360000 * times;
    if(angle < 0000)
        {
            angle += 360000;
        }
  }
  *angleIn = angle;
}

//---- Sine ----
// Inputs : angle in degrees
// Outputs: fixed point sine
// Takes an angle in degrees and returns the sine of it
fixed32_3 sine(int32_t angle)
{
  //Normalize the angle to 0-360
  /*
  while(angle >= 360)
  {
    angle -= 360;
  }
  while(angle < 0)
  {
    angle += 360;
  }
    */
    //angle = normalizeAngle(angle);
    normalizeAnglePtr(&angle);

  uint32_t index = 0;     // Index of sine table
  uint8_t negative = 0;   // Is it negative?

  fixed32_3 output = 0;

  //Index calculation
  if(0 <= angle && angle <= 90)
  {
    index = angle;
  }
  else if (90 < angle && angle <= 180)
  {
    index = 180 - angle;
  }
  else if (180 < angle && angle <= 270)
  {
    negative = 1;
    index = angle - 180;
  }
  else
  {
    //if 270<angle<360
    negative = 1;
    index = 360 - angle;
  }
  output = (fixed32_3)sinTable[index];
  if(negative)
  {
    output *= -1; //Negate the result
  }
  return output;
}

//---- Sine Fixed----
// Inputs : angle in degrees
// Outputs: fixed point sine
// Takes a fixed point angle in degrees and returns the sine of it, with linear interpolation
fixed32_3 sineFix(fixed32_3 angle)
{
  //Normalize the angle to 0-360
  /*
  while(angle >= 360000)
  {
    angle -= 360000;
  }
  while(angle < 0)
  {
    angle += 360000;
  }
  */
  //angle = normalizeAngleFix(angle);
  normalizeAngleFixPtr(&angle);

  fixed32_3 mod = angle%1000; // Get the fractional number
  angle /= 1000;
  uint32_t index = 0;     // Index of sine table
  uint8_t negative = 0;   // Is it negative?

  fixed32_3 output = 0;
  fixed32_3 alias0;
  fixed32_3 alias1;
  //Index calculation
  if(0 <= angle && angle <= 90)
  {
    index = angle;
  }
  else if (90 < angle && angle <= 180)
  {
    index = 180 - angle;
  }
  else if (180 < angle && angle <= 270)
  {
    negative = 1;
    index = angle - 180;
  }
  else
  {
    //if 270<angle<360
    negative = 1;
    index = 360 - angle;
  }
  //output = (fixed32_3)sinTable[index];
  if(mod==0)
  {
    output = (fixed32_3)sinTable[index];
  }
  else
  {
    alias0 = (fixed32_3)sinTable[index];
    alias1 = (fixed32_3)sinTable[index+1];
    output = mul32_3((alias1-alias0),mod) + alias0; //(alias1-alias0)*mod + alias0 to get an inbetween value
  }
  if(negative)
  {
    output *= -1; //Negate the result
  }
  return output;
}

//---- Cosine ----
// Inputs : angle in degrees
// Outputs: fixed point cosine
// Takes an angle in degrees and returns the cosine of it
fixed32_3 cosine(int32_t angle)
{
  return sine(angle+90);
}

//---- Cosine Fixed----
// Inputs : angle in degrees
// Outputs: fixed point cosine
// Takes a fixed point angle in degrees and returns the cosine of it, with linear interpolation
fixed32_3 cosineFix(fixed32_3 angle)
{
  return sineFix(angle+90000);
}

//---- Tangent ----
// Inputs : angle in degrees
// Outputs: fixed point tangent
// Takes an angle in degrees and returns the tangent of it
fixed32_3 tangent(int32_t angle)
{
  //Normalize the angle to 0-360
  while(angle >= 360)
  {
    angle -= 360;
  }
  while(angle < 0)
  {
    angle += 360;
  }

  uint32_t index = 0;     // Index of sine table
  uint8_t negative = 0;   // Is it negative?

  fixed32_3 output = 0;

  //Index calculation
  if(0 <= angle && angle <= 90)
  {
    index = angle;
  }
  else if (90 < angle && angle <= 180)
  {
    negative = 1;
    index = 180 - angle;
  }
  else if (180 < angle && angle <= 270)
  {
    index = angle - 180;
  }
  else
  {
    //if 270<angle<360
    negative = 1;
    index = 360 - angle;
  }
  output = (fixed32_3)tanTable[index];
  if(negative)
  {
    output *= -1; //Negate the result
  }
  return output;
}

//---- Tangent Fixed----
// Inputs : angle in degrees
// Outputs: fixed point tangent
// Takes a fixed point angle in degrees and returns the tangent of it, with linear interpolation
fixed32_3 tangentFix(fixed32_3 angle)
{
  //Normalize the angle to 0-360
  while(angle >= 360000)
  {
    angle -= 360000;
  }
  while(angle < 0)
  {
    angle += 360000;
  }

  fixed32_3 mod = angle%1000; // Get the fractional number
  angle /= 1000;
  uint32_t index = 0;     // Index of sine table
  uint8_t negative = 0;   // Is it negative?

  fixed32_3 output = 0;
  fixed32_3 alias0;
  fixed32_3 alias1;
  //Index calculation
  if(0 <= angle && angle <= 90)
  {
    index = angle;
  }
  else if (90 < angle && angle <= 180)
  {
    negative = 1;
    index = 180 - angle;
  }
  else if (180 < angle && angle <= 270)
  {
    index = angle - 180;
  }
  else
  {
    //if 270<angle<360
    negative = 1;
    index = 360 - angle;
  }
  //output = (fixed32_3)sinTable[index];
  if(mod==0)
  {
    output = (fixed32_3)tanTable[index];
  }
  else
  {
    alias0 = (fixed32_3)tanTable[index];
    alias1 = (fixed32_3)tanTable[index+1];
    output = mul32_3((alias1-alias0),mod) + alias0; //(alias1-alias0)*mod + alias0 to get an inbetween value
  }
  if(negative)
  {
    output *= -1; //Negate the result
  }
  return output;
}

//---- Point Add Vector ----
// Inputs : pointer to point, pointer to vector
// Outputs: point
// Adds a vector to a point
point pointAddVector(point* p, vector* v)
{
  point output;
  output.x = p->x + p->x;
  output.x = p->y + p->y;
  output.x = p->z + p->z;
  return output;
}

//---- Point Sub Vector ----
// Inputs : pointer to point, pointer to vector
// Outputs: point
// Subtracts a vector from a point
point pointSubVector(point* p, vector* v)
{
  point output;
  output.x = p->x - p->x;
  output.x = p->y - p->y;
  output.x = p->z - p->z;
  return output;
}

//---- Point to Vector ----
// Inputs : pointer to point p, pointer to point q
// Outputs: vector
// Returns the vector PQ from points p,q
vector pointsToVector(point* p, point* q)
{
  vector output;
  output.x = q->x - p->x;
  output.y = q->y - p->y;
  output.z = q->z - p->z;
  return output;
}

//---- Vector Scale ----
// Inputs: pointer to vector, scale value
// Output: vector
// Scales vector c*V
vector vectorScale(vector* v1, fixed32_3 scale)
{
  vector output;
  output.x = mul32_3(v1->x,scale);
  output.y = mul32_3(v1->y,scale);
  output.z = mul32_3(v1->z,scale);
  return output;
}


//---- Vector Scale Invert ----
// Inputs: pointer to vector, scale value
// Output: vector
// Scales vector 1/c * V
vector vectorScaleInvert(vector* v1, fixed32_3 scale)
{
  vector output;
  output.x = div32_3(v1->x,scale);
  output.y = div32_3(v1->y,scale);
  output.z = div32_3(v1->z,scale);
  return output;
}

//---- Vector Add ----
// Inputs : pointer to vector, pointer to vector
// Outputs: vector
// Adds two vectors: v1 + v2
vector vectorAdd(vector* v1, vector* v2)
{
  vector output;
  output.x = v1->x + v2->x;
  output.y = v1->y + v2->y;
  output.z = v1->z + v2->z;
  return output;
}

//---- Vector Sub ----
// Inputs : pointer to vector, pointer to vector
// Outputs: vector
// Adds two vectors: v1 - v2
vector vectorSub(vector* v1, vector* v2)
{
  vector output;
  output.x = v1->x - v2->x;
  output.y = v1->y - v2->y;
  output.z = v1->z - v2->z;
  return output;
}

//---- Vector Dot Product ----
// Inputs : pointer to vector, pointer to vector
// Outputs: dot product fixed32_3
// Calculates the dot product of two vectors
fixed32_3 vectorDotProduct(vector* v1, vector* v2)
{
  fixed32_3 output;
  output = (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z)*(v2->z);
  return output;
}

//---- Vector Cross Product ----
// Inputs : pointer to vector, pointer to vector
// Outputs: cross product vector
// Calculates the cross product of two vectors
vector vectorCrossProduct(vector* v1, vector* v2)
{
  vector output;
  output.x = (v1->y) * (v2->z) - (v1->z) * (v2->y);
  output.y = (v1->z) * (v2->x) - (v1->x) * (v2->z);
  output.z = (v1->x) * (v2->y) - (v1->y) * (v2->x);
  return output;
}

vector polarToVector(fixed32_3 mag, fixed32_3 angle)
{
  vector output = {0,0,0};
  
  output.x = mul32_3(mag,cosineFix(angle));
  output.y = mul32_3(mag,sineFix(angle));
  output.z = 0;
  
  return output;
}


//---- Square Root ----
// Inputs: x is the the operand
// Outputs: the square root of x
// Calculates a square root
// Uses the fact that sqrt(A * B) = sqrt(A) * sqrt(B)
fixed32_3 sqrtFix(fixed32_3 n){
	if(n<0){
		return -1;
	}else{
		return udiv32_3_lhp(sqrtInt(n) * 1000, 31623);
	}
}

#define iter1(N) \
    try = root + (1 << (N)); \
    if (n >= try << (N))   \
    {   n -= try << (N);   \
        root |= 2 << (N); \
    }

//---- Square Root ----
// Inputs: x is the the operand
// Outputs: the square root of x
// Calculates a square root
int32_t sqrtInt(int32_t n){
  if(n < 0){
    return -1;
  }else{
    uint32_t root = 0, try;
    iter1(15);    iter1(14);    iter1(13);    iter1(12);
    iter1(11);    iter1(10);    iter1( 9);    iter1( 8);
    iter1( 7);    iter1( 6);    iter1( 5);    iter1( 4);
    iter1( 3);    iter1( 2);    iter1( 1);    iter1( 0);
    return root >> 1;
  }
}

//---- Atan2 Fixed ----
// Inputs: x, y
// Outputs: angle
// Calculates atan2, using a binary search  with tanFix()

fixed32_3 atan2Fix(fixed32_3 x, fixed32_3 y)
{
  if(y == 0)
  {
    return 0000;
  }
  if(x == 0)
  {
    return 90000;
  }
  fixed32_3 offset = 0;

  //Get angle offsets for angles > 90°
  fixed32_3 oldX = x;
  fixed32_3 oldY = y;
  if(x < 0)
  {
    if(y>0)
    {
      x = oldY;
      y = -oldX;
      offset = 90000;
    }
    else  //if y<0
    {
      x = -oldX;
      y = -oldY;
      offset = 180000;
    }
  }
  else if(y<0)
  {
    x = -oldY;
    y = oldX;
    offset = 270000;
  }
  #define ERROR 10   //There's an error of .5 of an angle
  #define MAX 1000
  ufixed32_3 ratio = udiv32_3_lhp(y,x);
  if(ratio > 10000)
  {
    return 90000 + offset;
  }
  fixed32_3 angle = 45000;  //binary search starting at 45.000
  fixed32_3 result = tangentFix(angle);
  uint16_t iterations = 1;
  while(abs(ratio-result)>ERROR)
  {
    if(result<ratio)
    {
      angle = angle + (angle/2);
    }
    else
    {
      angle = angle - (angle/2);
    }
    result = tangentFix(angle);
    iterations++;
    if(iterations > MAX)
    {
      return angle+offset;
    }
  }
  return angle + offset;
}


//---- Angle Rel----
// Inputs: position vector of origin, position vector of other
// Outputs: angle
// Finds the angle of another position vector relative to the current position vector
fixed32_3 angleRel(vector* origin, vector* offset)
{
  fixed32_3 x = offset->x - origin->x;  //Relative coords
  fixed32_3 y = offset->y - origin->y;
  return atan2Fix(x,y);
}

//---- Get Distance---
// Inputs: Two vectors
// Output: Distance
fixed32_3 getDistance2D(vector* v1, vector* v2)
{
  fixed32_3 x = v2->x - v1->x;
  fixed32_3 y = v2->y - v1->y;
  return sqrtFix(mul32_3_lhp(x,x) + mul32_3_lhp(y,y));
}

//---- Get Distance---
// Inputs: Two vectors
// Output: Distance
fixed32_3 getDistance(vector* v1, vector* v2)
{
  fixed32_3 x = v2->x - v1->x;
  fixed32_3 y = v2->y - v1->y;
  fixed32_3 z = v2->z - v1->z;
  return sqrtFix(mul32_3_lhp(x,x) + mul32_3_lhp(y,y) + mul32_3_lhp(z,z));
}

