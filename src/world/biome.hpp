#ifndef REDI_WORLD_BIOME_HPP
#define REDI_WORLD_BIOME_HPP

#include <cstdint>

namespace redi
{

enum class Biome : std::uint8_t
{
  Ocean = 0,
  Plains = 1,
  Desert = 2,
  ExtremeHills = 3,
  Forest = 4,
  Taiga = 5,
  Swampland = 6,
  River = 7,
  Hell = 8,
  Sky = 9,
  FrozenOcean = 10,
  FrozenRiver = 11,
  IceFlats = 12,
  IceMountains = 13,
  MushroomIsland = 14,
  MushroomIslandShore = 15,
  Beaches = 16,
  DesertHills = 17,
  ForestHills = 18,
  TaigaHills = 19,
  SmallerExtremeHills = 20,
  Jungle = 21,
  JungleHills = 22,
  JungleEdge = 23,
  DeepOcean = 24,
  StoneBeach = 25,
  ColdBeach = 26,
  BirchForest = 27,
  BirchForestHills = 28,
  RoofedForest = 29,
  TaigaCold = 30,
  TaigaColdHills = 31,
  RedwoodTaiga = 32,
  RedwoodTaigaHills = 33,
  ExtremeHillsWithTrees = 34,
  Savanna = 35,
  SavannaRock = 36,
  Mesa = 37,
  MesaRock = 38,
  MesaClearRock = 39,
  Void = 127,
  MutatedPlains = 129,
  MutatedDesert = 130,
  MutatedExtremeHills = 131,
  MutatedForest = 132,
  MutatedTaiga = 133,
  MutatedSwampland = 134,
  MutatedIceFlats = 140,
  MutatedJungle = 149,
  MutatedJungleEdge = 151,
  MutatedBirchForest = 155,
  MutatedBirchForestHills = 156,
  MutatedRoofedForest = 157,
  MutatedTaigaCold = 158,
  MutatedRedwoodTaiga = 160,
  MutatedRedwoodTaigaHills = 161,
  MutatedExtremeHillsWithTrees = 162,
  MutatedSavanna = 163,
  MutatedSavannaRock = 164,
  MutatedMesa = 165,
  MutatedMesaRock = 166,
  MutatedMesaClearRock = 167
};
  
} // namespace redi

#endif // REDI_WORLD_BIOME_HPP
