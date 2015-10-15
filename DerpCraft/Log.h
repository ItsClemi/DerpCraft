#pragma once

typedef enum LogColor : int { Black = 0, DarkBlue, Green, Blue, Pink = 5, Brown, Silver, DeepSilver = 9, BrightGreen, Cyan, Red };
typedef enum LogType : byte { Error, Success, Info };

void Log( _In_ LogType lType, _In_ char* szStr, _In_ ... );
void LogEx( _In_ WORD lColor, bool bShowTime, _In_ char* szStr, _In_ ... );