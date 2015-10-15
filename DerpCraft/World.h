#pragma once
#include "IWorld.h"
#include "GeneratorBase.h"
#include "Mover.h"
#include "User.h"

class CMover;
class CUser;

class CWorld
{
public:
	CWorld( _In_ int nAllocSize, _In_ CGeneratorBase* pGenerator );
	~CWorld(void);

	void	GenerateChunks( _In_ int nAllocSize );
	void	AllocWorld( _In_ int nAllocSize );
	void	SetTime( _In_ long lTime ) { m_lTime = lTime; }

	void	CreatePyramid( _In_ DirectX::XMINT2 vCenter, _In_ int nHeightFrom, _In_ int nHeightTo, _In_ BlockId bNewBlock, _In_ BlockMeta bBlockMeta );
	void	CreateCircle( _In_ int xCenter, _In_ int zCenter, _In_ int nRad, _In_ BlockId bNewBlock, _In_ BlockMeta bBlockMeta );

	void	ForVisibilityChunkRange( _In_ CUser* pUser, _In_ std::function< void ( _Out_ IChunkSection*, _Out_ int, _Out_ int ) > f )
	{
		int x = (int)pUser->m_vPos.x >> 4;
		int z = (int)pUser->m_vPos.z >> 4;

		for( int _x = x - 10; _x < x + 10; _x++ )
		{
			for( int _z = z -10; _z < z + 10; _z++ )
			{
				IChunkSection* pSect = GetChunkSection( _x, _z );
				f( pSect, _x, _z );
			}
		}
	}

public:
	inline	WeatherType		GetWeather()			{	return m_bWeather;		}
	inline	WorldDimension	GetDimension()			{	return m_cDimension;	}
	inline	CGeneratorBase*	GetWorldGenerator()		{	return m_pGenerator;	}
	inline	long			GetTime()				{	return m_lTime;			}
	inline	std::wstring*	GetworldType()			{	return &m_wszWorldType;	}

	inline	IChunkSection*	GetChunkSection( int x, int z )
	{
		if( x >= 0 && z > 0 )		return m_pChunkMap[ 0 ][  x >> 4  ][  z >> 4  ];
		else if( x < 0 && z >= 0 )	return m_pChunkMap[ 1 ][ -x >> 4  ][  z >> 4  ];
		else if( x < 0 && z < 0 )	return m_pChunkMap[ 2 ][ -x >> 4  ][ -z >> 4  ];
									return m_pChunkMap[ 3 ][  x >> 4  ][ -z >> 4  ];
	}
public:
	inline	void	SetDimension( _In_ WorldDimension cDim )		{	m_cDimension = cDim;		}
	inline	void	SetWorldType( _In_ std::wstring wszType )		{	m_wszWorldType = wszType;	}

private:
	WorldDimension				m_cDimension;
	WeatherType					m_bWeather;
	IChunkSection***			m_pChunkMap[4];
	CGeneratorBase*				m_pGenerator;
	std::wstring				m_wszWorldType;

public:
	DirectX::XMFLOAT3			m_vSpawnPos;

	long	m_lTime;
};

