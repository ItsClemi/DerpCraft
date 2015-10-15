#include "stdafx.h"
#include "World.h"


CWorld::CWorld( _In_ int nAllocSize, _In_ CGeneratorBase* pGenerator )
{
	m_pGenerator = pGenerator;
	GetWorldGenerator()->SetupGenerator( this );

	AllocWorld( nAllocSize );
	GenerateChunks( nAllocSize );
}


CWorld::~CWorld(void)
{
}

void CWorld::GenerateChunks( _In_ int nAlocSize )
{
	for( int x = -nAlocSize; x < nAlocSize; x++ )
	{
		for( int z = -nAlocSize; z < nAlocSize; z++ )
			GetWorldGenerator()->GenerateChunk( GetChunkSection( x, z ) );
	}
}

void CWorld::AllocWorld( _In_ int nAllocSize )
{
	for( byte i = 0; i < 4; i++ )
	{
		m_pChunkMap[ i ] = new IChunkSection**[ nAllocSize ];
		for( int x = 0; x < nAllocSize; x++ )
		{
			m_pChunkMap[ i ][ x ] = new IChunkSection*[ nAllocSize ];
			for( int z = 0; z < nAllocSize; z++ )
				m_pChunkMap[ i ][ x ][ z ] = new IChunkSection[ nAllocSize ];
		}
	}
}

void CWorld::CreatePyramid( _In_ XMINT2 vCenter, _In_ int nHeightFrom, _In_ int nHeightTo, _In_ BlockId bNewBlock, _In_ BlockMeta bBlockMeta )
{
	int nSize = nHeightTo - nHeightFrom;
	for( int y = nHeightFrom; y < nHeightTo; y++ )
	{
		nSize--;
		for( int x = vCenter.x - nSize; x <= vCenter.x + nSize; x++ )
		{
			for( int z = vCenter.y - nSize; z <= vCenter.y + nSize; z++ )
			{
				IChunkSection* pSect = GetChunkSection( x, z );
				if( pSect )
				{
					IChunk* pChunk = pSect->At( y >> 4 );
					if( pChunk )
						pChunk->SetBlock( x & 15, y, z & 15, bNewBlock, bBlockMeta );
				}
			}
		}
	}
}


void CWorld::CreateCircle( _In_ int xCenter, _In_ int zCenter, _In_ int nRad, _In_ BlockId bNewBlock, _In_ BlockMeta bBlockMeta )
{
	int xChunk = xCenter >> 4;
	int zChunk = zCenter >> 4;

	/*
	1 1 0 1 1
	1 0 0 0 1
	0 0 0 0 0
	1 0 0 0 1
	1 1 0 1 1
	*/


	for( int x = xCenter - nRad; x < xCenter + nRad; x++  )
	{
		for( int z = zCenter - nRad; z < zCenter + nRad; z++ )
		{
		}
	}

}

