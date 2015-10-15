#pragma once
#include <rpcndr.h>

enum WeatherType	: byte { Sun = 0, Rain, Storm };
enum WorldDimension : char { Nether = -1, Overworld = 0, End = 1 };
enum BiomeType		: byte { BiomeOcean = 0, BiomePlains, BiomeDesert, BiomeExtremeHills, BiomeForest, BiomeTaiga, BiomeSwampland, 
								BiomeRiver, BiomeHell, BiomeNether = 8, BiomeSky, BiomeFrozenOcean, BiomeFrozenRiver, BiomeIcePlains, BiomeTundra = 12, 
								BiomeIceMountains, BiomeMushroomIsland, BiomeMushroomShore, BiomeBeach, BiomeDesertHills, BiomeForestHills, BiomeTaigaHills, 
								BiomeExtremeHillsEdge, BiomeJungle, BiomeJungleHills,  };

enum WorldType		: byte { Default, Flat, LargBioms };

typedef byte BlockId;
typedef byte BlockMeta;
