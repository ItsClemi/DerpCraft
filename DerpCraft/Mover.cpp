#include "stdafx.h"
#include "Mover.h"


CMover::CMover(void)
{
	m_vPos.x = 0;
	m_vPos.y = 32;
	m_vPos.z = 0;
	m_bOnGround = true;
	m_fYaw = 0;
	m_fPitch = 0;
	m_dStance = 0;
}


CMover::~CMover(void)
{
}


void CMover::Process()
{
	
}