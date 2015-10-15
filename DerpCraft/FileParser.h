#pragma once

#include "IFile.h"

enum ParserTokeType	: byte	{ FINISHED, HEX, STRING, _WORD, NUMBER, TEMP };


class CFileParser : IFile
{
public:
	CFileParser(void);
	~CFileParser(void);

public:
	bool ReadFile( _In_ char* szFileName )			{ return Open( szFileName,  "r" )  && PrepareFile();	}
	bool ReadFile( _In_ wchar_t* wszFileName )		{ return Open( wszFileName, L"r" ) && PrepareFile();	}
	


private:
	bool			PrepareFile();
	

public:
	ParserTokeType	GetToken();
	std::wstring	GetString();
	int				GetInt();
	__int64			GetInt64();
	double			GetDouble();

	DWORDLONG		GetHex();

public:
	inline	bool			IsEndOfFile()			{	return m_bParserState == FINISHED;		}
	inline	ParserTokeType	GetTokenType()			{	return m_bParserState;					}

private:
	inline	long	GetFileBufferLength()			{	return m_lFileBufferLength;				}


private:
	inline	bool	IsWhiteSpace( wchar_t c )		{	return c > 0 && c <= 0x20;				}
	inline	bool	IsNumber( wchar_t c )			{	return c >= L'0' && c <= L'9';			} 

private:
	wchar_t*		m_wszFileBuff;
	wchar_t*		m_wszBuff;
	long			m_lFileBufferLength;

	std::wstring	m_wszToken;
	ParserTokeType	m_bParserState;

};

