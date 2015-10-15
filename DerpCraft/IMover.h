#pragma once

enum AuthType		: byte	{ Normal, Administrator };
enum GameMode		: byte	{ Survival, Creative, Adventure  };
enum MoverState		: DWORD	{ FlagDelete = 1, FlagInvisible, }; 
enum EntityAction	: byte	{ Crouch, Uncrouch, LeaveBed, StartSpring, StopSprint };
enum GameDifficult	: byte	{ GameModePeaceful = 0, GameModeEasy, GameModeNormal, GameModeHard  };