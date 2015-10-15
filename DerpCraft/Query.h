#pragma once
#define MAXCOL		64
#define CQUERYNULL -100

#include <sql.h>
#include <sqlext.h>

class CQuery
{
public:
	CQuery();
	~CQuery();
	inline void Clear();

	bool Connect( _In_ char* szDBName, _In_ char* szLogin, _In_ char* szDBPass );
	void Disconnect();

	void Execute( _In_ wchar_t* szFormat, _In_ ... );
	bool BindParameter( _In_ SQLUSMALLINT parameterNumber, _In_ SQLSMALLINT inputOutputType, _In_ SQLSMALLINT valueType, _In_ SQLSMALLINT parameterType, _In_ SQLUINTEGER columnSize, _In_ SQLSMALLINT decimalDigits, _In_ SQLPOINTER  parameterValuePtr, _In_ SQLINTEGER bufferLength, _In_ SQLINTEGER *strLen_or_IndPtr );

	inline bool Fetch()
	{
		return SQL_SUCCEEDED( SQLFetch( m_hStmt ) );
	}
private:
	void			PrepareFetch();	
public:
	inline	byte	GetByte( char* szColumn )	{ return _wtoi( m_szCol[FindCol(szColumn)-1] ); }
	inline	double	GetDouble( char* szColumn )	{ return _wtof( m_szCol[FindCol(szColumn)-1] ); }
	inline	int		GetInt( char* szColumn )	{ return _wtoi( m_szCol[FindCol(szColumn)-1] ); }
	inline	__int64	GetInt64( char* szColumn )	{ return _wtoi64( m_szCol[FindCol(szColumn)-1] ); }
	inline	float	GetFloat( char* szColumn )	{ return (float)_wtof( m_szCol[FindCol(szColumn)-1] ); }
	inline	wchar_t	GetChar( char* szColumn )	{ return m_szCol[FindCol(szColumn)-1][0]; }

	static bool		EnableConnectionPooling()		{ return SQL_SUCCEEDED( SQLSetEnvAttr( NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER ) );  }
private:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;

	wchar_t*	m_szCol[MAXCOL];
	SQLCHAR		m_szColName[MAXCOL][32];
	SQLINTEGER	m_nColLength[MAXCOL];

	SQLSMALLINT m_nCol;

	short FindCol( _In_ char* szName );
#ifdef _DEBUG
	void LogError();
#endif
};

