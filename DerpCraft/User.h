#pragma once
#include "Mover.h"
#include "IPacketBuff.h"
#include <DirectXMath.h>

class CInventory
{
public:

};

class CUser : public CMover
{
public:
	CUser();
	~CUser(void);

public:
	void	Initialize( _In_ SOCKET hSocket );
	bool	Recv();
	void	Process();


	template<size_t _size> 
	void GetAddr( char (&szAddr)[_size] )
	{
		sprintf_s( szAddr, 16, "%d.%d.%d.%d", m_addr.sin_addr.s_net, m_addr.sin_addr.s_host, m_addr.sin_addr.s_lh, m_addr.sin_addr.s_impno );
	}

	inline void				UpdatePingTime()	{	m_tmPing = _time64(nullptr);	}

public:
	void					SetAuthorization( _In_ AuthType Auth )		{	m_bAuthorization = Auth;	}
public:
	inline	SOCKET			GetSocket()			{	return m_hSocket;				}
	inline	IPackBuff*		GetRecvBuf()		{	return m_pckRecvBuf->m_pBuff;	}
	inline	WSABUF			GetSendBuf()		{	return m_pSendBuf;				}
	inline	wchar_t*		GetName()			{	return m_wszName;				}
	inline	wchar_t*		GetAccount()		{	return m_wszAccount;			}
	inline	time_t			GetPingTime()		{	return m_tmPing;				}

	inline	int				GetId()				{	return m_nDbId;					}
	inline	byte			GetGameMode()		{	return m_bGameMode;				}
	inline	bool			IsHardcore()		{	return false;					}
	inline	byte			GetViewRange()		{	return m_bViewRange;			}
	inline	byte			GetDifficult()		{	return m_bDifficult;			}
	inline	byte			GetAuthorization()	{	return m_bAuthorization;		}

public:
	CRITICAL_SECTION	m_cs;
	SOCKADDR_IN			m_addr;
	wchar_t				m_wszName[24];
	wchar_t				m_wszLocation[12];
	wchar_t				m_wszAccount[16];

	WSAOVERLAPPED		m_ovRecv;
	WSAOVERLAPPED		m_ovSend;

	int					m_nDbId;
	byte				m_bGameMode;
	byte				m_bDifficult;

	byte				m_bViewRange;
	byte				m_bChatFlag;
	byte				m_bAuthorization;
	byte				m_bEntityAction;
	byte				m_bLevel;
	int					m_nExperience;
	short				m_sHealth;
	short				m_sFood;
	float				m_fSaturation;
private:
	DirectX::XMINT3		m_vRespawnPos;

	SOCKET				m_hSocket;
	char				m_szRecBuff[1024];
	IPacketBuff*		m_pckRecvBuf;
	WSABUF				m_pSendBuf;
	volatile time_t		m_tmPing;
};

