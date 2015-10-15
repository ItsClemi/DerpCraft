#pragma once
#include <stdio.h>
#include <io.h>

class IFile
{
public:
	~IFile()
	{
		fclose(m_pFileStream);
	}

public:
	inline	bool	Open( _In_ char* szFileName, _In_ char* szMode = "r" )			{ 	 return fopen_s( &m_pFileStream, szFileName, szMode ) == 0;		}
	inline	bool	Open( _In_ wchar_t* wszFileName, _In_ wchar_t* wszMode = L"r" )	{	 return _wfopen_s(&m_pFileStream, wszFileName, wszMode ) == 0;	}

	inline	size_t	Write( _In_ void* ptr, _In_ size_t size)						{	 return fwrite( ptr, size, 1, m_pFileStream );					}
	inline	size_t	Read( _In_ void* ptr, _In_ size_t size,  _In_ size_t n = 1 )	{ 	 return fread( ptr, size, n, m_pFileStream );					}
	

public:
	inline	byte	ReadByte()					{	return (byte)fgetc( m_pFileStream );														}
	inline	char	ReadChar()					{	return (char)fgetc( m_pFileStream );														}
	inline	wchar_t ReadWideChar()				{	return (wchar_t)fgetwc( m_pFileStream );													}

	inline	DWORD	ReadDword()					{	DWORD nRet = 0; fread( &nRet, sizeof( DWORD ),	1, m_pFileStream );		return nRet;		}
	inline	int		ReadInt()					{	int nRet = 0;	fread( &nRet, sizeof( int ),	1, m_pFileStream );		return nRet;		}
	inline	float	ReadFloat()					{	float nRet = 0; fread( &nRet, sizeof( float ),	1, m_pFileStream );		return nRet;		}

	inline	__int64	ReadInt64()					{	__int64 nRet = 0;	fread( &nRet, sizeof( __int64 ),	1, m_pFileStream );	return nRet;	}
	inline	ULONG64	ReadUInt64()				{	ULONG64 nRet = 0;	fread( &nRet, sizeof( ULONG64 ),	1, m_pFileStream );	return nRet;	}
	inline	double	ReadDouble()				{	double nRet = 0;	fread( &nRet, sizeof( double ),		1, m_pFileStream );	return nRet;	}

public:
	inline	void	WriteByte( _In_ byte b )			{	putc( (char)b, m_pFileStream );							}
	inline	void	WriteChar( _In_ char c )			{	putc( c, m_pFileStream );								}
	inline	void	WriteWideChar( _In_ wchar_t w )		{	_putw( w, m_pFileStream );								}

	inline	void	WriteDword( _In_ DWORD d )			{	fwrite( &d, sizeof( DWORD ),	1, m_pFileStream );		}
	inline	void	WriteInt( _In_ int i )				{	fwrite( &i, sizeof( int ),		1, m_pFileStream );		}
	inline	void	WriteFloat( _In_ float f )			{	fwrite( &f, sizeof( float ),	1, m_pFileStream );		}

	inline	void	WriteInt64( _In_ __int64 i )		{	fwrite( &i, sizeof( __int64 ),	1, m_pFileStream );		}
	inline	void	WriteUInt64( _In_ ULONG64 i )		{	fwrite( &i, sizeof( ULONG64 ),	1, m_pFileStream );		}
	inline	void	WriteDouble( _In_ double d )		{	fwrite( &d, sizeof( double ),	1, m_pFileStream );		}

public:
	inline  bool	IsEndOfFile()		{	return !feof( m_pFileStream );						} 

	inline 	long	GetLength() 		{	return _filelength( _fileno( m_pFileStream ) );		}
	inline	int		GetHandle()			{	return _fileno( m_pFileStream );					}
	inline	int		GetError()			{	return ferror( m_pFileStream );						}
	inline	FILE*	GetFileStream()		{	return m_pFileStream;								}

	
private:
	FILE*	m_pFileStream;

};
