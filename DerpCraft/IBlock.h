#pragma once
#include "IWorld.h"
#include <rpcndr.h>


struct BlockData
{
	inline BlockMeta	GetMetaData()	{	return m_bMeta;				}
	inline BlockId		GetId()			{	return m_bId;				}
	inline byte			GetSkyLight()	{	return m_bSkyLight;			}
	inline byte			GetBlockLight()	{	return m_bBlockLight;		}


	BlockMeta	m_bMeta;
	byte		m_bBlockLight;
	byte		m_bSkyLight;
	BlockId		m_bId;
};


enum BlockFace
{
	BLOCK_FACE_NONE   = -1,  // Interacting with no block face - swinging the item in the air
	BLOCK_FACE_BOTTOM = 0,   // Interacting with the bottom   face of the block
	BLOCK_FACE_TOP    = 1,   // Interacting with the top      face of the block
	BLOCK_FACE_NORTH  = 2,   // Interacting with the northern face of the block
	BLOCK_FACE_SOUTH  = 3,   // Interacting with the southern face of the block
	BLOCK_FACE_WEST   = 4,   // Interacting with the western  face of the block
	BLOCK_FACE_EAST   = 5,   // Interacting with the eastern  face of the block
};

enum DigStatus
{
	DIG_STATUS_STARTED   = 0,
	DIG_STATUS_CANCELLED = 1,
	DIG_STATUS_FINISHED  = 2,
	DIG_STATUS_DROP_HELD = 4,
	DIG_STATUS_SHOOT_EAT = 5,
} ;


enum BlockList
{
	BLOCK_AIR = 0, 
	BLOCK_STONE, 
	BLOCK_GRASS, 
	BLOCK_DIRT,
	BLOCK_COBBLESTONE, 
	BLOCK_PLANKS, 
	BLOCK_SAPLING, 
	BLOCK_BEDROCK, 
	BLOCK_WATER, 
	BLOCK_WATER_FLOWING, 
	BLOCK_LAVA,
	BLOCK_LAVA_FLOWING, 
	BLOCK_SAND, 
	BLOCK_GRAVEL, 
	BLOCK_ORE_GOLD, 
	BLOCK_ORE_IRON,
	BLOCK_ORE_COAL, 
	BLOCK_WOOD, 
	BLOCK_LEAVES, 
	BLOCK_SPONGE, 
	BLOCK_GLASS, 
	BLOCK_ORE_LAPIS, 
	BLOCK_LAPIS, 
	BLOCK_DISPENSER, 
	BLOCK_SANDSTONE, 
	BLOCK_NOTE, 
	BLOCK_BED, 
	BLOCK_RAIL_POWERED, 
	BLOCK_RAIL_TRIGGER, 
	BLOCK_PISTON_STICKY, 
	BLOCK_COBWEB, 
	BLOCK_GRASS_2, 
	BLOCK_DEAD_BUSH, 
	BLOCK_PISTON, 
	BLOCK_PISTON_EXTENSION, 
	BLOCK_WOOL, 
	BLOCK_PISTON_MOVING, 
	BLOCK_FLOWER_YELLOW, 
	BLOCK_ROSE_RED, 
	BLOCK_MUSHROOM_BROWN, 
	BLOCK_MUSHROOM_RED,
	BLOCK_GOLD, 
	BLOCK_IRON, 
	BLOCK_DOUBLE_STEP, 
	BLOCK_STEP, 
	BLOCK_BRICK, 
	BLOCK_TNT, 
	BLOCK_BOOKSHELF, 
	BLOCK_MOSE_COBBLESTONE, 
	BLOCK_OBSIDIAN, 
	BLOCK_TORCH, 
	BLOCK_FIRE, 
	BLOCK_MOB_SPAWNER,
	BLOCK_STAIRS_WOODEN,
	BLOCK_CHEST,
	BLOCK_REDSTONE_CABLE,
	BLOCK_DIAMOND_ORE,
	BLOCK_DIAMOND,
	BLOCK_WORKBENCH,
	BLOCK_WHEAT,
	BLOCK_SOIL,
	BLOCK_FURNACE,
	BLOCK_FURNACE_BURGNING,
	BLOCK_SIGN,
	BLOCK_DOOR_WOOD,
	BLOCK_LADDER,
	BLOCK_RAIL,
	BLOCK_STAIRS_STONE,
	BLOCK_SIGN_WALL,
	BLOCK_LEVER,
	BLOCK_PRESSURE_PLATE_STONE,
	BLOCK_DOOR_IRON,
	BLOCK_PRESSURE_PLATE_WOOD,
	BLOCK_REDSTONE_ORE,
	BLOCK_REDSTONE_ORE_GLOW,
	BLOCK_REDSTONE_TORCH,
	BLOCK_REDSTONE_TORCH_GLOW,
	BLOCK_BUTTON_STONE,
	BLOCK_SNOW,
	BLOCK_ICE,
	BLOCK_SNOWBLOCK,
	BLOCK_CACTUS,
	BLOCK_CLAY,
	BLOCK_SUGARCANE,
	BLOCK_RECORD_PLAYER,
	BLOCK_FENCE_WOOD,
	BLOCK_PUMPKIN,
	BLOCK_NETHER_STONE,
	BLOCK_NETHER_SAND,
	BLOCK_GLOWSTONE,
	BLOCK_PORTAL,
	BLOCK_PUMPKIN_GLOW,
	BLOCK_CAKE,
	BLOCK_REDSTONE_REPEATER,
	BLOCK_REDSTONE_REPEATER_GLOW,
	BLOCK_CHEST_LOCKED,
	BLOCK_TRAPDOOR,
	BLOCK_STONE_FISH,
	BLOCK_STONE_BRICK,
	BLOCK_MUSHROOM_BROWN_BLOCK,
	BLOCK_MUSHROOM_RED_BLOCK,
	BLOCK_IRON_GRID,
	BLOCK_PANE,
	BLOCK_MELON,
	BLOCK_PUMPKIN_PLANT,
	BLOCK_MELON_PLANT,
	BLOCK_RANK,
	BLOCK_FENCE_GATE,
	BLOCK_STAIRS_BRICK,
	BLOCK_STAIRS_STONEBRICK,
	BLOCK_MYCELIUM,
	BLOCK_LILYPAD,
	BLOCK_NETHER_BRICK,
	BLOCK_FENCE_NETHERBRICK,
	BLOCK_STAIRS_NETHERBRICK,
	BLOCK_NETHER_WARTS,
};