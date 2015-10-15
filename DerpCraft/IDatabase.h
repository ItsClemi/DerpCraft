#pragma once

#include "User.h"

#ifdef __USE_MSSQL
namespace MSSQL
{
	//=> Get
	class IDatabaseGet
	{
	public:
		IDatabaseGet()
		{
			pQuery = new CQuery;
			pQuery->Connect( "MINECRAFT", "Crasy_02_Test", "@crasy_back!" );
		}
		CQuery* pQuery;
	public:
		inline bool GetPlayer( _In_ CUser* pUser )
		{
			pQuery->Execute( L"USER_STR 'S1', @Account='%s'", pUser->GetAccount() );
			while( pQuery->Fetch() )
				switch( pQuery->GetInt( "nError" ) )
				{
				case 0:		return false;		break;													//=> Account not found.
				case 1:		g_srv.SendDisconnect( pUser, L"Dein Account wurde Blockiert." );	break;	//=> Account blocked.
				case 2:		goto LOAD;			break;													//=> Account OK.
				}												
LOAD:
			pQuery->Execute( L"USER_STR 'S2', @Account='%s'", pUser->GetAccount() );
			while( pQuery->Fetch() )
			{
				pUser->m_nDbId			= pQuery->GetInt( "ID" );
				Log( Error, "Get Player.. %d", pUser->m_nDbId );
				//pUser->m_wszName		= pQuery->GetChar( "Name" );
				pUser->m_bGameMode		= pQuery->GetByte( "Mode" );
				pUser->m_bAuthorization	= pQuery->GetByte( "Authority" );
				pUser->m_bLevel			= pQuery->GetByte( "PlayerLv" );
				pUser->m_nExperience	= pQuery->GetInt( "Experience" );
			}
			return true;
		}
	};
	//=> Insert 
	class IDatabaseIns
	{
	public:
		IDatabaseIns()
		{
			pQuery = new CQuery;
			pQuery->Connect( "MINECRAFT", "Crasy_02_Test", "@crasy_back!" );
		}
		CQuery* pQuery;
	public:
		inline void InsPlayer( _In_ CUser* pUser )
		{
			char szAddress[16];
			pUser->GetAddr( szAddress );
			Log( Info, "Insert Player..%s", szAddress );
			pQuery->Execute( L"USER_STR 'I1', @Account='%s', @Name='%s', @Address='%s', @Authority=%d, @GameMode=%d, @Position_X=%f, @Position_Y=%f, @Position_Z=%f, @World=%d, @Experience=%d, @PlayerLv=%d, @LookYaw=%d, @LookPitch=%d, @Ground=%d, @Health=%d, @Food=%d, @Saturation=%f",
				pUser->GetAccount(),
				pUser->GetName(),
				szAddress,
				pUser->GetAuthorization(),
				pUser->GetGameMode(),
				pUser->m_vPos.x,
				pUser->m_vPos.y,
				pUser->m_vPos.z,
				pUser->m_pWorld->GetDimension(),
				pUser->m_nExperience,
				pUser->m_bLevel,
				pUser->m_fYaw,
				pUser->m_fPitch,
				pUser->m_bOnGround,
				pUser->m_sHealth,
				pUser->m_sFood,
				pUser->m_fSaturation );
		}
	};
};
#else
namespace MYSQL
{

};
#endif
