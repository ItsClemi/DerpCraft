#include "stdafx.h"
#include "GeneratorBase.h"
#include "GeneratorFlat.h"
#include "GeneratorNether.h"
#include "GeneratorSky.h"
#include "WorldManager.h"


CWorldManager::CWorldManager(void)
{

}


void CWorldManager::CreateWorld( _In_ WorldDimension cDim, _In_ int nAllocSize, _In_ WorldType cWorldType )
{
	Log( Info, "--> Create world" );

	CWorld* pWorld = NULL;
	switch( cDim )
	{
	case WorldDimension::Overworld:		
		if( cWorldType == WorldType::Flat )
			pWorld = new CWorld( nAllocSize, new CGeneratorFlat );
		else
		{
		}
		break;
	case WorldDimension::Nether:
		pWorld = new CWorld( nAllocSize, new CGeneratorNether );
		break;
	}
	
	pWorld->SetDimension( cDim );
	pWorld->SetTime( 18000 );

	pWorld->m_vSpawnPos.x = NULL;
	pWorld->m_vSpawnPos.y = 1 + ( pWorld->GetChunkSection( 1, 1 )->GetHeight( 1, 1 ) * 2 );
	pWorld->m_vSpawnPos.z = NULL;

	m_mapWorld.insert( map<WorldDimension, CWorld*>::value_type( cDim, pWorld ) );
}

CWorld* CWorldManager::GetWorld( _In_ WorldDimension wDim )
{
	map<WorldDimension, CWorld*>::iterator i = m_mapWorld.find( wDim );
	return i != m_mapWorld.end() ? i->second : nullptr;
}


CWorldManager g_world;