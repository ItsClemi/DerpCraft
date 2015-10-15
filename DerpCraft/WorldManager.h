#pragma once

#include "IWorld.h"
#include "World.h"

class CWorldManager
{
public:
	CWorldManager(void);
	
	void	CreateWorld( _In_ WorldDimension cDim, _In_ int nAllocSize, _In_ WorldType cWorldType );
	CWorld*	GetWorld( _In_ WorldDimension wDim );
public:
	std::map<WorldDimension, CWorld*> m_mapWorld; 
};

