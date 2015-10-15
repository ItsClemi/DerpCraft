#include "stdafx.h"
#include "IPacketBuff.h"
#include "IChunk.h"

#include "World.h"
#include "Mover.h"
#include "User.h"
#include "PacketHandler.h"


CPacketHandler::CPacketHandler(void)
{
	InitializeCriticalSection( &m_cs );

	ZeroMemory( m_pPacketList, sizeof( m_pPacketList ) );

	m_pPacketList[ PacketId::KeepAlive ]				= &CPacketHandler::OnKeepAlive;
	m_pPacketList[ PacketId::Handshake ]				= &CPacketHandler::OnHandshake;
	m_pPacketList[ PacketId::Chat	]					= &CPacketHandler::OnChat;
	m_pPacketList[ PacketId::PlayerGround ]				= &CPacketHandler::OnPlayerGround;
	m_pPacketList[ PacketId::PlayerPosition ]			= &CPacketHandler::OnPlayerPosition;
	m_pPacketList[ PacketId::PlayerLook]				= &CPacketHandler::OnPlayerLook;
	m_pPacketList[ PacketId::ClientSettings ]			= &CPacketHandler::OnClientSettings;
	m_pPacketList[ PacketId::ServerPing ]				= &CPacketHandler::OnServerInformation;
	m_pPacketList[ PacketId::EntityAct ]				= &CPacketHandler::OnEntityAction;
	m_pPacketList[ PacketId::PlayerPositionAndLook ]	= &CPacketHandler::OnPlayerPositionAndLook;
}


void CPacketHandler::UserMessageHandler( _In_ CUser* pUser, _In_ IPackBuff* pBuff, _In_ ULONG lMsgSize )
{
	void (CPacketHandler::*vptr)(CUser* pUser, IPacketBuff& pPacket ) = m_pPacketList[ (byte)pBuff->buf[0] ];
	if( vptr )
		(this->*(vptr))( pUser, IPacketBuff( pBuff->buf, lMsgSize ) );
#ifdef _DEBUG
	else	
		Log( Error, "Packet %d not found [ Size: %i ]", (byte)pBuff->buf[0], lMsgSize);
#endif
}

void CPacketHandler::OnKeepAlive( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pUser->UpdatePingTime();
}

void CPacketHandler::OnHandshake( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	byte bProtocollVer;
	pack >> bProtocollVer;
	pack.GetString( pUser->m_wszAccount );

	if( bProtocollVer != SERVER_VER )
	{
		SendDisconnect( pUser, L"Invalid: §4§nclient version");
		return;
	}

	if( GetUserCount() >= g_conf.GetMaxPlayer() )
	{
		SendDisconnect( pUser, L"Spieler Limit wurde erreicht, versuche es Später erneut!" );
		return;
	}

	SendLogin( pUser );
	return;

	LPDB_OVERLAPPED_PLUS pOverlapped = g_DBManager.Alloc();
	pOverlapped->pUser		= pUser;
	pOverlapped->nQueryMode	= QUERY_GET::GET_PLAYER;
	PostQueuedCompletionStatus( g_DBManager.GetThreadHandle(), 1, NULL, &pOverlapped->sOverlapped );
}

void CPacketHandler::OnChat( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	wchar_t wszStr[256];
	pack.GetString( wszStr );

	if( wszStr[0] == '/' )
	{
		//g_command.ParseCommand( pUser, wszStr );
	}
	else
	{
		if( pUser->GetAuthorization() == AuthType::Administrator )
			SendChat( nullptr, L"§c%s§f: %s", pUser->GetAccount(), wszStr );
		else
			SendChat( nullptr, L"%s: %s", pUser->GetAccount(), wszStr );
	}
}

void CPacketHandler::OnPlayerGround( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pack >> pUser->m_bOnGround;
}

void CPacketHandler::OnPlayerPosition( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pack >> pUser->m_vPos.x >> pUser->m_vPos.z >> pUser->m_dStance >> pUser->m_vPos.y >> pUser->m_bOnGround;
}

void CPacketHandler::OnPlayerLook( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pack >> pUser->m_fYaw >> pUser->m_fPitch >> pUser->m_bOnGround;
}

void CPacketHandler::OnPlayerPositionAndLook( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pack >> pUser->m_vPos.x >> pUser->m_vPos.z >> pUser->m_dStance >> pUser->m_vPos.y >> pUser->m_fYaw >> pUser->m_fPitch >> pUser->m_bOnGround;
}

