#include "stdafx.h"
#include "FileParser.h"


#define READ_TABLE( func ) 		\
	switch( m_wszToken[0] ) \
		{ \
	case '=':		return -1; \
	case '-':		return -func(  m_wszToken.c_str() ); \
	default:		return func( m_wszToken.c_str() ); \
		}

#define READ_TABLE_PROTOTYPE( type, func )		\
	if( GetTokenType() == HEX )	{ \
		m_wszBuff += 2; \
		wchar_t* pPos = m_wszBuff; \
		\
		while(!IsWhiteSpace(*pPos) && *pPos ) \
			pPos++; \
		\
		m_wszToken = wstring( m_wszBuff, pPos - m_wszBuff); \
		m_wszBuff = pPos; \
		return (type)GetHex(); \
	} \
	else if( m_wszToken[0] ) \
		READ_TABLE( func ); \


CFileParser::CFileParser()
{
	m_wszToken.reserve( 255 );
}


CFileParser::~CFileParser()
{
	delete [] m_wszFileBuff;
}


bool CFileParser::PrepareFile()
{
	m_lFileBufferLength = GetLength();

	byte bFileEncoding[2];
	Read( &bFileEncoding, 2 );

	if( bFileEncoding[0] == 0xFF && bFileEncoding[1] == 0xFE )		//Unicode
	{  
		long lSize = ( m_lFileBufferLength / 2 );
		m_wszFileBuff = new wchar_t[ lSize ];
		Read( m_wszFileBuff, GetFileBufferLength() - 2 );

		m_lFileBufferLength = lSize;
	}
	else if( bFileEncoding[0] == 0xFE && bFileEncoding[1] == 0xFF ) //Unicode big Endian
		throw new exception( "Not" );
	else if( bFileEncoding[0] == 0xEF && bFileEncoding[1] == 0xBB ) //UTF 8  
		throw new exception( "Not" );
	else //multibyte encoding
	{ 
		char* szFileBuff = new char[ GetFileBufferLength() ];
		m_wszFileBuff = new wchar_t[ GetFileBufferLength() + 1 ];

		m_wszFileBuff[0] = bFileEncoding[0];
		m_wszFileBuff[1] = bFileEncoding[1];

		Read( szFileBuff, GetFileBufferLength() - 2 );

		MultiByteToWideChar(CP_NONE, NULL, szFileBuff, -1, &m_wszFileBuff[2], GetFileBufferLength() );
		delete [] szFileBuff;
	}

	m_wszFileBuff[ GetFileBufferLength() -1 ] = '\0';
	m_wszBuff = m_wszFileBuff;
	m_bParserState = TEMP;

	return true;
}


ParserTokeType CFileParser::GetToken()
{
_begin:
	m_wszToken[0] = '\0';
	

	while( IsWhiteSpace( *m_wszBuff ) && *m_wszBuff )
		m_wszBuff++;

	if( *m_wszBuff == L'/' ) 
	{	
		if( *++m_wszBuff == L'/' )
		{
			while( *m_wszBuff != L'\r' && *m_wszBuff )
				m_wszBuff++;

			m_wszBuff += 2;
		}
		else if( *m_wszBuff == L'*')
		{
			do 
			{
				while( *m_wszBuff != L'*' && *m_wszBuff )
					m_wszBuff++;

				if( !*m_wszBuff )
				{
					m_bParserState = FINISHED;
					return FINISHED;
				}

			} while( *++m_wszBuff != L'/' );

			m_wszBuff++;
			goto _begin;
		}
		else m_wszBuff--;
	}

	if( !*m_wszBuff )
	{
		m_bParserState = FINISHED;
		return FINISHED;
	}

	if( *m_wszBuff == '"')
	{
		m_bParserState = STRING;
		return STRING;
	}
	else if( *m_wszBuff == '0' && ( *(m_wszBuff + 1) == 'x' ||  *(m_wszBuff + 1) == 'X' ) )
	{
		m_bParserState = HEX;
		return HEX;
	}
	else if( IsNumber( *m_wszBuff ) )
	{
		m_bParserState = NUMBER;
		return NUMBER;
	}

	m_bParserState = _WORD;
	return _WORD;
}

wstring CFileParser::GetString()
{
	if( GetTokenType() == STRING )
	{
		wchar_t* pPos = ++m_wszBuff;

		while( *pPos != L'"' && *pPos != L'\r' && *pPos )
		{
			if( ( m_wszBuff - pPos ) > 255 )
				throw new exception( "String too long!" );

			pPos++;
		}

		m_wszToken = wstring( m_wszBuff, pPos - m_wszBuff);
		m_wszBuff = pPos;
	}
	else
	{
		wchar_t* pPos = m_wszBuff;

		while( *pPos != L'\r' && *pPos && !IsWhiteSpace( *pPos ) )
		{
			if( ( m_wszBuff - pPos ) > 255 )
				throw new exception( "String too long!" );

			pPos++;
		}

		m_wszToken = wstring( m_wszBuff, pPos - m_wszBuff);
		m_wszBuff = pPos;
	}

	return m_wszToken;
}


int CFileParser::GetInt()
{
	GetToken();

	if( GetTokenType() == HEX )	
	{ 
		m_wszBuff += 2; 
		wchar_t* pPos = m_wszBuff; 
		
		while(!IsWhiteSpace(*pPos) && *pPos ) 
			pPos++; 
		
			m_wszToken = wstring( m_wszBuff, pPos - m_wszBuff); 
			m_wszBuff = pPos; 
			return (int)GetHex(); 
	} 
	else 
	{
		wchar_t* pPos = m_wszBuff;

		while(!IsWhiteSpace(*pPos) && *pPos )
			pPos++;



		m_wszToken = wstring( m_wszBuff, pPos - m_wszBuff );

		m_wszBuff = pPos;

		if( m_wszToken[0] ) 
			READ_TABLE( _wtoi ); 
	}

	return 0;
}

__int64 CFileParser::GetInt64()
{
	READ_TABLE_PROTOTYPE( __int64, _wtoi64 );
	return 0;
}


double CFileParser::GetDouble()
{
	if(GetTokenType() != NUMBER)
		return 0.0f;

	if( m_wszToken[0] )
	{
		READ_TABLE( _wtof );
	}

	return 0.0f;
}

DWORDLONG CFileParser::GetHex()
{
	if( m_wszToken[0] )
	{
		DWORDLONG dwlNumber = 0;
		DWORD dwMulCnt = 0;
		wchar_t cVal;

		for( int i = m_wszToken.length() - 1; i >= 0; i--)
		{
			cVal = m_wszToken[ i ];

			if( cVal >= L'a' )	
				cVal = ( cVal - L'a' ) + 10;
			else 
				cVal -= L'0';

			dwlNumber |= (DWORDLONG)cVal << dwMulCnt;
			dwMulCnt += 4;
		}

		return dwlNumber;
	}

	return 0x0;
}