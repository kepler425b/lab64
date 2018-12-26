#ifndef HEADER_FILE
#define HEADER_FILE
#include <math.h>

struct v2 {
	float x, y;

	float &operator[](int Index) { return((&x)[Index]); }

	inline v2 &operator*=(float A);
	inline v2 &operator+=(v2 A);
};


inline v2
operator+(v2 A, v2 B)
{
	v2 Result;
	Result.x = A.x + B.x;
	Result.y = A.y + B.y;
	return(Result);
}

inline v2 &v2::
operator+=(v2 A)
{
	*this = A + *this;

	return(*this);
}


inline v2 
operator-(v2 A, v2 B)
{
	v2 Result;
	Result.x = A.x - B.x;
	Result.y = A.y - B.y;
	return(Result);
}

inline v2 
operator-(v2 A)
{
	v2 Result;
	Result.x = -A.x;
	Result.y = -A.y;
	return (Result);
}

inline v2
operator*(v2 B, float A)
{
	v2 Result;
	Result.x = B.x * A;
	Result.y = B.y * A;
	return Result;
}

inline v2 &v2::
operator*=(float A)
{
	*this = *this * A;

	return(*this);
}

inline
v2 norm(v2 A)
{
	v2 Result;
	Result.x = A.x / A.x;
	Result.y = A.y / A.y;
	return Result;
}

inline
float mag(v2 A) 
{
	float Result;
	Result = A.x * A.x + A.y * A.y;
	return Result * Result;
}

inline 
float dotProduct(v2 A, v2 B)
{
	float Result;
	Result = A.x * B.x + A.y * B.y;
	return Result;
}

inline v2 rotate(float Angle, v2 B)
{
	v2 Result;
	Result.x = B.x * cos(Angle) - B.y * sin(Angle);
	Result.y = B.y * sin(Angle) + B.x * cos(Angle);
	return (Result);
}


#endif