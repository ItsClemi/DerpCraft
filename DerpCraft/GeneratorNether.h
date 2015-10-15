#pragma once
#include "GeneratorBase.h"

class CGeneratorNether : public CGeneratorBase
{
public:
	CGeneratorNether();

public:
	virtual void GenerateChunk( _In_ IChunkSection* pChunkSection );
	virtual void SetupGenerator( _In_ CWorld* pWorld );
};

