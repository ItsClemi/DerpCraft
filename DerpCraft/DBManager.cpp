#include "StdAfx.h"
#include "DBManager.h"
#include "Query.h"
#include "IDatabase.h"
#include "User.h"

u_int __stdcall GetThread( LPVOID lpParam );
u_int __stdcall	InsThread( LPVOID lpParam );
u_int __stdcall LogThread( LPVOID lpParam );
u_int __stdcall UptThread( LPVOID lpParam );
u_int __stdcall	SavThread( LPVOID lpParam );

CDBManager::CDBManager()
{

}

CDBManager::~CDBManager()
{

}

DB_OVERLAPPED_PLUS* CDBManager::Alloc()
{
	DB_OVERLAPPED_PLUS*	pOverlapped = new DB_OVERLAPPED_PLUS;
	memset( pOverlapped, NULL, sizeof(DB_OVERLAPPED_PLUS) );
	return pOverlapped;
}

bool CDBManager::CreateWorker( _In_ byte byProcess )
{
	Log( Info, "--> Create database worker" );

	SYSTEM_INFO si;
	GetSystemInfo( &si );
	byte byThreads = (byte)si.dwNumberOfProcessors * byProcess;

	if( byThreads > MAX_THREAD_SIZE )
	{
		Log( Error, "Invalid Thread Size!! [ %d ]", byThreads );
		return false;
	}

	hHandle		= CreateEvent( NULL, FALSE, FALSE, NULL );
	hHandleGet	= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL );
	hHandleIns	= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL );
	hHandleLog	= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL );
	hHandleUpt	= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, NULL );

	char szOutput[64];
	DWORD dwThreadId = NULL;
	for( int i = 0; i != si.dwNumberOfProcessors * byProcess; i++ )
	{
		hThreadGet[i]	= (HANDLE)_beginthreadex( NULL, NULL, GetThread, (LPVOID)this, NULL, (unsigned*)&dwThreadId ); 
#ifdef _DEBUG
		sprintf_s( szOutput, "Created DB Worker Thread!! [ GET ] [ %d ]\n", dwThreadId );
		::OutputDebugString( szOutput );
#endif
		hThreadIns[i]	= (HANDLE)_beginthreadex( NULL, NULL, InsThread, (LPVOID)this, NULL, (unsigned*)&dwThreadId );
#ifdef _DEBUG
		sprintf_s( szOutput, "Created DB Worker Thread!! [ INS ] [ %d ]\n", dwThreadId );
		::OutputDebugString( szOutput );
#endif
		hThreadLog[i]	= (HANDLE)_beginthreadex( NULL, NULL, LogThread, (LPVOID)this, NULL, (unsigned*)&dwThreadId ); 
#ifdef _DEBUG
		sprintf_s( szOutput, "Created DB Worker Thread!! [ LOG ] [ %d ]\n", dwThreadId );
		::OutputDebugString( szOutput );
#endif
		hThreadUpt[i]	= (HANDLE)_beginthreadex( NULL, NULL, UptThread, (LPVOID)this, NULL, (unsigned*)&dwThreadId ); 
#ifdef _DEBUG
		sprintf_s( szOutput, "Created DB Worker Thread!! [ UPT ] [ %d ]\n", dwThreadId );
		::OutputDebugString( szOutput );
#endif

		SetThreadPriority( hThreadGet[i], THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriority( hThreadUpt[i], THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriority( hThreadLog[i], THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriority( hThreadIns[i], THREAD_PRIORITY_BELOW_NORMAL );

#ifndef _DEBUG
		WaitForSingleObject( hHandle, 1000 ); //=> Wait 1 Sec.
#endif
	}

	hThreadSave	= (HANDLE)_beginthreadex( NULL, NULL, SavThread, (LPVOID)this, NULL, (unsigned*)&dwThreadId ); 
	SetThreadPriority( hThreadSave, THREAD_PRIORITY_HIGHEST );

#ifdef _DEBUG
	sprintf_s( szOutput, "Created DB Worker Thread!! [ SAV ] [ %d ]\n", dwThreadId );
	::OutputDebugString( szOutput );
#endif

	return true;
}

void CDBManager::CreatePlayer( CUser* pUser )
{
	LPDB_OVERLAPPED_PLUS pOverlapped = Alloc();
	pOverlapped->pUser		= pUser;
	pOverlapped->nQueryMode	= QUERY_INS::INS_PLAYER;
	PostQueuedCompletionStatus( InsThreadHandle(), 1, NULL, &pOverlapped->sOverlapped );
}

u_int __stdcall GetThread( LPVOID lpParam )
{
	CDBManager* pDBManager	= (CDBManager*)lpParam;

	DWORD dwBytesTransferred				= NULL;
	DWORD dwCompletionKey					= NULL;
	LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus	= NULL;

#ifdef __USE_MSSQL
	MSSQL::IDatabaseGet* pDatabase			= new MSSQL::IDatabaseGet;
#endif

	while( true )
	{
		GetQueuedCompletionStatus( pDBManager->GetThreadHandle(), &dwBytesTransferred, &dwCompletionKey, (LPOVERLAPPED*)&lpDbOverlappedPlus, INFINITE );
		switch( lpDbOverlappedPlus->nQueryMode )
		{
		case QUERY_GET::GET_PLAYER:		pDatabase->GetPlayer( lpDbOverlappedPlus->pUser ) ? g_srv.SendLogin( lpDbOverlappedPlus->pUser ) : pDBManager->CreatePlayer( lpDbOverlappedPlus->pUser );	break;
		}
	}

	return NULL;
}

u_int __stdcall InsThread( LPVOID lpParam )
{
	CDBManager* pDBManager	= (CDBManager*)lpParam;

	DWORD dwBytesTransferred				= NULL;
	DWORD dwCompletionKey					= NULL;
	LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus	= NULL;

#ifdef __USE_MSSQL
	MSSQL::IDatabaseIns* pDatabase			= new MSSQL::IDatabaseIns;
#endif

	while( true )
	{
		GetQueuedCompletionStatus( pDBManager->InsThreadHandle(), &dwBytesTransferred, &dwCompletionKey, (LPOVERLAPPED*)&lpDbOverlappedPlus, INFINITE );
		switch( lpDbOverlappedPlus->nQueryMode )
		{
		case QUERY_INS::INS_PLAYER:		pDatabase->InsPlayer( lpDbOverlappedPlus->pUser );	break;
		}
	}

	return NULL;
}

u_int __stdcall LogThread( LPVOID lpParam )
{
	CDBManager* pDBManager	= (CDBManager*)lpParam;

	DWORD dwBytesTransferred				= NULL;
	DWORD dwCompletionKey					= NULL;
	LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus	= NULL;

	while( true )
	{
		GetQueuedCompletionStatus( pDBManager->LogThreadHandle(), &dwBytesTransferred, &dwCompletionKey, (LPOVERLAPPED*)&lpDbOverlappedPlus, INFINITE );
	}

	return NULL;
}

u_int __stdcall UptThread( LPVOID lpParam )
{
	CDBManager* pDBManager	= (CDBManager*)lpParam;

	DWORD dwBytesTransferred				= NULL;
	DWORD dwCompletionKey					= NULL;
	LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus	= NULL;

	while( true )
	{
		GetQueuedCompletionStatus( pDBManager->UptThreadHandle(), &dwBytesTransferred, &dwCompletionKey, (LPOVERLAPPED*)&lpDbOverlappedPlus, INFINITE );
	}

	return NULL;
}

u_int __stdcall SavThread( LPVOID lpParam )
{
	CDBManager* pDBManager	= (CDBManager*)lpParam;

	while( true )
	{
		WaitForSingleObject( CreateEvent( NULL, FALSE, FALSE, NULL ), SAVE_PLAYER_TICK );
	}

	return NULL;
}
CDBManager	g_DBManager;