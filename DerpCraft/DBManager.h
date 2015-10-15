#pragma once

#include "User.h"

#define MAX_THREAD_SIZE 32

typedef struct tagDB_OVERLAPPED_PLUS
{
	OVERLAPPED		sOverlapped;
	int				nQueryMode;
	CUser*			pUser;
	tagDB_OVERLAPPED_PLUS()
	{
		pUser		= nullptr;
	}
}DB_OVERLAPPED_PLUS, *LPDB_OVERLAPPED_PLUS;

class CDBManager
{
public:
	CDBManager();
	virtual ~CDBManager();

	bool	CreateWorker( _In_ byte byProcess );
	void	CreatePlayer( CUser* pUser );

	DB_OVERLAPPED_PLUS*	Alloc();

	HANDLE	GetThreadHandle() { return hHandleGet; }
	HANDLE	InsThreadHandle() { return hHandleIns; }
	HANDLE	LogThreadHandle() { return hHandleLog; }
	HANDLE	UptThreadHandle() { return hHandleUpt; }
protected:
	HANDLE	hHandle;
	HANDLE	hHandleGet;
	HANDLE	hHandleIns;
	HANDLE	hHandleLog;
	HANDLE	hHandleUpt;

	HANDLE	hThreadGet[MAX_THREAD_SIZE];
	HANDLE	hThreadIns[MAX_THREAD_SIZE];
	HANDLE	hThreadLog[MAX_THREAD_SIZE];
	HANDLE	hThreadUpt[MAX_THREAD_SIZE];
	HANDLE	hThreadSave;
private:
};