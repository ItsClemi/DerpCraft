#pragma once
#include "NetworkBase.h"

class CMover;
class CUser;
class CPacketHandler;

typedef void (CPacketHandler::*tPacket)(CUser* pPlayer, IPacketBuff& pPacket );

class CPacketHandler : public CNetworkBase
{
public:
	CPacketHandler();

private:
	void OnKeepAlive( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnHandshake( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnChat( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnPlayerGround( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnPlayerPosition( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnClientSettings( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnServerInformation( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnPlayerLook( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnEntityAction( _In_ CUser* pUser, _In_ IPacketBuff& pack );
	void OnPlayerPositionAndLook( _In_ CUser* pUser, _In_ IPacketBuff& pack );

public:
	void SendLogin( _In_ CUser* pUser );
	void SendDisconnect( _In_ CUser* pUser, _In_ wchar_t* wszStr );
	void SendChat( _In_ CUser* pUser, _In_ wchar_t* szMsg, ... );
	void SendTime( _In_ CUser* pUser, _In_ ULONGLONG lAgeWorld, _In_ ULONGLONG lTime );
	void SendChunk( _In_ CUser* pUser );
	void SendChunkBulk( _In_ CUser* pUser, _In_ int x, _In_ int z );
	void SendPosition( _In_ CUser* pUser );
	void SendGameState( _In_ CUser* pUser, _In_ bool bSingle, _In_ byte bReason, _In_ byte bGameMode );
	void SendExperience( _In_ CUser* pUser, _In_ float fBar, _In_ short sLevel, _In_ short sTotal );
	void SendHealth( _In_ CUser* pUser, _In_ short sHealth, _In_ short sFood, _In_ float fSaturation );
	void SendExplosion( _In_ CUser* pUser, _In_ DirectX::XMFLOAT3 vPos, _In_ float fRadius, _In_ int nRecord, _In_ byte bRecords, _In_ DirectX::XMFLOAT3 vMotion );
	void SendSpawnExperience( _In_ CUser* pUser, _In_ int nEntityID, _In_ DirectX::XMINT3 vPos, _In_ short sCount );

public:
	void UpdateServerInformation();

public:
	inline IPackBuff*	GetServerInformationPacket()	{ return m_pSrvrInfPacket;	}

private:
	virtual void UserMessageHandler( _In_ CUser* pUser, _In_ IPackBuff* pBuff, _In_ ULONG lMsgSize );

private:
	tPacket m_pPacketList[256];
	IPackBuff*	m_pSrvrInfPacket;

	CRITICAL_SECTION m_cs;
};

