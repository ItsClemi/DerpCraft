#pragma once
#include <WinSock2.h>
#include "PacketList.h"
#include <assert.h>
#include "Log.h"

	
#define TWriteF( type, var, func )	assert( m_szCurPos + sizeof( type ) <= m_szMaxPos );					*(type*)m_szCurPos = func( var );	m_szCurPos += sizeof( type );	return *this;
#define TReadF( type, var, func ) if( m_szCurPos + sizeof(type) > m_szMaxPos ){ var = 0; return *this; }	var = func(*(type*)m_szCurPos);		m_szCurPos += sizeof( type );	return *this;

#define TWrite( type, var )			assert( m_szCurPos + sizeof( type ) <= m_szMaxPos );					*(type*)m_szCurPos = var;			m_szCurPos += sizeof( type );	return *this;
#define TRead( type, var ) if( m_szCurPos + sizeof(type) > m_szMaxPos ){ var = 0; return *this; }			var = *(type*)m_szCurPos;			m_szCurPos += sizeof( type );	return *this;


typedef struct IPackBuff : public WSABUF
{
	IPackBuff( ULONG uLen )
	{
		buf = new char[ uLen ];
		len = uLen;
		m_lSendCount = 0;
		m_bDelete = true;
	}

	bool	m_bDelete;
	long	m_lSendCount;
}IPackBuff;


class IPacketBuff
{
public:
	inline IPacketBuff()
	{
		m_pBuff = new IPackBuff( 1024 );
		m_szCurPos = m_pBuff->buf;
		m_szMaxPos = m_szCurPos + m_pBuff->len;
	}

	inline IPacketBuff( IPackBuff* pBuff )
	{
		m_szCurPos = pBuff->buf;
		m_szMaxPos = m_szCurPos + pBuff->len;
	}

	inline IPacketBuff( PacketId pId, int uLen )		//write function
	{
		m_pBuff = new IPackBuff( uLen );
		m_szCurPos = m_pBuff->buf;
		m_szMaxPos = m_szCurPos + uLen;
		*(this) << (byte)pId;
	}

	inline IPacketBuff( char* szPacketBuff, int uLen ) //read function
	{
		m_szCurPos = szPacketBuff + sizeof( byte );
		m_szMaxPos = szPacketBuff + uLen;
	}

public:
	inline IPacketBuff& operator << ( byte b )			{	TWrite( byte, b );					}
	inline IPacketBuff& operator >> ( byte& b )			{	TRead( byte, b );					}

	inline IPacketBuff& operator << ( bool b )			{	return IPacketBuff::operator << ( ( byte )b );			}
	inline IPacketBuff& operator >> ( bool& b )			{	return IPacketBuff::operator >>	( ( byte& )b );			}

	inline IPacketBuff& operator << ( u_short s )		{	TWriteF( u_short, s, htons );		}
	inline IPacketBuff& operator >> ( u_short& s )		{	TReadF( u_short, s, ntohs ); 		}

	inline IPacketBuff& operator << ( short s )			{	return IPacketBuff::operator << ( ( u_short )s );		}
	inline IPacketBuff& operator >> ( short& s )		{	return IPacketBuff::operator >> ( ( u_short& )s );		}

	inline IPacketBuff& operator << ( ULONG l )			{	TWriteF( ULONG, l, htonl );			}
	inline IPacketBuff& operator >> ( ULONG& l )		{	TReadF( ULONG, l, ntohl );			}

	inline IPacketBuff& operator << ( int i )			{	return IPacketBuff::operator << ( ( ULONG )i );			}
	inline IPacketBuff& operator >> ( int& i )			{	return IPacketBuff::operator >> ( ( ULONG& )i );		}

	inline IPacketBuff& operator << ( float f )			
	{	
		assert( m_szCurPos + sizeof( float ) <= m_szMaxPos );

		int nval; 
		memcpy(&nval, &f, 4);
		*(this)<<(int)nval;
		return *this;
	}

