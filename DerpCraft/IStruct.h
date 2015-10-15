#pragma once

#include <sal.h>

namespace DirectX
{
	struct XMDOUBLE3
	{
	public:
		inline	XMDOUBLE3() {}
		inline	XMDOUBLE3( _In_ double dVal )									{	x = dVal;	y = dVal;	z = dVal;		}
		inline	XMDOUBLE3( _In_ double dX, _In_ double dY, _In_ double dZ )		{	x = dX;		y = dY;		z = dZ;			}
		inline	XMDOUBLE3( _In_ XMDOUBLE3& vec )								{	x = vec.x;	y = vec.y;	z = vec.z;		}

	public:
		inline	XMDOUBLE3& operator	+=	( XMDOUBLE3& vec )						{	x += vec.x;	y += vec.y; z += vec.z;		}
		inline	XMDOUBLE3& operator -=	( XMDOUBLE3& vec )						{	x -= vec.x; y -= vec.y;	z -= vec.z;		}
		inline	XMDOUBLE3& operator *=	( float f )								{	x *= f;		y *= f;		z *= f;			}
		inline	XMDOUBLE3& operator /=	( float f )								{	x /= f;		y /= f;		z /= f;			}

		inline	XMDOUBLE3& operator + ( XMDOUBLE3& vec )						{	x += vec.x; y += vec.y; z += vec.z;		}
		inline	XMDOUBLE3& operator - ( XMDOUBLE3& vec )						{	x -= vec.x; y -= vec.y;	z -= vec.z;		}

	public:
		double x;
		double y;
		double z;
	};


}
