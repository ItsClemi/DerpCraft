#include "stdafx.h"
#include "GeneratorSky.h"


CGeneratorSky::CGeneratorSky(void)
{
	bLastHeight = 0;
}

void CGeneratorSky::SetupGenerator( _In_ CWorld* pWorld )
{
	CGeneratorBase::SetupGenerator( pWorld );
}
