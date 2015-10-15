#include "stdafx.h"


BOOL WINAPI ConsoleMessageHandler( _In_ DWORD dwCtrlType );

int _tmain( int argc, _TCHAR* argv[] )
{
	LogEx( FOREGROUND_GREEN|FOREGROUND_INTENSITY, false, "*******************************************************************************" );
	LogEx( FOREGROUND_GREEN|FOREGROUND_INTENSITY, false, "**                               ~~ DerpCraft ~~                             **" );
	LogEx( FOREGROUND_GREEN|FOREGROUND_INTENSITY, false, "**                            %s %s                           **", __DATE__, __TIME__ );
	LogEx( FOREGROUND_GREEN|FOREGROUND_INTENSITY, false, "*******************************************************************************\n" );

#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	SetConsoleCtrlHandler( ConsoleMessageHandler, TRUE );
	SetXSeed( GetTickCount() );
	
	g_conf.ReadConfig( "test.txt" );

	//if( !g_DBManager.CreateWorker( 2 ) )
	//{
	//	Log( Error, "Create DB Worker Failed." );
	//	goto FAILED;
	//}

	g_world.CreateWorld( WorldDimension::Overworld, 10, WorldType::Flat );
	g_world.CreateWorld( WorldDimension::Nether, 10, WorldType::Default );

	g_srv.StartNetwork( g_conf.GetServerPort() );
	g_srv.UpdateServerInformation();

	LogEx( FOREGROUND_GREEN|FOREGROUND_INTENSITY, false, "\n*******************************************************************************\n" );

	MSG msg;
	while(GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

FAILED:
	Log( Error, "Failed!!" );
	system( "pause" );
	return 0;
}

BOOL WINAPI ConsoleMessageHandler( _In_ DWORD dwCtrlType )
{
	if( dwCtrlType == CTRL_CLOSE_EVENT )
	{
		
		//_asm{int 3};
		return TRUE;
	}

	return FALSE;
}