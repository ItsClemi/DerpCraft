#pragma once
#include <math.h>




#define PI	3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f
#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))

inline void EulerToVector(double dPan, double dPitch, double & x, double & y, double & z)
{
	x = cos( dPan / 180 * PI ) * cos( dPitch / 180 * PI );
	y = sin( dPan / 180 * PI ) * cos( dPitch / 180 * PI );
	z = sin( dPitch / 180 * PI );
}


inline void VectorToEuler(double x, double y, double z, double & dPan, double & dPitch)
{
	if (x != 0)
		dPan = atan2( z, x ) * 180 / PI - 90;
	else
		dPan = 0;

	dPitch = atan2( y, sqrt(( x * x ) + ( z * z ))) * 180 / PI;
}


inline float GetSignf(float a_Val)
{
	return (a_Val < 0.f) ? -1.f : 1.f;
}

inline float GetSpecialSignf( float a_Val )
{
	return (a_Val <= 0.f) ? -1.f : 1.f;
}