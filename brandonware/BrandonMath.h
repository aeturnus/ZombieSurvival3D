#ifndef BRANDONMATH_H
#define BRANDONMATH_H

#include <stdint.h>
#include "BrandonTypes.h"

#define abs(a) (a<0?-(a):(a))

typedef struct BM_Point
{
  fixed32_3 x,y,z;
} point;

typedef struct BM_Vector
{
  fixed32_3 x,y,z;
} vector;


/* Functions */
//---- Normalize Angle ----
// Inputs : int angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
fixed32_3 normalizeAngle(int32_t angle);

//---- Normalize Angle Fix ----
// Inputs : fixed point angle in degrees
// Output : fixed point angle mapped to 0-360.000
//
fixed32_3 normalizeAngleFix(fixed32_3 angle);

//---- Normalize Angle ----
// Inputs : Pointer to int angle in degrees
// Output : None
//
void normalizeAnglePtr(int32_t* angleIn);

//---- Normalize Angle Fix ----
// Inputs : Pointer to fixed point angle in degrees
// Output :
//
void normalizeAngleFixPtr(fixed32_3* angleIn);

//---- Sine ----
// Inputs : angle in degrees
// Outputs: fixed point sine
// Takes an angle in degrees and returns the sine of it
fixed32_3 sine(int32_t angle);

//---- Sine Fixed----
// Inputs : angle in degrees
// Outputs: fixed point sine
// Takes a fixed point angle in degrees and returns the sine of it, with linear interpolation
fixed32_3 sineFix(fixed32_3 angle);

//---- Cosine ----
// Inputs : angle in degrees
// Outputs: fixed point cosine
// Takes an angle in degrees and returns the cosine of it
fixed32_3 cosine(int32_t angle);

//---- Cosine Fixed----
// Inputs : angle in degrees
// Outputs: fixed point cosine
// Takes a fixed point angle in degrees and returns the cosine of it, with linear interpolation
fixed32_3 cosineFix(fixed32_3 angle);

//---- Tangent ----
// Inputs : angle in degrees
// Outputs: fixed point tangent
// Takes an angle in degrees and returns the tangent of it
fixed32_3 tangent(int32_t angle);

//---- Tangent Fixed----
// Inputs : angle in degrees
// Outputs: fixed point tangent
// Takes a fixed point angle in degrees and returns the tangent of it, with linear interpolation
fixed32_3 tangentFix(fixed32_3 angle);

//---- Atan2 Fixed ----
// Inputs: x, y
// Outputs: angle
// Calculates atan2, using a binary search  with tanFix()

fixed32_3 atan2Fix(fixed32_3 x, fixed32_3 y);

//---- Point Add Vector ----
// Inputs : pointer to point, pointer to vector
// Outputs: point
// Adds a vector to a point
point pointAddVector(point* p, vector* v);

//---- Point Sub Vector ----
// Inputs : pointer to point, pointer to vector
// Outputs: point
// Subtracts a vector from a point
point pointSubVector(point* p, vector* v);

//---- Point to Vector ----
// Inputs : pointer to point p, pointer to point q
// Outputs: vector
// Returns the vector PQ from points p,q
vector pointsToVector(point* p, point* q);

//---- Vector Scale ----
// Inputs: pointer to vector, scale value
// Output: vector
// Scales vector c*V
vector vectorScale(vector* v1, fixed32_3 scale);

//---- Vector Scale Invert ----
// Inputs: pointer to vector, scale value
// Output: vector
// Scales vector 1/c * V
vector vectorScaleInvert(vector* v1, fixed32_3 scale);

//---- Vector Add ----
// Inputs : pointer to vector, pointer to vector
// Outputs: vector
// Adds two vectors: v1 + v2
vector vectorAdd(vector* v1, vector* v2);

//---- Vector Sub ----
// Inputs : pointer to vector, pointer to vector
// Outputs: vector
// Adds two vectors: v1 - v2
vector vectorSub(vector* v1, vector* v2);

//---- Vector Dot Product ----
// Inputs : pointer to vector, pointer to vector
// Outputs: dot product fixed32_3
// Calculates the dot product of two vectors
fixed32_3 vectorDotProduct(vector* v1, vector* v2);

//---- Vector Cross Product ----
// Inputs : pointer to vector, pointer to vector
// Outputs: cross product vector
// Calculates the cross product of two vectors
vector vectorCrossProduct(vector* v1, vector* v2);

vector polarToVector(fixed32_3 mag, fixed32_3 angle);

//---- Square Root ----
// Inputs: x is the the operand
// Outputs: the square root of x
// Calculates a square root
fixed32_3 sqrtFix(fixed32_3 n);

//---- Square Root ----
// Inputs: x is the the operand
// Outputs: the square root of x
// Calculates a square root
int32_t sqrtInt(int32_t n);

//---- Angle Rel----
// Inputs: position vector of origin, position vector of other
// Outputs: angle
// Finds the angle of another position vector relative to the current position vector
fixed32_3 angleRel(vector* origin, vector* offset);

//---- Get Distance---
// Inputs: Two vectors
// Output: Distance
fixed32_3 getDistance2D(vector* v1, vector* v2);

//---- Get Distance---
// Inputs: Two vectors
// Output: Distance
fixed32_3 getDistance(vector* v1, vector* v2);


#endif
