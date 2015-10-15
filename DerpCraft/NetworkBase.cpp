#include "stdafx.h"
#include "Mover.h"
#include "User.h"
#include "IPacketBuff.h"
#include "NetworkBase.h"


CNetworkBase::CNetworkBase()
{
	m_quePacket.get_allocator().allocate( 1024 );
	m_hIOCPRemoveClient = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL );
}

bool CNetworkBase::StartNetwork( _In_ u_short uPort )
{
	Log( Info, "--> Start network port [ %i ]", uPort );

	if( WSAStartup( 0x0202, &m_wsaData ) == SOCKET_ERROR )	
	{	
		Log( Error, "WSAStartup failed with error %d.", WSAGetLastError() );	
		return false;
	}

	if( ( m_hServerSocket = WSASocketW( AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED ) ) == INVALID_SOCKET )
	{
		Log( Error, "WSASocket failed with error %d", WSAGetLastError()  );
		return false;
	}

	sockaddr_in sin = {0};
	sin.sin_family	= AF_INET;
	sin.sin_port	= htons( uPort );

	if( ::bind( m_hServerSocket, (sockaddr*)&sin, sizeof( sockaddr_in ) ) == SOCKET_ERROR || 
		listen( m_hServerSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		Log( Error, "bind or listen failed with error %d", WSAGetLastError()  );
		return false;
	}
	
	m_bCoreCount = (byte)g_sysinfo.dwNumberOfProcessors * 2;
	m_hIOCP	= new HANDLE[ GetCoreCount() ];

	for( byte i = 0; i < GetCoreCount(); i++ )
	{
		if( ( m_hIOCP[i] = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL ) ) == nullptr || 
			!SetThreadPriority( (HANDLE)_beginthread( IoWorker, 0, this ), THREAD_PRIORITY_ABOVE_NORMAL ) )
			return false;
	}

	Log( Info, "--> Network Worker [ %i ]", GetCoreCount() );

	SetThreadPriority( (HANDLE)_beginthread( AcceptThread, 0, this ), THREAD_PRIORITY_ABOVE_NORMAL );
	_beginthread( AliveThread, 0, this );
	_beginthread( DeleteUserThread, 0, this );
	_beginthread( DeletePacketThread, 0, this );

	for( byte i = 0; i < 16; i++ )
		m_bServerId[i] = _bRandTable[ xRandom( 0, 15 ) ];
	m_bServerId[16] = '\0';

	Log( Info, "--> Server ID [ %s ]", m_bServerId );


	return false;
}

void CNetworkBase::SendToAllPlayers( _In_ IPackBuff* pBuff )
{
	pBuff->m_lSendCount = m_mapChild.size();

	for( auto i = m_mapChild.begin(); i != m_mapChild.end(); i++ )
		Send( i->second, pBuff );
}

void CNetworkBase::Send( _In_ CUser* pUser, _In_ IPackBuff* pBuff )
{
	pUser->m_ovSend.Pointer = pBuff;

	DWORD dwBytes;
	WSASend( pUser->GetSocket(), pBuff, 1, &dwBytes, 0, &pUser->m_ovSend, nullptr );
}

void CNetworkBase::AcceptLoop( )
{
	while( true )
	{
		CUser* pUser = new CUser();

_start:
		int nSize = sizeof( SOCKADDR_IN );
		SOCKET hClientSock = WSAAccept( GetServerSocket(), (SOCKADDR*)&pUser->m_addr, &nSize, nullptr, NULL );
		if( hClientSock  == INVALID_SOCKET )
			goto _start;

		pUser->Initialize( hClientSock );

		if( !m_mapChild.insert( concurrent_hash_map< SOCKET, CUser* >::value_type( hClientSock, pUser ) ) ||	
			!CreateIoCompletionPort( (HANDLE)hClientSock, GetIOCP( ( pUser->GetSocket() / 4 ) % GetCoreCount() ), (DWORD)pUser, GetCoreCount() ) )
		{
			Log( Error, "CreateIoCompletionPort Failure with error %d", WSAGetLastError( )  );
			goto _error;
		}
		
		if( pUser->Recv() )
			continue;
_error:
		RemoveClient( pUser );
	}	
}

