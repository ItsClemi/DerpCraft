#include "stdafx.h"
#include "GeneratorFlat.h"


CGeneratorFlat::CGeneratorFlat()
{
	
}

void CGeneratorFlat::SetupGenerator( _In_ CWorld* pWorld )
{
	CGeneratorBase::SetupGenerator( pWorld );
	pWorld->SetWorldType( L"flat" );
}

void CGeneratorFlat::GenerateChunk( _In_ IChunkSection* pChunkSection )
{
	pChunkSection->AllocChunk( 1 );
	pChunkSection->SetGlobalBiome( BiomePlains );

	IChunk* pChunk = pChunkSection->At( 0 );

	for( byte x = 0; x < 16; x++)
	{
		for( byte z = 0; z < 16; z++)
		{
			pChunk->SetBlock( x, 0, z, BLOCK_BEDROCK, 0 );
			pChunk->SetBlock( x, 1, z, BLOCK_DIRT, 0 );
			pChunk->SetBlock( x, 2, z, BLOCK_DIRT, 0 );
			pChunk->SetBlock( x, 3, z, BLOCK_GRASS, 0 );
		}
	}

	pChunkSection->CalculateHeightmap();
}