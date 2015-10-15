#pragma once


class CConfig
{
public:
	CConfig();
public:
	void	ReadConfig( _In_ char* szFile );
	void	SaveConfig( _In_ char* szFile );
public:
	inline u_short	GetServerPort()		{ return m_uServerPort;	}
	inline int		GetMaxPlayer()		{ return _wtoi( m_wszMaxPlayer )-1;	}
	inline wchar_t*	GetMaxPlayerStr()	{ return m_wszMaxPlayer; }
	inline wchar_t*	GetMOTD()			{ return m_wszMOTD;	}
private:
	u_short	m_uServerPort;

	int		m_nMaxPlayer;

	wchar_t	m_wszMOTD[24];
	wchar_t	m_wszMaxPlayer[16];
};

