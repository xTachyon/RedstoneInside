#ifndef REDI_BLOCK_HPP
# define REDI_BLOCK_HPP

namespace redi
{
struct Block
{
  std::uint16_t id;
  std::int8_t data;
  std::int8_t skylight;
  std::int8_t blocklight;

  Block(std::uint16_t id = 0, std::int8_t data = 0, std::int8_t skylight = 0, std::int8_t blocklight = 0)
    : id(id), data(data), skylight(skylight), blocklight(blocklight) {}
};

struct Blocks
{
  enum : std::uint16_t
  {
    Air = 0,
    Stone = 1,
    Grass = 2,
    Dirt = 3,
    Cobblestone = 4,
    Planks = 5,
    Sapling = 6,
    Bedrock = 7,
    FlowingWater = 8,
    Water = 9,
    FlowingLava = 10,
    Lava = 11,
    Sand = 12,
    Gravel = 13,
    GoldOre = 14,
    IronOre = 15,
    CoalOre = 16,
    Log = 17,
    Leaves = 18,
    Sponge = 19,
    Glass = 20,
    LapisOre = 21,
    LapisBlock = 22,
    Dispenser = 23,
    Sandstone = 24,
    Noteblock = 25,
    Bed = 26,
    GoldenRail = 27,
    DetectorRail = 28,
    StickyPiston = 29,
    Web = 30,
    Tallgrass = 31,
    Deadbush = 32,
    Piston = 33,
    PistonHead = 34,
    Wool = 35,
    PistonExtension = 36,
    YellowFlower = 37,
    RedFlower = 38,
    BrownMushroom = 39,
    RedMushroom = 40,
    GoldBlock = 41,
    IronBlock = 42,
    DoubleStoneSlab = 43,
    StoneSlab = 44,
    BrickBlock = 45,
    Tnt = 46,
    Bookshelf = 47,
    MossyCobblestone = 48,
    Obsidian = 49,
    Torch = 50,
    Fire = 51,
    MobSpawner = 52,
    OakStairs = 53,
    Chest = 54,
    RedstoneWire = 55,
    DiamondOre = 56,
    DiamondBlock = 57,
    CraftingTable = 58,
    Wheat = 59,
    Farmland = 60,
    Furnace = 61,
    LitFurnace = 62,
    StandingSign = 63,
    WoodenDoor = 64,
    Ladder = 65,
    Rail = 66,
    StoneStairs = 67,
    WallSign = 68,
    Lever = 69,
    StonePressurePlate = 70,
    IronDoor = 71,
    WoodenPressurePlate = 72,
    RedstoneOre = 73,
    LitRedstoneOre = 74,
    UnlitRedstoneTorch = 75,
    RedstoneTorch = 76,
    StoneButton = 77,
    SnowLayer = 78,
    Ice = 79,
    Snow = 80,
    Cactus = 81,
    Clay = 82,
    Reeds = 83,
    Jukebox = 84,
    Fence = 85,
    Pumpkin = 86,
    Netherrack = 87,
    SoulSand = 88,
    Glowstone = 89,
    Portal = 90,
    LitPumpkin = 91,
    Cake = 92,
    UnpoweredRepeater = 93,
    PoweredRepeater = 94,
    StainedGlass = 95,
    Trapdoor = 96,
    MonsterEgg = 97,
    Stonebrick = 98,
    BrownMushroomBlock = 99,
    RedMushroomBlock = 100,
    IronBars = 101,
    GlassPane = 102,
    MelonBlock = 103,
    PumpkinStem = 104,
    MelonStem = 105,
    Vine = 106,
    FenceGate = 107,
    BrickStairs = 108,
    StoneBrickStairs = 109,
    Mycelium = 110,
    Waterlily = 111,
    NetherBrick = 112,
    NetherBrickFence = 113,
    NetherBrickStairs = 114,
    NetherWart = 115,
    EnchantingTable = 116,
    BrewingStand = 117,
    Cauldron = 118,
    EndPortal = 119,
    EndPortalFrame = 120,
    EndStone = 121,
    DragonEgg = 122,
    RedstoneLamp = 123,
    LitRedstoneLamp = 124,
    DoubleWoodenSlab = 125,
    WoodenSlab = 126,
    Cocoa = 127,
    SandstoneStairs = 128,
    EmeraldOre = 129,
    EnderChest = 130,
    TripwireHook = 131,
    Tripwire = 132,
    EmeraldBlock = 133,
    SpruceStairs = 134,
    BirchStairs = 135,
    JungleStairs = 136,
    CommandBlock = 137,
    Beacon = 138,
    CobblestoneWall = 139,
    FlowerPot = 140,
    Carrots = 141,
    Potatoes = 142,
    WoodenButton = 143,
    Skull = 144,
    Anvil = 145,
    TrappedChest = 146,
    LightWeightedPressurePlate = 147,
    HeavyWeightedPressurePlate = 148,
    UnpoweredComparator = 149,
    PoweredComparator = 150,
    DaylightDetector = 151,
    RedstoneBlock = 152,
    QuartzOre = 153,
    Hopper = 154,
    QuartzBlock = 155,
    QuartzStairs = 156,
    ActivatorRail = 157,
    Dropper = 158,
    StainedHardenedClay = 159,
    StainedGlassPane = 160,
    Leaves2 = 161,
    Log2 = 162,
    AcaciaStairs = 163,
    DarkOakStairs = 164,
    Slime = 165,
    Barrier = 166,
    IronTrapdoor = 167,
    Prismarine = 168,
    SeaLantern = 169,
    HayBlock = 170,
    Carpet = 171,
    HardenedClay = 172,
    CoalBlock = 173,
    PackedIce = 174,
    DoublePlant = 175,
    StandingBanner = 176,
    WallBanner = 177,
    DaylightDetectorInverted = 178,
    RedSandstone = 179,
    RedSandstoneStairs = 180,
    DoubleStoneSlab2 = 181,
    StoneSlab2 = 182,
    SpruceFenceGate = 183,
    BirchFenceGate = 184,
    JungleFenceGate = 185,
    DarkOakFenceGate = 186,
    AcaciaFenceGate = 187,
    SpruceFence = 188,
    BirchFence = 189,
    JungleFence = 190,
    DarkOakFence = 191,
    AcaciaFence = 192,
    SpruceDoor = 193,
    BirchDoor = 194,
    JungleDoor = 195,
    AcaciaDoor = 196,
    DarkOakDoor = 197,
    EndRod = 198,
    ChorusPlant = 199,
    ChorusFlower = 200,
    PurpurBlock = 201,
    PurpurPillar = 202,
    PurpurStairs = 203,
    PurpurDoubleSlab = 204,
    PurpurSlab = 205,
    EndBricks = 206,
    Beetroots = 207,
    GrassPath = 208,
    EndGateway = 209,
    RepeatingCommandBlock = 210,
    ChainCommandBlock = 211,
    FrostedIce = 212,
    Magma = 213,
    NetherWartBlock = 214,
    RedNetherBrick = 215,
    BoneBlock = 216,
    StructureVoid = 217,
    StructureBlock = 255
  };
};

} // namespace redi

#endif // REDI_BLOCK_HPP
