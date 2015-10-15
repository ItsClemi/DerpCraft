#include "StdAfx.h"
#include "Query.h"

#ifdef _DEBUG
#define CHECKRET 	if(!SQL_SUCCEEDED(ret)){ LogError(); __debugbreak(); }
#endif

CQuery::CQuery()
{
	m_hStmt	= NULL;
	m_hDbc	= NULL;
	m_hEnv	= NULL;

	for( byte i = 0; i < MAXCOL; i++ )
	{
		m_szCol[i] = new wchar_t[4192];
		memset( m_szCol[i], 0, 4192);
	}
}

CQuery::~CQuery()
{
	Disconnect();

	for( byte i = 0; i < MAXCOL; i++ )
		delete [] ( m_szCol[i] );
}

void CQuery::Disconnect()
{
	Clear();
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV,m_hEnv);
}

void CQuery::Clear()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

bool CQuery::Connect( _In_ char* szDBName, _In_ char* szLogin, _In_ char* szDBPass )
{
	SQLAllocHandle( SQL_HANDLE_ENV,SQL_NULL_HANDLE, &m_hEnv );
	SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER );

	SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );

	SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
	if(!SQL_SUCCEEDED( SQLConnect( m_hDbc, (SQLCHAR*)szDBName, SQL_NTS, (SQLCHAR*)szLogin, SQL_NTS, (SQLCHAR*)szDBPass, SQL_NTS ) ))
	{
		Log( Error, "Can't connect to database!!" );
		throw exception();
	}

	SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );
	return true;
}

bool CQuery::BindParameter( _In_ SQLUSMALLINT parameterNumber, _In_ SQLSMALLINT inputOutputType, _In_ SQLSMALLINT valueType, _In_ SQLSMALLINT parameterType, _In_ SQLUINTEGER columnSize, _In_ SQLSMALLINT decimalDigits, _In_ SQLPOINTER  parameterValuePtr, _In_ SQLINTEGER bufferLength, _In_ SQLINTEGER *strLen_or_IndPtr )
{
	return SQL_SUCCEEDED( SQLBindParameter( m_hStmt, parameterNumber, inputOutputType, valueType, parameterType, columnSize, decimalDigits, parameterValuePtr, bufferLength, strLen_or_IndPtr ) );
}

void CQuery::PrepareFetch()
{
	SQLNumResultCols(m_hStmt, &m_nCol);

	SWORD	nActualLen;
	SWORD	m_nSQLType;
	UDWORD	m_nPrecision;
	SWORD	m_nScale;
	SWORD	m_nNullability;

	for( short i = 0; i < m_nCol; i++ ) 
	{
		SQLBindCol( m_hStmt, i+1, SQL_C_CHAR, m_szCol[i], 8192, &m_nColLength[i] );
		SQLDescribeCol( m_hStmt, i+1, m_szColName[i], 16, &nActualLen, &m_nSQLType, &m_nPrecision, &m_nScale, &m_nNullability );
	}
}

void CQuery::Execute( _In_ wchar_t* szFormat, _In_ ... )
{
	wchar_t szBuffer[1024] = { NULL };

	va_list arg;
	va_start( arg, szFormat );
	int nLength = _vsnwprintf_s( szBuffer, 1024, szFormat, arg );
	va_end(arg);

	Clear();

	if( !SQL_SUCCEEDED( SQLExecDirectW( m_hStmt, (SQLWCHAR*)szBuffer, nLength ) ) )
		Log( Error, "Query Excute Failed!! [ %s ]", szBuffer );

	PrepareFetch();
}

short CQuery::FindCol( _In_ char* szName )
{
	for(short i = 0; i < m_nCol; i++) 
	{
		if ( strcmp(szName, (char*)m_szColName[i]) == 0)
			return i+1;
	}
	return -1;
}

#ifdef _DEBUG
void CQuery::LogError()
{
	UCHAR szMsg[SQL_MAX_MESSAGE_LENGTH] = {0};
	UCHAR szState[SQL_SQLSTATE_SIZE+1] = {0};
	SQLSMALLINT nLen;
	SQLINTEGER nError;
	int nIndex = 0;

	SQLRETURN ret = SQLError(m_hEnv, m_hDbc, m_hStmt, szState, &nError, szMsg, SQL_MAX_MESSAGE_LENGTH-1, &nLen );

	Log( Error, "Error %i SQLSTATE:%s error:%s", nError, (char*)szState,(char*)szMsg );
}
#endif