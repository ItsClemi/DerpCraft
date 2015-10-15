#include "stdafx.h"
#include "GeneratorNether.h"

CGeneratorNether::CGeneratorNether()
{

}

void CGeneratorNether::SetupGenerator( _In_ CWorld* pWorld )
{
	CGeneratorBase::SetupGenerator( pWorld );
	pWorld->SetWorldType( L"default" );
}

void CGeneratorNether::GenerateChunk( _In_ IChunkSection* pChunkSection )
{
	pChunkSection->AllocChunk( 1 );
	pChunkSection->SetGlobalBiome( BiomeNether );

	IChunk* pChunk = pChunkSection->At( 0 );

	for( byte x = 0; x < 16; x++)
	{
		for( byte z = 0; z < 16; z++)
		{
			pChunk->SetBlock( x, 0, z, BLOCK_BEDROCK, 0 );
			for( byte i = 1; i < 15; i++ )
				pChunk->SetBlock( x, i, z, BLOCK_NETHER_STONE, 0 );
			pChunk->SetBlock( x, 15, z, BLOCK_BEDROCK, 0 );
		}
	}

	pChunkSection->CalculateHeightmap();
}