	inline IPacketBuff& operator >> ( float& f )		
	{	
		if(m_szCurPos + sizeof(float) > m_szMaxPos)
		{
			f = 0;
			return *this;
		}

		u_long i = ntohl(*(int*)m_szCurPos); 
		m_szCurPos += sizeof(int);
		memcpy(&f, &i, 4 );

		return *this;
	}

	inline IPacketBuff& operator << ( ULONGLONG l )		{	TWriteF( ULONGLONG, l, ntohll );	}
	inline IPacketBuff& operator >> ( ULONGLONG& l )	{	TReadF( ULONGLONG, l, ntohll );		}

	inline IPacketBuff& operator << ( __int64 i )		{	return IPacketBuff::operator << ( ( ULONGLONG )i );		}
	inline IPacketBuff& operator >> ( __int64& i )		{	return IPacketBuff::operator >> ( ( ULONGLONG& )i )	;	}

	inline IPacketBuff& operator << ( double d )		
	{
		assert( m_szCurPos + sizeof( __int64 ) <= m_szMaxPos );

		__int64 nval;
		memcpy( &nval, &d, 8 );
		*(this)<<(__int64)nval;
		return *this;
	}

	inline IPacketBuff& operator >> ( double& d )		
	{	
		if(m_szCurPos + sizeof(__int64) > m_szMaxPos)
		{
			d = 0;
			return *this;
		}

		_ULonglong i = ntohll(*(__int64*)m_szCurPos); 
		m_szCurPos += sizeof(__int64);
		memcpy(&d, &i, 8 );

		return *this;
	}

	inline IPacketBuff& operator << ( wchar_t* sz )		{	byte bLen = (byte)wcslen(sz); WriteString( sz, bLen ); return *this;	}

public:
	template<u_short _size> inline void GetString(wchar_t (&wszStr)[_size])
	{ 
		assert( m_szCurPos + sizeof(short) <= m_szMaxPos);

		u_short sStringSize = 0;
		*(this)>>sStringSize;

		if( sStringSize > _size)
			sStringSize = _size;

		assert( m_szCurPos + (sStringSize*2) <= m_szMaxPos);

		u_short i = 0;
		for(; i < sStringSize; i++)
		{
			wszStr[i] = ntohs(*(wchar_t*)m_szCurPos);
			m_szCurPos += sizeof(wchar_t);
		}
		
		wszStr[ sStringSize ] = '\0';
	}

	inline void WriteString( wchar_t* wszStr, u_short uSize)
	{
		assert( m_szCurPos + uSize + sizeof( u_short ) <= m_szMaxPos);

		*(this) << uSize;
		for( u_short i = 0; i < uSize; i++ )
			*(this) << (u_short)wszStr[i];
	}

	inline void WriteString( std::wstring* wszStr )
	{
		assert( m_szCurPos + wszStr->size() + sizeof( u_short ) <= m_szMaxPos);

		*(this) << ( u_short )wszStr->size();

		for( u_short i = 0; i < ( u_short )wszStr->size(); i++ )
			*(this) << ( u_short )wszStr->at( i );
	}

	inline void AddArray( byte* ptr, int nSize)
	{
		assert( m_szCurPos + nSize <= m_szMaxPos);
		memcpy(m_szCurPos, ptr, nSize );
		m_szCurPos += nSize;
	}

	inline void GetArray(byte* bPtr, int nSize)
	{
		if(m_szCurPos + nSize > m_szMaxPos)
			return;

		memcpy(bPtr, m_szCurPos, nSize );
		m_szCurPos += nSize;
	}
	
	inline void Clear() 
	{
		m_szCurPos = m_pBuff->buf;
	}

public:
#ifdef _DEBUG
	inline	IPackBuff*	GetBuffer()		
	{	
		if( m_pBuff->len < (ULONG)( m_szMaxPos - m_szCurPos )  )
			Log(Error, "PacketBuff ( %i ) is too big [ written: %i | created: %i ]", (int)( m_szMaxPos - m_szCurPos ),  m_pBuff->len );
		
		return m_pBuff;
	}
#else
	inline	IPackBuff*	GetBuffer()		{		return m_pBuff;			}
#endif

public:
	IPackBuff* m_pBuff;

	char*	m_szCurPos;
	char*	m_szMaxPos;
};

