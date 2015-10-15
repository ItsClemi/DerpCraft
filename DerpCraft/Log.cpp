#include "stdafx.h"
#include "Log.h"


void Log( _In_ LogType lType, _In_ char* szStr, _In_ ... )
{
	char szBuff[256] = {0,};

	va_list arg;
	va_start(arg, szStr);
	_vsnprintf_s(szBuff, 256, szStr, arg);
	va_end(arg);
	
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	__time64_t time = _time64( nullptr ); 
	struct tm t;
	_localtime64_s(&t, &time);

	printf("[%02d:%02d:%02d]", t.tm_hour, t.tm_min, t.tm_sec );

	switch(lType)
	{
	case Error:
		{
			SetConsoleTextAttribute( hStdout, Red );
			printf("[ERROR]");

			break;
		}
	case Success:
		{
			SetConsoleTextAttribute( hStdout, Green );
			printf("[SUCCESS]");

			break;
		}
	case Info:
		{
			SetConsoleTextAttribute( hStdout, Blue );
			printf("[INFO]");

			break;
		}
	}
	SetConsoleTextAttribute( hStdout, Silver );
	printf( " %s\n", szBuff );		
}

void LogEx( _In_ WORD lColor, bool bShowTime, _In_ char* szStr, _In_ ... )
{
	char szBuff[256] = {0,};

	va_list arg;
	va_start(arg, szStr);
	_vsnprintf_s(szBuff, 256, szStr, arg);
	va_end(arg);

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if(bShowTime)
	{
		__time64_t time = _time64( nullptr ); 
		struct tm t;
		_localtime64_s(&t, &time);

		printf("[%02d:%02d:%02d] ", t.tm_hour, t.tm_min, t.tm_sec );
	}

	SetConsoleTextAttribute( hStdout, lColor );
	printf( "%s\n", szBuff );		
	SetConsoleTextAttribute( hStdout, Silver );
}