#pragma once
#include <minwinbase.h>
#include <winnt.h>
#include <functional>
#include <synchapi.h>
#include <rpcndr.h>
#include <WinSock2.h>
#include <zlib.h>
#include "IBlock.h"
#include "IWorld.h"


class IChunk
{
public:
	IChunk()
	{
		ClearChunk();

		m_bGroundUpContinuous = true;
		m_uAddBitMap = 0;	
		m_uPrimaryBitMap = 0;
		m_bCanUnload = true;
	}
public:
	BlockMeta GetBlockMeta( _In_ byte x, _In_ byte y, _In_ byte z)
	{
		int i = y << 8 | z << 4 | x;
		return ( i & 1 ) ? ( m_bMeta[ i >> 1 ] >> 4 & 15 ) : ( m_bMeta[ i >> 1 ] & 15 );
	}

	inline byte GetBlock( _In_ byte x, _In_ byte y, _In_ byte z )		{	return m_bData[ y << 8 | z << 4 | x ];	}
	inline byte GetSkyLight( _In_ byte x, _In_ byte y, _In_ byte z )	
	{	
		int i = y << 8 | z << 4 | x; 
		return i & 1? m_bSkylight[i] >> 4: m_bSkylight[i] & 15;	
	}
public:
	inline void SetBlock( _In_ byte x, _In_ byte y, _In_ byte z, _In_ byte bNewBlock = NULL, _In_ byte bMeta = NULL )
	{
		int i = y << 8 | z << 4 | x;
		m_bData[i] = bNewBlock;
		i & 1? m_bMeta[ i >> 1 ] |= bMeta & 15 : m_bMeta[ i >> 1 ] |= bMeta << 4 ;
	}

	inline void SetSkylight( _In_ byte x, _In_ byte y, _In_ byte z, _In_ byte bSkylight = 255 )
	{
		int i = y << 8 | z << 4 | x; 
		i & 1? m_bSkylight[ i >> 1 ] |= bSkylight << 4 : m_bSkylight[ i >> 1 ] |= bSkylight;
	}
public:
	inline int	CoordToIndex( _In_ byte x, _In_ byte y, _In_ byte z )	{	return y << 8 | z << 4 | x;		}
	inline void RemoveBlock( _In_ byte x, _In_ byte y, _In_ byte z )	{	SetBlock( x, y, z );			}
	inline void	ClearChunk() 
	{ 
		ZeroMemory( m_bData, 4096 );
		ZeroMemory( m_bMeta, 2048 );
		ZeroMemory( m_bSkylight, 2048 );
		ZeroMemory( m_bBlockAdd, 2048 );

		memset( m_bLight, 255, 2048 );
	}

	//inline void FillChunk( _In_ BlockId bBlock, _In_ BlockMeta bMeta );

public:
	byte		m_bData[4096];
	byte		m_bMeta[2048];
	byte		m_bLight[2048];
	byte		m_bSkylight[2048];
	byte		m_bBlockAdd[2048];

	bool		m_bGroundUpContinuous;

	u_short		m_uPrimaryBitMap;
	u_short		m_uAddBitMap;

	bool		m_bCanUnload;
};

class IChunkSection
{
public:
	IChunkSection();

public:
	void AllocChunk( _In_ byte y );
	void AllocChunk( _In_ byte yStart, _In_ byte yEnd );

	void DeallocChunk( _In_ byte y );

	void CalculateHeightmap();
	void Relight();
	void ForAllMover( std::function< void (void*) > function);
	void SetBlock( _In_ byte x, _In_ byte y, _In_ byte z, _In_ byte bNewBlock = NULL, _In_ byte bMeta = NULL );


	inline bool BlockCanSeeSky( _In_ byte x, _In_ byte y, _In_ byte z )	{ 	return y >= m_bHeightMap[ z << 4 | x ];		}

public:
	inline byte	GetSectionHeight()							{	return m_bSectionHeight;			}
	inline byte GetBiome( _In_ byte x, _In_ byte z )		{	return m_bBiome[ x | z << 4 ];		}
	inline byte GetHeight( _In_ byte x, _In_ byte z )		{	return m_bHeightMap[ x | z << 4 ];	}

public:
	inline void SetBiome( _In_ byte x, _In_ byte z, _In_ BiomeType bBiome )		{	m_bBiome[ x | z << 4 ] = bBiome;		}
	inline void SetGlobalBiome( _In_ BiomeType bBiome )							{	memset( m_bBiome, bBiome, 256 );		}
	inline void SetHeight( _In_ byte x, _In_ byte z, _In_ byte bHeight )		{	m_bHeightMap[ x | z << 4 ] = bHeight;	}

public:
	inline IChunk* At( _In_ byte y )		{ assert( y < GetSectionHeight() );	return m_pChunk[y];		}


	//private:
	byte		m_bSectionHeight;
	IChunk*		m_pChunk[16];
	BiomeType	m_bBiome[256];
	byte		m_bHeightMap[256];
	std::mutex	m_lock;
};