void CNetworkBase::IoLoop()
{
	HANDLE hIoCp = GetIOCP( InterlockedIncrement( &m_lActIoWorker ) -1 );
	LPOVERLAPPED lpov;
	DWORD dwBytes;
	CUser* pUser;

	while( true )
	{
		GetQueuedCompletionStatus( hIoCp, &dwBytes, (DWORD*)&pUser, &lpov, INFINITE );

		if( dwBytes == 0 )
		{
			PostQueuedCompletionStatus( GetDeleteClientIOCP(), NULL, pUser->GetSocket(), nullptr );
			continue;
		}

		if( lpov == &pUser->m_ovRecv )
		{
			UserMessageHandler( pUser, pUser->GetRecvBuf(), lpov->InternalHigh );
			pUser->Recv();
		}
	}
}

void CNetworkBase::CheckAlive()
{
	IPacketBuff packet( KeepAlive, 5 );
	packet.GetBuffer()->m_bDelete = false;
	packet << 1337;
	
	while( true )
	{
		time_t tmNow = _time64(nullptr);
		for( auto i = m_mapChild.begin(); i != m_mapChild.end(); i++ )
		{
			CUser* pUser = i->second;

			if( ( tmNow - pUser->GetPingTime() ) > 5 )
				RemoveClient( pUser );
			else
				Send( pUser, packet.GetBuffer() );
		}

		SleepEx( 5000, TRUE );
	}
}

void CNetworkBase::DeleteClientLoop()
{
	LPOVERLAPPED lpov;
	DWORD dwBytes = 0;
	SOCKET hSocket;

	while( true )
	{
		GetQueuedCompletionStatus( GetDeleteClientIOCP(), &dwBytes, (DWORD*)&hSocket, &lpov, INFINITE );

		concurrent_hash_map< SOCKET, CUser* >::accessor a;
		if( m_mapChild.find( a, hSocket ) )
		{
			CUser* pUser = a->second;

			char szIp[16] = {0};
			pUser->GetAddr( szIp );
			Log( Info, "Dropping Client [ %s ]", szIp );

			
			if( !m_mapChild.erase( a ) )
				PostQueuedCompletionStatus( GetDeleteClientIOCP(), 0, (DWORD)pUser->GetSocket(), nullptr );
			else
				delete pUser;
		}
	}
}

void CNetworkBase::DeletePacketLoop()
{
	while(true)
	{
		if( !GetPacketDeallocQueue()->empty() )
		{
			IPackBuff* packet;
			if( !GetPacketDeallocQueue()->try_pop( packet ) )
				continue;

			Log( Info, ">> %x", packet );
			SAFE_DELETE( packet );
		}
		Sleep( 250 );
	}
}

void AcceptThread( _In_ void* ptr )
{
	CNetworkBase* pNet = (CNetworkBase*)ptr;
	pNet->AcceptLoop();
	_endthread();
}

void IoWorker( _In_ void* ptr )
{
	CNetworkBase* pNet = (CNetworkBase*)ptr;
	pNet->IoLoop();
	_endthread();
}

void AliveThread( _In_ void* ptr )
{
	CNetworkBase* pNet = (CNetworkBase*)ptr;
	pNet->CheckAlive();
	_endthread();
}

void DeleteUserThread( _In_ void* ptr )
{
	CNetworkBase* pNet = (CNetworkBase*)ptr;
	pNet->DeleteClientLoop();
	_endthread();
}

void DeletePacketThread( _In_ void* ptr )
{
	CNetworkBase* pNet = (CNetworkBase*)ptr;
	pNet->DeletePacketLoop();
	_endthread();
}