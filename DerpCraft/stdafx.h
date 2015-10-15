#pragma once

//////////////////////////////////////////////////////////////////////////
//=> Defines

#define SAFE_DELETE(p)			{ if(p) { delete(p);      (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete [] (p);  (p)=NULL; } }
#define CLOSE_HANDLE(p)			{ if(p) { CloseHandle(p); (p)=NULL; } }

#define SERVER_VER 60
#define ZLIB_WINAPI 
#define SAVE_PLAYER_TICK	60*1000 //=> 60 Sek.

#define BEGINTHREAD( pfnStartAddr, pvParam )  (HANDLE)_beginthreadex( NULL, NULL, pfnStartAddr, (void*)pvParam , NULL, NULL)

#define __USE_MSSQL

//////////////////////////////////////////////////////////////////////////
//=> Includes

#include <WinSock2.h>
#include <MSWSock.h>

#include <ws2tcpip.h>

#include <winsdkver.h>
#include <windows.h>

#include <assert.h>
#include <stdarg.h>
#include <cstdlib>
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>
#include <assert.h>
#include <conio.h>
#include <time.h>
#include <vector>
#include <new>
#include <codecvt>
#include <locale>
#include <stdint.h>
#include <mutex>
#include <map>
#include <functional>
#include <string>
#include <exception>
#include <process.h>
#include <array>

#include <DirectXMath.h>
#include "IStruct.h"
#include <math.h>

#include <xmmintrin.h>
#include <intrin.h>


#include <tbb\tbb.h>
#include <tbb\concurrent_queue.h>
#include <tbb\concurrent_unordered_map.h>
#include <tbb\concurrent_hash_map.h>

#include <zlib.h>

#include "Random.h"

#include "IQuery.h"
#include "IMover.h"
#include "IChunk.h"
#include "IBlock.h"
#include "IFile.h"
#include "FileParser.h"
#include "Log.h"
#include "Config.h"
#include "Command.h"
#include "DBManager.h"
#include "WorldManager.h"
#include "PacketHandler.h"

//////////////////////////////////////////////////////////////////////////
//=> Namespaces

using namespace std;
using namespace tbb;
using namespace DirectX;

//////////////////////////////////////////////////////////////////////////
//=> Librarys

#pragma comment(lib, "Ws2_32.lib")


class CSystemInfo : public SYSTEM_INFO
{
public:
	CSystemInfo()	{ GetSystemInfo( this ); }
};

//////////////////////////////////////////////////////////////////////////
//=> Global shared variables

extern time_t				g_tmNow;
extern CSystemInfo			g_sysinfo;
extern CConfig				g_conf;
extern CPacketHandler		g_srv;
extern CWorldManager		g_world;
extern CDBManager			g_DBManager;
extern CCommand				g_command;

//////////////////////////////////////////////////////////////////////////
//=> Global functions
