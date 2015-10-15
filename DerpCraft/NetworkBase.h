#pragma once
#include "Mover.h"
#include "User.h"

static byte _bRandTable[] = { 
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
	'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '=', '-'
};

class CNetworkBase
{
public:
	CNetworkBase();

public:
	bool StartNetwork( _In_ u_short uPort );
	void Shutdown();
	void RemoveClient( _In_ CUser* pUser )		{	PostQueuedCompletionStatus( GetIOCP( ( pUser->GetSocket() / 4 ) % GetCoreCount() ), NULL, (DWORD)pUser, nullptr );		}
	void Send( _In_ CUser* pUser, _In_ IPackBuff* pBuff );
	void SendToAllPlayers( _In_ IPackBuff* pBuff );
public:
	void AcceptLoop();
	void IoLoop();
	void CheckAlive();
	void DeleteClientLoop();
	void DeletePacketLoop();
public:
	virtual void UserMessageHandler( _In_ CUser* pPlayer, _In_ IPackBuff* pBuff, _In_ ULONG lMsgSize ) = 0;
public:
	inline	WSADATA		GetWsaData()			{	return m_wsaData;				}
	inline	SOCKET		GetServerSocket()		{	return m_hServerSocket;			}
	inline	byte*		GetServerId()			{	return m_bServerId;				}
	inline	HANDLE		GetDeleteClientIOCP()	{	return m_hIOCPRemoveClient;		}
	inline	tbb::concurrent_queue< IPackBuff* >* GetPacketDeallocQueue() {		return &m_quePacket;		}	
	inline	int			GetUserCount()				{	return m_mapChild.size();	}
private:
	inline	byte		GetCoreCount()				{	return m_bCoreCount;	}
	inline	HANDLE		GetIOCP( int ioWrk )		{	return m_hIOCP[ioWrk];		}
	inline	CUser*		GetUser( SOCKET hSocket )	{	tbb::concurrent_hash_map< SOCKET, CUser* >::accessor a; return m_mapChild.find( a, hSocket )?  a->second : nullptr;		}
private:
	byte		m_bServerId[17];
	SOCKET		m_hServerSocket;
	long		m_lActIoWorker;

	byte		m_bCoreCount;

	HANDLE*		m_hIOCP;
	HANDLE		m_hIOCPRemoveClient;

	WSADATA		m_wsaData;
	tbb::concurrent_hash_map< SOCKET, CUser* >	m_mapChild; 
	tbb::concurrent_queue< IPackBuff* >			m_quePacket;
};

void AcceptThread( _In_ void* ptr );
void IoWorker( _In_ void* ptr );
void AliveThread( _In_ void* ptr );
void DeleteUserThread( _In_ void* ptr );
void DeletePacketThread( _In_ void* ptr );


template<size_t _size>
void GetSocketAddr( SOCKET hSocket, char (&szAddr)[_size] )
{
	SOCKADDR_IN sin;
	ZeroMemory( &sin, sizeof(SOCKADDR_IN) );

	int namelen	= sizeof( SOCKADDR_IN );
	if( getpeername( hSocket, (SOCKADDR *)&sin, &namelen ) == SOCKET_ERROR )
		szAddr[0] = '\0';
	else
		sprintf_s( szAddr, 16, "%d.%d.%d.%d", sin.sin_addr.s_net, sin.sin_addr.s_host, sin.sin_addr.s_lh, sin.sin_addr.s_impno );
}
