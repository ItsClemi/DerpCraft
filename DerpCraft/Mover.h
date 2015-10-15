#pragma once

#include "World.h"

#include "IStruct.h"
#include <DirectXMath.h>



class CWorld;
class CMover
{
public:
	CMover(void);
	~CMover(void);

public:
	void Process();


public:
	CWorld*		GetWorld()			{	return m_pWorld;		}
	int			GetId()				{	return m_nId;			}
	void		AnnounceDelete()	{	AddFlag( FlagDelete );	}

	void		AddFlag( _In_ MoverState dwFlag )			{	m_dwFlags |= dwFlag;	}
	void		RemoveFlag( _In_ MoverState dwFlag )		{	m_dwFlags &= dwFlag;	}

public:
	BOOL		IsDelete()			{	return ( m_dwFlags & MoverState::FlagDelete );}


public:
	int					m_nId;

	CWorld*				m_pWorld;

	bool				m_bOnGround;
	double				m_dStance;
	float				m_fYaw;
	float				m_fPitch;

	DirectX::XMDOUBLE3	m_vPos;
	

	DWORD				m_dwFlags;
};

