#pragma once
#include <WinSock2.h>

typedef enum PacketId : byte 
{ 
	KeepAlive = 0, 
	Login, 
	Handshake, 
	Chat, 
	TimeUpt,
	HealthUpt = 8,
	PlayerGround = 10, 
	PlayerPosition, 
	PlayerLook = 12, 
	PlayerPositionAndLook = 13,
	EntityAct = 19,
	SpawnExperience = 26,
	Experience = 43,
	ChunkData = 51, 
	MultiBlockChange, 
	BlockChange, 
	BlockAction, 
	BlockBreakAnimation, 
	MapChunkBulk, 
	Explosion = 60,
	GameState = 70,
	ClientSettings = 204,  
	ClientStatuses,
	EncryptionResponse = 252, 
	EncryptionRequest, 
	ServerPing, 
	Disconnect  
};