#pragma once
#include "GeneratorBase.h"

class CGeneratorSky : public CGeneratorBase
{
public:
	CGeneratorSky(void);
public:
	virtual void SetupGenerator( _In_ CWorld* pWorld );
public:
	byte	bLastHeight;
};


