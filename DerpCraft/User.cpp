#include "stdafx.h"
#include "User.h"


CUser::CUser() : CMover()
{
	ZeroMemory( &m_addr,  sizeof( SOCKADDR_IN ) );
	ZeroMemory( &m_ovSend, sizeof( WSAOVERLAPPED ) );
	ZeroMemory( &m_ovRecv, sizeof(WSAOVERLAPPED) );
	m_pckRecvBuf		= new IPacketBuff();

	//=> Default.
	m_pWorld			= g_world.GetWorld( WorldDimension::Nether );
	m_bGameMode			= GameMode::Creative;
	m_bDifficult		= GameDifficult::GameModePeaceful;
	m_bAuthorization	= AuthType::Normal;
	m_nDbId				= NULL;
	
	//=> Look
	m_fYaw				= 0.0f;
	m_fPitch			= 0.0f;

	//=> Position
	m_vPos.x			= 0.0f;
	m_vPos.y			= 4.0f;
	m_vPos.z			= 0.0f;
	m_bOnGround			= true;

	//=> Level
	m_nExperience		= 7;
	m_bLevel			= 1;

	//=> Stats
	m_sHealth			= 20;
	m_sFood				= 10;
	m_fSaturation		= 5.0f;
}

CUser::~CUser(void)
{
	shutdown( GetSocket(), SD_BOTH );
	closesocket( GetSocket() );
	delete m_pckRecvBuf;
}

void CUser::Initialize( _In_ SOCKET hSocket )
{
	m_hSocket = hSocket;
	m_tmPing = _time64( nullptr );
}

bool CUser::Recv()
{
	memset( &m_ovRecv, 0, sizeof(WSAOVERLAPPED) );

	DWORD dwBytesRecvd	= 0, dwFlags = 0;
	int nRet = WSARecv( GetSocket(), GetRecvBuf(), 1, &dwBytesRecvd, &dwFlags, &m_ovRecv, nullptr);
	if((nRet == SOCKET_ERROR) && (WSA_IO_PENDING != (nRet = WSAGetLastError())))
	{
		Log( Error, "WSARecv failed with error: %d WSA:%i\n", nRet, WSAGetLastError());
		return false;
	}

	return true;
}



void CUser::Process()
{
	
}