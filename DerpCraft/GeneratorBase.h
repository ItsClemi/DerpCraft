#pragma once
#include "IChunk.h"
#include "World.h"

class CWorld;
class CGeneratorBase
{
public:
	CGeneratorBase() { }

public:
	virtual void GenerateChunk( _In_ IChunkSection* pChunk ) = 0;

	virtual void SetupGenerator( _In_ CWorld* pWorld )
	{
		m_pWorld = pWorld;
	};

public:
	inline CWorld*			GetWorld()				{	return m_pWorld;			}


private:
	CWorld*			m_pWorld;
};