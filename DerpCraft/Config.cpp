#include "stdafx.h"
#include "FileParser.h"
#include "Config.h"


CConfig::CConfig()
{
	m_uServerPort		= 25565;

	wcscpy_s( m_wszMOTD, L"DerpCraft" );
	wcscpy_s( m_wszMaxPlayer, L"10" );
}

void CConfig::ReadConfig( _In_ char* szFile )
{
	return;
	Log( Info, "Attempt to read config [ %s ]", szFile );

	CFileParser parser;
	if( parser.ReadFile( szFile ) )
	{
		while( parser.GetToken() != FINISHED )
		{
			if( parser.GetString() == L"ServerPort:" )
				m_uServerPort = parser.GetInt();
		}
	}
	else
		Log( Info, "Read failed. Defaulting settings!\n" );
}

void CConfig::SaveConfig( _In_ char* szFile )
{
	
}


CConfig g_conf;