void CPacketHandler::OnClientSettings( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	pack.GetString( pUser->m_wszLocation );
	pack >> pUser->m_bViewRange >> pUser->m_bChatFlag >> pUser->m_bDifficult;

	SendChat( pUser, L"Welcome §a%s§f!", pUser->GetAccount() );
	SendChat( nullptr, L"§a%s§f joined the server!", pUser->GetAccount() );
}

void CPacketHandler::OnServerInformation( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{ 	
	EnterCriticalSection( &m_cs );
	Send( pUser, GetServerInformationPacket() );
	LeaveCriticalSection( &m_cs );
}

void CPacketHandler::OnEntityAction( _In_ CUser* pUser, _In_ IPacketBuff& pack )
{
	int nPlayerID = NULL;
	pack >> nPlayerID >> pUser->m_bEntityAction;
	Log( Info, "%s: ID [ %08i ][ %d ][ %s ]", __FUNCTION__, nPlayerID, pUser->m_bEntityAction, pUser->GetAccount() );
}

void CPacketHandler::SendLogin( _In_ CUser* pUser )
{
	CWorld* pWorld = pUser->GetWorld();

	IPacketBuff packet( Login, 12 + ( pWorld->GetworldType()->size() * 2 ) );
	packet << pUser->GetId();
	packet.WriteString( pWorld->GetworldType() );
	packet << (byte)pUser->GetGameMode() << (byte)pWorld->GetDimension() << pUser->GetDifficult() << (byte)60; 
	Send( pUser, packet.GetBuffer() );

	SendPosition( pUser );
	SendChunk( pUser );

	switch( pWorld->GetDimension() )
	{
	case Nether:	SendTime( pUser, NULL, 18000 );						break;
	case Overworld:	SendTime( pUser, NULL, pWorld->GetTime() );			break;	
	case End:		SendTime( pUser, NULL, 18000 );						break;
	}
	
	//SendExplosion( pUser, DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f ), 3.0, 1, 1, DirectX::XMFLOAT3( 0.0f, 25.0f, 5.0f ) );		//=> Failed.
	SendHealth( pUser, pUser->m_sHealth, pUser->m_sFood, pUser->m_fSaturation );			
	SendExperience( pUser, 0.0, pUser->m_bLevel, pUser->m_nExperience );	
	//SendGameState( pUser, true, 1, NULL );	//=> Works ;)
	
	//DirectX::XMINT3		vSpawn;
	//vSpawn.x = ( int )pUser->m_vPos.x;
	//vSpawn.y = ( int )pUser->m_vPos.y;
	//vSpawn.z = ( int )pUser->m_vPos.z;

	//SendSpawnExperience( pUser, 1000, vSpawn, 20 );
}

void CPacketHandler::SendGameState( _In_ CUser* pUser, _In_ bool bSingle, _In_ byte bReason, _In_ byte bGameMode )
{
	IPacketBuff packet( PacketId::GameState, 3 );
	packet << bReason << bGameMode;

	if( bSingle )
		Send( pUser, packet.GetBuffer() );
	else
		SendToAllPlayers( packet.GetBuffer() );
}

void CPacketHandler::SendTime( _In_ CUser* pUser, _In_ ULONGLONG lAgeWorld, _In_ ULONGLONG lTime )
{
	IPacketBuff packet( PacketId::TimeUpt, 17 );
	packet << lAgeWorld << lTime;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendExperience( _In_ CUser* pUser, _In_ float fBar, _In_ short sLevel, _In_ short sTotal )
{
	IPacketBuff packet( PacketId::Experience, 1 + sizeof( float ) + ( sizeof( short ) * 2 ));
	packet << fBar << sLevel << sTotal;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendSpawnExperience( _In_ CUser* pUser, _In_ int nEntityID, _In_ DirectX::XMINT3 vPos, _In_ short sCount )
{
	IPacketBuff packet( PacketId::SpawnExperience, 19 );
	packet << nEntityID << vPos.x << vPos.y << vPos.z << sCount;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendHealth( _In_ CUser* pUser, _In_ short sHealth, _In_ short sFood, _In_ float fSaturation )
{
	IPacketBuff packet( PacketId::HealthUpt, 9 );
	packet << sHealth << sFood << fSaturation;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendExplosion( _In_ CUser* pUser, _In_ DirectX::XMFLOAT3 vPos, _In_ float fRadius, _In_ int nRecord, _In_ byte bRecords, _In_ DirectX::XMFLOAT3 vMotion )
{
	IPacketBuff packet( PacketId::Explosion, 45 + 3*nRecord );
	packet << vPos.x << vPos.y << vPos.z << fRadius << nRecord;
	for( byte i = NULL; i < nRecord; i++ )
		packet << bRecords << bRecords << bRecords;
	packet << vMotion.x << vMotion.y << vMotion.z;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendChat( _In_ CUser* pUser, _In_ wchar_t* szMsg, ... )
{
	wchar_t szBuffer[256] = {0};

	va_list arg;
	va_start(arg, szMsg);
	int n = _vsnwprintf_s( szBuffer, 256, szMsg, arg );
	va_end(arg);

	IPacketBuff packet( PacketId::Chat, 3 + n * 2 );
	packet.WriteString( szBuffer, n );

	if( pUser )
		Send( pUser, packet.GetBuffer() );
	else
		SendToAllPlayers( packet.GetBuffer() );
}

void CPacketHandler::SendPosition( _In_ CUser* pUser )
{
	CWorld* pWorld = pUser->GetWorld();

	IPacketBuff packet( PacketId::PlayerPositionAndLook, 42 );
	packet << (double)pWorld->m_vSpawnPos.x << (double)pWorld->m_vSpawnPos.y << (double)pWorld->m_vSpawnPos.z << pUser->m_fYaw << pUser->m_fPitch << pUser->m_bOnGround;
	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::SendChunk( _In_ CUser* pUser )
{
	CWorld* pWorld = pUser->GetWorld();
	pWorld->ForVisibilityChunkRange( pUser, [=]( _Out_ IChunkSection* pSect, _Out_ int x, _Out_ int y )
	{
		USHORT mask = 1;

		for( byte i = 0; i < pSect->GetSectionHeight(); i++ )
		{
			IChunk* pChunk = pSect->At( i );
			if(!pChunk)	break;
			
			if( i > 1 )
				pChunk->m_bGroundUpContinuous = true;

			byte* _bMembuff = new byte[12544];
			byte* bBuff		= new byte[12544];
			uLong uCompressed = 12544; 
		
			memcpy( &bBuff[0],		pChunk->m_bData, 4096 );
			memcpy( &bBuff[4096],	pChunk->m_bMeta, 2048 );
			memcpy( &bBuff[6144],	pChunk->m_bLight, 2048 );
			memcpy( &bBuff[8192],	pChunk->m_bSkylight, 2048 );
			memcpy( &bBuff[10240],	pChunk->m_bBlockAdd, 2048 );
			memcpy( &bBuff[12288],	pSect->m_bBiome, 256 );

			compress2( _bMembuff, &uCompressed, bBuff, uCompressed, Z_BEST_COMPRESSION );

			delete [] bBuff;

			pChunk->m_uPrimaryBitMap |= mask;
			mask <<= 1;

			IPacketBuff packet( ChunkData, 18 + uCompressed );
			packet << x << y << pChunk->m_bGroundUpContinuous << (short)pChunk->m_uPrimaryBitMap << (short) pChunk->m_uAddBitMap << (int) uCompressed;
			packet.AddArray( _bMembuff, uCompressed );
			Send( pUser, packet.GetBuffer() );

			delete [] _bMembuff;

			
		}
	});	
}

void CPacketHandler::SendChunkBulk( _In_ CUser* pUser, _In_ int x, _In_ int z )
{
	CWorld* pWorld = pUser->GetWorld();

	// 	pWorld->ForVisibilityChunkRange( pUser, [=]( IChunkSection* pSect ){
	// 
	// 		ULONG uChunkDataSize = 0;
	// 
	// 		//IPacketBuff packet( MapChunkBulk, 8 + uChunkDataSize ( pSect->GetUsedHeight() * 12 ) );
	// 
	// 
	// 	} );


}

void CPacketHandler::SendDisconnect( _In_ CUser* pUser, _In_ wchar_t* wszStr )
{
	int nSize = wcslen( wszStr );

	IPacketBuff packet( Disconnect,  3 + nSize * 2);
	packet.WriteString( wszStr,  nSize );

	Send( pUser, packet.GetBuffer() );
}

void CPacketHandler::UpdateServerInformation()
{
	EnterCriticalSection( &m_cs );

	wstring Reply = L"§";

	Reply.append(L"1");
	Reply.push_back(0);
	Reply.append(L"1.5.1");
	Reply.push_back(0);
	Reply.append(L"60");
	Reply.push_back(0);
	Reply.append( g_conf.GetMOTD() );
	Reply.push_back( 0 );
	Reply.append( L"255" );
	Reply.push_back( 0 );
	Reply.append( g_conf.GetMaxPlayerStr() );

	IPacketBuff packet( Disconnect, 3 + Reply.size() * 2 );
	packet.WriteString( (wchar_t*)Reply.c_str(), Reply.size() );
	m_pSrvrInfPacket = packet.GetBuffer();
	m_pSrvrInfPacket->m_bDelete = false;


	LeaveCriticalSection( &m_cs );
}



CPacketHandler g_srv;