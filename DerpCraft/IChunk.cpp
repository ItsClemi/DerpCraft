#include "stdafx.h"
#include "IChunk.h"


IChunkSection::IChunkSection()
{ 
	m_bSectionHeight = 0;

	ZeroMemory( m_pChunk, sizeof( m_pChunk ) );
	SetGlobalBiome( BiomePlains );

	memset( m_bHeightMap, -1, 256 );
}

void IChunkSection::AllocChunk( _In_ byte y )
{
	if( y <= GetSectionHeight() )
		return;

	for( byte bHeight = GetSectionHeight(); bHeight < y; bHeight++ )
		m_pChunk[ bHeight ] = new IChunk;

	m_bSectionHeight = y;
}

void IChunkSection::AllocChunk( _In_ byte yFrom, _In_ byte yTo )
{
	for( byte y = yFrom; y < yTo; y++ )
	{
		if( m_pChunk[ y ] == nullptr )
			m_pChunk[ y ] = new IChunk;
	}

	m_bSectionHeight = yTo;
}

void IChunkSection::DeallocChunk( _In_ byte y ) 
{
	for( ; m_bSectionHeight > y-1; m_bSectionHeight-- )
		SAFE_DELETE( m_pChunk[ m_bSectionHeight ] );
}


void IChunkSection::SetBlock( _In_ byte x, _In_ byte y, _In_ byte z, _In_ byte bNewBlock, _In_ byte bMeta )
{
	byte bChunkPos = y >> 4;
	if( m_pChunk[ bChunkPos ] == nullptr )
		AllocChunk( bChunkPos + 1 );

	At( bChunkPos )->SetBlock( x, y & 15, z, bNewBlock, bMeta );
}


void IChunkSection::CalculateHeightmap()
{
	for( byte x = 0; x < 16; x++ )
	{
		for( byte z = 0; z < 16; z++ )
		{
			for( byte y = 16; y > 0; y--)
			{
				if( At( GetSectionHeight() -1 )->GetBlock( x, y, z ) != BLOCK_AIR )
				{
					SetHeight( x, z, y );
					break;
				}
			}
		}
	}
}

void IChunkSection::Relight()
{

}

void IChunkSection::ForAllMover( std::function< void (void*) > function)
{

}
