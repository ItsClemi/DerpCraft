#pragma once
#include "GeneratorBase.h"

class CGeneratorFlat : public CGeneratorBase
{
public:
	CGeneratorFlat();

public:
	virtual void GenerateChunk( _In_ IChunkSection* pChunkSection );
	virtual void SetupGenerator( _In_ CWorld* pWorld );
};

